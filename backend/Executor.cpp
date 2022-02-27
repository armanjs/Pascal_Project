//
// Created by Arman Sadeghi on 2/12/22.
//

/**
 * Executor class for a simple interpreter.
 *
 * (c) 2020 by Ronald Mak
 * Department of Computer Science
 * San Jose State University
 */
#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "../Object.h"
#include "../intermediate/Symtab.h"
#include "../intermediate/Node.h"
#include "Executor.h"

namespace backend {

    using namespace std;
    using namespace intermediate;

    set<NodeType> Executor::singletons;
    set<NodeType> Executor::relationals;

    void Executor::initialize()
    {
        singletons.insert(VARIABLE);
        singletons.insert(INTEGER_CONSTANT);
        singletons.insert(REAL_CONSTANT);
        singletons.insert(STRING_CONSTANT);
        singletons.insert(NEGATE); // negate added
        singletons.insert(NOT); // not added

        relationals.insert(EQ);
        relationals.insert(NE);
        relationals.insert(LT);
        relationals.insert(GT);
        relationals.insert(LE);
        relationals.insert(GE);
        relationals.insert(AND);
        relationals.insert(OR);
    }

    Object Executor::visit(Node *node)
    {
        switch (node->type)
        {
            case PROGRAM :  return visitProgram(node);

            case COMPOUND :
            case ASSIGN :
            case LOOP :
            case IF:
            case SWITCH:
            case WRITE :
            case WRITELN :  return visitStatement(node);

            case TEST:      return visitTest(node);

            default :       return visitExpression(node);
        }
    }

    Object Executor::visitProgram(Node *programNode)
    {
        Node *compoundNode = programNode->children[0];
        return visit(compoundNode);
    }

    Object Executor::visitStatement(Node *statementNode)
    {
        lineNumber = statementNode->lineNumber;

        switch (statementNode->type)
        {
            case COMPOUND :  return visitCompound(statementNode);
            case ASSIGN :    return visitAssign(statementNode);
            case LOOP :      return visitLoop(statementNode);
            case IF :        return visitIf(statementNode); // if added
            case SWITCH :    return visitSwitch(statementNode); // switch added
            case WRITE :     return visitWrite(statementNode);
            case WRITELN :   return visitWriteln(statementNode);

            default :        return Object();
        }
    }

    Object Executor::visitCompound(Node *compoundNode)
    {
        for (Node *statementNode : compoundNode->children) visit(statementNode);

        return Object();
    }

    Object Executor::visitAssign(Node *assignNode)
    {
        Node *lhs = assignNode->children[0];
        Node *rhs = assignNode->children[1];

        // Evaluate the right-hand-side expression;
        double value = visit(rhs).D;

        // Store the value into the variable's symbol table entry.
        string variableName = lhs->text;
        SymtabEntry *variableId = lhs->entry;
        variableId->setValue(value);

        return Object();
    }

    Object Executor::visitLoop(Node *loopNode)
    {
        bool b = false;
        do
        {
            for (Node *node : loopNode->children)
            {
                Object value = visit(node);  // statement or test

                // Evaluate the test condition. Stop looping if true.
                b = (node->type == TEST) && value.B;
                if (b) break;
            }
        } while (!b);

        return Object();
    }

    Object Executor::visitTest(Node *testNode)
    {
        return visit(testNode->children[0]);
    }

    Object Executor::visitIf(Node *ifNode) {

    }

    Object Executor::visitSwitch(Node *switchNode) {

    }

    Object Executor::visitWrite(Node *writeNode)
    {
        printValue(writeNode->children);
        return Object();
    }

    Object Executor::visitWriteln(Node *writelnNode)
    {
        if (writelnNode->children.size() > 0) printValue(writelnNode->children);
        cout << endl;

        return Object();
    }

    void Executor::printValue(vector<Node *> children)
    {
        long fieldWidth    = -1;
        long decimalPlaces = 0;

        // Use any specified field width and count of decimal places.
        if (children.size() > 1)
        {
            fieldWidth = visit(children[1]).L;

            if (children.size() > 2)
            {
                decimalPlaces = visit(children[2]).L;
            }
        }

        // Print the value with a format.
        Node *valueNode = children[0];
        if (valueNode->type == VARIABLE)
        {
            string format = "%";
            if (fieldWidth >= 0)    format += to_string(fieldWidth);
            if (decimalPlaces >= 0) format += "." + to_string(decimalPlaces);
            format += "f";

            double value = visit(valueNode).D;
            printf(format.c_str(), value);
        }
        else  // Node *type STRING_CONSTANT
        {
            string format = "%";
            if (fieldWidth > 0) format += to_string(fieldWidth);
            format += "s";

            string value = visit(valueNode).S;
            printf(format.c_str(), value.c_str());
        }
    }

    Object Executor::visitExpression(Node *expressionNode)
    {
        // Single-operand expressions.
        if (singletons.find(expressionNode->type) != singletons.end())
        {
            switch (expressionNode->type)
            {
                case VARIABLE         : return visitVariable(expressionNode);
                case INTEGER_CONSTANT : return visitIntegerConstant(expressionNode);
                case REAL_CONSTANT    : return visitRealConstant(expressionNode);
                case STRING_CONSTANT  : return visitStringConstant(expressionNode);
                case NOT              : return visitNotNode(expressionNode); // not added
                case NEGATE           : return visitNegateNode(expressionNode); // negate added

                default: return Object();
            }
        }

        // Binary expressions.
        double value1 = visit(expressionNode->children[0]).D;
        double value2 = visit(expressionNode->children[1]).D;

        // Relational expressions.
        if (relationals.find(expressionNode->type) != relationals.end())
        {
            bool value = false;

            switch (expressionNode->type)
            {
                case EQ : value = value1 == value2; break;
                case NE : value = value1 != value2; break;
                case LT : value = value1 <  value2; break;
                case GT : value = value1 >  value2; break;
                case LE : value = value1 <=  value2; break;
                case GE : value = value1 >=  value2; break;

                default : break;
            }

            return Object(value);
        }

        double value = 0.0;

        // Arithmetic expressions.
        switch (expressionNode->type)
        {
            case ADD :      value = value1 + value2; break;
            case SUBTRACT : value = value1 - value2; break;
            case MULTIPLY : value = value1 * value2; break;

            case DIVIDE :
            {
                if (value2 != 0.0) value = value1/value2;
                else
                {
                    runtimeError(expressionNode, "Division by zero");
                    return new Object(0.0);
                }

                break;
            }

            default : break;
        }

        return Object(value);
    }

    Object Executor::visitVariable(Node *variableNode)
    {
        // Obtain the variable's value from its symbol table entry.
        SymtabEntry *variableId = variableNode->entry;
        double value = variableId->getValue();

        return value;
    }

    Object Executor::visitIntegerConstant(Node *integerConstantNode)
    {
        return integerConstantNode->value;
    }

    Object Executor::visitRealConstant(Node *realConstantNode)
    {
        return realConstantNode->value;
    }

    Object Executor::visitStringConstant(Node *stringConstantNode)
    {
        return stringConstantNode->value;
    }

    void Executor::runtimeError(Node *node, string message)
    {
        printf("RUNTIME ERROR at line %d: %s: %s\n",
               lineNumber, message.c_str(), node->text.c_str());
        exit(-2);
    }

    Object Executor::visitNotNode(Node *NotNode) {
//        Node *notNodeChild = NotNode->children[0];
//        Object result = visitExpression(notNodeChild).B;
//        return !result.B;
//        easier way to implement
        bool boolean = visit(NotNode->children[0]).B;
        return !boolean;
    }

    Object Executor::visitNegateNode(Node *NegateNode) {
        double value = visit(NegateNode->children[0]).D;
        return -value;
    }

}  // namespace backend
