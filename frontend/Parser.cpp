//
// Created by Arman Sadeghi on 2/12/22.
//
// branch test
/**
 * Parser class for a simple interpreter.
 *
 * (c) 2020 by Ronald Mak
 * Department of Computer Science
 * San Jose State University
 */
#include <string>
#include <map>

#include "Token.h"
#include "Parser.h"

namespace frontend {

    using namespace std;

    set<TokenType> Parser::statementStarters;
    set<TokenType> Parser::statementFollowers;
    set<TokenType> Parser::relationalOperators;
    set<TokenType> Parser::simpleExpressionOperators;
    set<TokenType> Parser::termOperators;

    void Parser::initialize()
    {
        // Tokens that can start a statement.
        statementStarters.insert(BEGIN);
        statementStarters.insert(IDENTIFIER);
        statementStarters.insert(REPEAT);
        statementStarters.insert(WHILE); // while added
        statementStarters.insert(FOR); // for added
        statementStarters.insert(TokenType::IF); // if added
        statementStarters.insert(CASE); // case added
        //statementStarters.insert(ELSE); // else should be added
        statementStarters.insert(TokenType::WRITE);
        statementStarters.insert(TokenType::WRITELN);

        // Tokens that can immediately follow a statement.
        statementFollowers.insert(SEMICOLON);
        statementFollowers.insert(END);
        statementFollowers.insert(UNTIL);
        statementFollowers.insert(END_OF_FILE);

        relationalOperators.insert(EQUALS);
        relationalOperators.insert(LESS_THAN);
        relationalOperators.insert(GREATER_THAN); // > added
        relationalOperators.insert(LESS_THAN_EQUALS); // <= added
        relationalOperators.insert(GREATER_THAN_EQUALS); // >= added
        relationalOperators.insert(NOT_EQUALS); // <> added

        simpleExpressionOperators.insert(PLUS);
        simpleExpressionOperators.insert(MINUS);
        simpleExpressionOperators.insert(OR);

        termOperators.insert(STAR);
        termOperators.insert(SLASH);
        termOperators.insert(DIV); // div added
        termOperators.insert(MOD); // mod added
        termOperators.insert(TokenType::AND); // and added
    }

    Node *Parser::parseProgram()
    {
        Node *programNode = new Node(NodeType::PROGRAM);

        currentToken = scanner->nextToken();  // first token!

        if (currentToken->type == TokenType::PROGRAM)
        {
            currentToken = scanner->nextToken();  // consume PROGRAM
        }
        else syntaxError("Expecting PROGRAM");

        if (currentToken->type == IDENTIFIER)
        {
            string programName = currentToken->text;
            symtab->enter(programName);
            programNode->text = programName;

            currentToken = scanner->nextToken();  // consume program name
        }
        else syntaxError("Expecting program name");

        if (currentToken->type == SEMICOLON)
        {
            currentToken = scanner->nextToken();  // consume ;
        }
        else syntaxError("Missing ;");

        if (currentToken->type != BEGIN) syntaxError("Expecting BEGIN");

        // The PROGRAM node adopts the COMPOUND tree.
        programNode->adopt(parseCompoundStatement());

        if (currentToken->type == SEMICOLON) syntaxError("Expecting .");
        return programNode;
    }

    Node *Parser::parseStatement()
    {
        Node *stmtNode = nullptr;
        int savedLineNumber = currentToken->lineNumber;
        lineNumber = savedLineNumber;

        switch (currentToken->type)
        {
            case IDENTIFIER : stmtNode = parseAssignmentStatement(); break;
            case BEGIN :      stmtNode = parseCompoundStatement();   break;
            case REPEAT :     stmtNode = parseRepeatStatement();     break;
            case WHILE :      stmtNode = parseWhileStatement();      break; // while added
            case FOR :        stmtNode = parseForStatement();        break; // for added
            //case IF :         stmtNode = parseIfStatement();         break; // if added
            case CASE :       stmtNode = parseCaseStatement();       break; // case added
            case WRITE :      stmtNode = parseWriteStatement();      break;
            case WRITELN :    stmtNode = parseWritelnStatement();    break;
            case SEMICOLON :  stmtNode = nullptr; break;  // empty statement

            default : syntaxError("Unexpected token");
        }

        if (stmtNode != nullptr) stmtNode->lineNumber = savedLineNumber;
        return stmtNode;
    }

    Node *Parser::parseAssignmentStatement()
    {
        // The current token should now be the left-hand-side variable name.

        Node *assignmentNode = new Node(ASSIGN);

        // Enter the variable name into the symbol table
        // if it isn't already in there.
        string variableName = currentToken->text;
        SymtabEntry *variableId = symtab->lookup(toLowerCase(variableName));
        if (variableId == nullptr) variableId = symtab->enter(variableName);

        // The assignment node adopts the variable node as its first child.
        Node *lhsNode  = new Node(VARIABLE);
        lhsNode->text  = variableName;
        lhsNode->entry = variableId;
        assignmentNode->adopt(lhsNode);

        currentToken = scanner->nextToken();  // consume the LHS variable;

        if (currentToken->type == COLON_EQUALS)
        {
            currentToken = scanner->nextToken();  // consume :=
        }
        else syntaxError("Missing :=");

        // The assignment node adopts the expression node as its second child.
        Node *rhsNode = parseExpression();
        assignmentNode->adopt(rhsNode);

        return assignmentNode;
    }

    Node *Parser::parseCompoundStatement()
    {
        Node *compoundNode = new Node(COMPOUND);
        compoundNode->lineNumber = currentToken->lineNumber;

        currentToken = scanner->nextToken();  // consume BEGIN
        parseStatementList(compoundNode, END);

        if (currentToken->type == END)
        {
            currentToken = scanner->nextToken();  // consume END
        }
        else syntaxError("Expecting END");

        return compoundNode;
    }

    void Parser::parseStatementList(Node *parentNode, TokenType terminalType)
    {
        while (   (currentToken->type != terminalType)
                  && (currentToken->type != END_OF_FILE))
        {
            Node *stmtNode = parseStatement();
            if (stmtNode != nullptr) parentNode->adopt(stmtNode);

            // A semicolon separates statements.
            if (currentToken->type == SEMICOLON)
            {
                while (currentToken->type == SEMICOLON)
                {
                    currentToken = scanner->nextToken();  // consume ;
                }
            }
            // Did we see the start of the next statement without
            // having seen a semicolon?
            else if (statementStarters.find(currentToken->type) !=
                     statementStarters.end())
            {
                syntaxError("Missing ;");
            }
        }
    }

    void Parser::parseConstantList(Node *parentNode){
        // we should be at the number list e.g. 9,5,2:
        while (currentToken->type != COLON){
            // create an Integer constant node and adopt it
            parentNode->adopt(parseIntegerConstant());
            if (currentToken->type == COMMA){
                currentToken = scanner->nextToken(); // consume comma
            }
        }
    }

    Node *Parser::parseRepeatStatement()
    {
        // The current token should now be REPEAT.

        // Create a LOOP node.
        Node *loopNode = new Node(LOOP);
        currentToken = scanner->nextToken();  // consume REPEAT

        parseStatementList(loopNode, UNTIL);

        if (currentToken->type == UNTIL)
        {
            // Create a TEST node. It adopts the test expression node.
            Node *testNode = new Node(TEST);
            lineNumber = currentToken->lineNumber;
            testNode->lineNumber = lineNumber;
            currentToken = scanner->nextToken();  // consume UNTIL

            testNode->adopt(parseExpression());

            // The LOOP node adopts the TEST node as its final child.
            loopNode->adopt(testNode);
        }
        else syntaxError("Expecting UNTIL");

        return loopNode;
    }

    Node *Parser::parseWhileStatement() {
        // the current token should now be WHILE

        // create a LOOP node
        Node *loopNode = new Node(LOOP);
        // create a TEST node
        Node *testNode = new Node(TEST);
        // create a NOT node
        Node *notNode = new Node(NodeType::NOT);

        // look at parse tree
        loopNode->adopt(testNode);
        testNode->adopt(notNode);

        currentToken = scanner->nextToken();  // consume WHILE
        notNode->adopt(parseExpression());

        if (currentToken->type != DO){
            syntaxError("Expecting DO");
        }
        currentToken = scanner->nextToken(); // consume DO
        loopNode->adopt(parseStatement());

        return loopNode;
    }

    Node *Parser::parseForStatement() {
        // current token should be at FOR

        // create a COMPOUND node
        Node *compoundNode = new Node(COMPOUND);
        currentToken = scanner->nextToken(); // consume FOR
        // create an ASSIGN node
        Node *assignNode = new Node(ASSIGN);
        // create a LOOP node
        Node *loopNode = new Node(LOOP);
        // create a TEST node
        Node *testNode = new Node(TEST);
        // create a operator node to check for TO or DOWNTO
        Node *relationalNode;
        //compoundNode->adopt(parseAssignmentStatement()); // e.g. k := j
        assignNode->adopt(parseAssignmentStatement());
        lineNumber = currentToken->lineNumber;
        assignNode->lineNumber = lineNumber;

        compoundNode->adopt(assignNode); // left child of compound
        compoundNode->adopt(loopNode); // right child of compound
        loopNode->adopt(testNode);

        if (currentToken->type == TO){
            relationalNode = new Node(GT);
            testNode->adopt(relationalNode);
        }
        else {
            relationalNode = new Node(LT);
            testNode->adopt(relationalNode);
        }
        currentToken = scanner->nextToken(); // consume TO/DOWNTO
        // now we should be at the Integer constant
        // first find the k variable
        // COMPOUND's left child is ASSIGN
        // ASSIGN's left child is k
        printf("%s", compoundNode->children[0]->text.c_str());
        relationalNode->adopt(compoundNode->children[0]->children[0]);
        relationalNode->adopt(parseIntegerConstant());
        return compoundNode;
    }

    Node *Parser::parseCaseStatement() {
        // the current token should be CASE

        // create SELECT node
        Node *selectNode = new Node(SELECT);
        currentToken = scanner->nextToken(); // consume CASE
        // the SELECT node will adopt the expression (e.g. i+1)
        Node *expressionNode = parseExpression();
        selectNode->adopt(expressionNode);

        if (currentToken->type == OF){
            currentToken = scanner->nextToken(); // consume OF
        } else { // throw an error if OF does not exist
            syntaxError("Expecting OF");
        }

        // create BRANCH node
        Node *branchNode = new Node(BRANCH);
        // create a select branch
        Node *selectBranchNode = new Node(SELECT_BRANCH);
        // create a Integer constant node
        Node *integerConstant = new Node(INTEGER_CONSTANT);

        while (currentToken->type != END && currentToken->type != END_OF_FILE){
            selectNode->adopt(branchNode);
            branchNode->adopt(selectBranchNode);
            parseConstantList(selectBranchNode);
            if (currentToken->type == COLON){
                currentToken = scanner->nextToken(); // consume :
            }
            selectBranchNode->adopt(parseStatement());
        }
        return selectNode;
    }

    Node *Parser::parseWriteStatement()
    {
        // The current token should now be WRITE.

        // Create a WRITE node-> It adopts the variable or string node.
        Node *writeNode = new Node(NodeType::WRITE);
        currentToken = scanner->nextToken();  // consume WRITE

        parseWriteArguments(writeNode);
        if (writeNode->children.size() == 0)
        {
            syntaxError("Invalid WRITE statement");
        }

        return writeNode;
    }

    Node *Parser::parseWritelnStatement()
    {
        // The current token should now be WRITELN.

        // Create a WRITELN node. It adopts the variable or string node.
        Node *writelnNode = new Node(NodeType::WRITELN);
        currentToken = scanner->nextToken();  // consume WRITELN

        if (currentToken->type == LPAREN) parseWriteArguments(writelnNode);
        return writelnNode;
    }

    void Parser::parseWriteArguments(Node *node)
    {
        // The current token should now be (

        bool hasArgument = false;

        if (currentToken->type == LPAREN)
        {
            currentToken = scanner->nextToken();  // consume (
        }
        else syntaxError("Missing left parenthesis");

        if (currentToken->type == IDENTIFIER)
        {
            node->adopt(parseVariable());
            hasArgument = true;
        }
        else if (   (currentToken->type == CHARACTER)
                    || (currentToken->type == STRING))
        {
            node->adopt(parseStringConstant());
            hasArgument = true;
        }
        else syntaxError("Invalid WRITE or WRITELN statement");

        // Look for a field width and a count of decimal places.
        if (hasArgument)
        {
            if (currentToken->type == COLON)
            {
                currentToken = scanner->nextToken();  // consume ,

                if (currentToken->type == INTEGER)
                {
                    // Field width
                    node->adopt(parseIntegerConstant());

                    if (currentToken->type == COLON)
                    {
                        currentToken = scanner->nextToken();  // consume ,

                        if (currentToken->type == INTEGER)
                        {
                            // Count of decimal places
                            node->adopt(parseIntegerConstant());
                        }
                        else syntaxError("Invalid count of decimal places");
                    }
                }
                else syntaxError("Invalid field width");
            }
        }

        if (currentToken->type == RPAREN)
        {
            currentToken = scanner->nextToken();  // consume )
        }
        else syntaxError("Missing right parenthesis");
    }

    Node *Parser::parseExpression()
    {
        // The current token should now be an identifier or a number.

        // The expression's root node->
        Node *exprNode = parseSimpleExpression();

        // The current token might now be a relational operator.
        if (relationalOperators.find(currentToken->type) != relationalOperators.end())
        {
            TokenType tokenType = currentToken->type;
            Node *opNode = nullptr;
            switch (tokenType) {
                case TokenType::EQUALS :              opNode = new Node(EQ); break;
                case TokenType::NOT_EQUALS :          opNode = new Node(NE); break;
                case TokenType::LESS_THAN :           opNode = new Node(LT); break;
                case TokenType::GREATER_THAN :        opNode = new Node(GT); break;
                case TokenType::LESS_THAN_EQUALS :    opNode = new Node(LE); break;
                case TokenType::GREATER_THAN_EQUALS : opNode = new Node(GE); break;

                default: syntaxError("Unexpected token");
            }
//            Node *opNode = tokenType == EQUALS    ? new Node(EQ)
//                                                  : tokenType == LESS_THAN ? new Node(LT)
//                                                  : nullptr;

            currentToken = scanner->nextToken();  // consume relational operator

            // The relational operator node adopts the first simple expression
            // node as its first child and the second simple expression node
            // as its second child. Then it becomes the expression's root node.
            if (opNode != nullptr)
            {
                opNode->adopt(exprNode);
                opNode->adopt(parseSimpleExpression());
                exprNode = opNode;
            }
        }

        return exprNode;
    }

    Node *Parser::parseSimpleExpression()
    {
        // The current token should now be an identifier or a number.

        // The simple expression's root node->
        Node *simpExprNode = parseTerm();

        // Keep parsing more terms as long as the current token
        // is a + or - operator.
        while (simpleExpressionOperators.find(currentToken->type) !=
               simpleExpressionOperators.end())
        {
            Node *opNode = nullptr;
            switch (currentToken->type) {
                case TokenType::PLUS :  opNode = new Node(ADD); break;
                case TokenType::MINUS : opNode = new Node(SUBTRACT); break;
                case TokenType::OR :    opNode = new Node(NodeType::OR); break;

                default: syntaxError("Unexpected token");

            }
//            Node *opNode = currentToken->type == PLUS ? new Node(ADD)
//                                                      : new Node(SUBTRACT);

            currentToken = scanner->nextToken();  // consume the operator

            // The add or subtract node adopts the first term node as its
            // first child and the next term node as its second child.
            // Then it becomes the simple expression's root node.
            opNode->adopt(simpExprNode);
            opNode->adopt(parseTerm());
            simpExprNode = opNode;
        }

        return simpExprNode;
    }

    Node *Parser::parseTerm()
    {
        // The current token should now be an identifier or a number.
        // what if the number starts with a + or - ?
        Node *termNode = nullptr;

        if (currentToken->type == PLUS){
            currentToken = scanner->nextToken(); // consume +
            termNode = parseFactor();
        }
        else if (currentToken->type == MINUS){
            currentToken = scanner->nextToken(); // consume -
            termNode = new Node(NEGATE);
            termNode->adopt(parseFactor());
        } else {
            termNode = parseFactor();
        }

        // The term's root node->
        //Node *termNode = parseFactor();

        // Keep parsing more factors as long as the current token
        // is a * or / operator.
        while (termOperators.find(currentToken->type) != termOperators.end())
        {
            Node * opNode = nullptr;
            switch (currentToken->type) {
                case TokenType::STAR  : opNode = new Node(MULTIPLY);           break;
                case TokenType::SLASH : opNode = new Node(DIVIDE);             break;
                case TokenType::DIV   : opNode = new Node(INTEGER_DIVIDE);     break;
                case TokenType::MOD   : opNode = new Node(MODULO);             break;
                case TokenType::AND   : opNode = new Node(NodeType::AND); break;

                default : syntaxError("Unexpected token");
            }
//            Node *opNode = currentToken->type == STAR ? new Node(MULTIPLY)
//                                                      : new Node(DIVIDE);

            currentToken = scanner->nextToken();  // consume the operator

            // The multiply or divide node adopts the first factor node as its
            // as its first child and the next factor node as its second child.
            // Then it becomes the term's root node.
            opNode->adopt(termNode);
            opNode->adopt(parseFactor());
            termNode = opNode;
        }

        return termNode;
    }

    Node *Parser::parseFactor()
    {
        // The current token should now be an identifier or a number or (

        if      (currentToken->type == IDENTIFIER) return parseVariable();
        else if (currentToken->type == INTEGER)    return parseIntegerConstant();
        else if (currentToken->type == REAL)       return parseRealConstant();

        else if (currentToken->type == LPAREN)
        {
            currentToken = scanner->nextToken();  // consume (
            Node *exprNode = parseExpression();

            if (currentToken->type == RPAREN)
            {
                currentToken = scanner->nextToken();  // consume )
            }
            else syntaxError("Expecting )");

            return exprNode;
        }

        else if (currentToken->type == TokenType::NOT){
            Node *notNode = new Node(NodeType::NOT);
            currentToken = scanner->nextToken(); // consume NOT
            notNode->adopt(parseFactor());
            return notNode;
        }

        else syntaxError("Unexpected token");
        return nullptr;
    }

    Node *Parser::parseVariable()
    {
        // The current token should now be an identifier.

        // Has the variable been "declared"?
        string variableName = currentToken->text;
        SymtabEntry *variableId = symtab->lookup(toLowerCase(variableName));
        if (variableId == nullptr) semanticError("Undeclared identifier");

        Node *node  = new Node(VARIABLE);
        node->text  = variableName;
        node->entry = variableId;

        currentToken = scanner->nextToken();  // consume the identifier
        return node;
    }

    Node *Parser::parseIntegerConstant()
    {
        // The current token should now be a number.

        Node *integerNode = new Node(INTEGER_CONSTANT);
        integerNode->value = currentToken->value;

        currentToken = scanner->nextToken();  // consume the number
        return integerNode;
    }

    Node *Parser::parseRealConstant()
    {
        // The current token should now be a number.

        Node *realNode = new Node(REAL_CONSTANT);
        realNode->value = currentToken->value;

        currentToken = scanner->nextToken();  // consume the number
        return realNode;
    }

    Node *Parser::parseStringConstant()
    {
        // The current token should now be string.

        Node *stringNode = new Node(STRING_CONSTANT);
        stringNode->value = currentToken->value;

        currentToken = scanner->nextToken();  // consume the string
        return stringNode;
    }

    void Parser::syntaxError(string message)
    {
        printf("SYNTAX ERROR at line %d: %s at '%s'\n",
               lineNumber, message.c_str(), currentToken->text.c_str());
        errorCount++;

        // Recover by skipping the rest of the statement.
        // Skip to a statement follower token.
        while (statementFollowers.find(currentToken->type) ==
               statementFollowers.end())
        {
            currentToken = scanner->nextToken();
        }
    }

    void Parser::semanticError(string message)
    {
        printf("SEMANTIC ERROR at line %d: %s at '%s'\n",
               lineNumber, message.c_str(), currentToken->text.c_str());
        errorCount++;
    }

}  // namespace frontend
