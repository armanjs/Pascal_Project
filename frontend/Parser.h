//
// Created by Arman Sadeghi on 2/16/22.
//

#ifndef PASCALPROJECT_PARSER_H
#define PASCALPROJECT_PARSER_H

#include <set>

#include "Scanner.h"
#include "Token.h"
#include "../intermediate/Symtab.h"
#include "../intermediate/Node.h"

namespace frontend {

    using namespace std;
    using namespace intermediate;

    class Parser
    {
    private:
        Scanner *scanner;
        Symtab *symtab;
        Token *currentToken;
        int lineNumber;
        int errorCount;

        static set<TokenType> statementStarters;          // what starts a statement
        static set<TokenType> statementFollowers;         // what follows a statement
        static set<TokenType> relationalOperators;        // relational operators
        static set<TokenType> simpleExpressionOperators;  // simple expression operators
        static set<TokenType> termOperators;              // term operators

    public:
        /**
         * Initialize the static sets.
         */
        static void initialize();

        Parser(Scanner *scanner, Symtab *symtab)
                : scanner(scanner), symtab(symtab), currentToken(nullptr),
                  lineNumber(1), errorCount(0) {}

        int getErrorCount() const { return errorCount; }

        Node *parseProgram();

    private:
        Node *parseStatement();
        Node *parseAssignmentStatement();
        Node *parseCompoundStatement();
        Node *parseRepeatStatement();
        Node *parseWriteStatement();
        Node *parseWritelnStatement();
        Node *parseExpression();
        Node *parseSimpleExpression();
        Node *parseTerm();
        Node *parseFactor();
        Node *parseVariable();
        Node *parseIntegerConstant();
        Node *parseRealConstant();
        Node *parseStringConstant();
        Node *parseWhileStatement();// while added
        Node *parseForStatement(); // for added
        Node *parseIfStatement(); // if added
        Node *parseCaseStatement(); // case added

        void parseStatementList(Node *parentNode, TokenType terminalType);
        void parseWriteArguments(Node *node);

        void syntaxError(string message);
        void semanticError(string message);

        //void parseConstantList(Node *parentNode);
    };

}  // namespace frontend
#endif //PASCALPROJECT_PARSER_H
