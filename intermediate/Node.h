//
// Created by Arman Sadeghi on 2/16/22.
//

#ifndef PASCALPROJECT_NODE_H
#define PASCALPROJECT_NODE_H

#include <string>
#include <vector>

#include "../Object.h"
#include "SymtabEntry.h"

namespace intermediate {

    using namespace std;

    enum class NodeType
    {
        PROGRAM, COMPOUND, ASSIGN, LOOP, TEST, IF, WRITE, WRITELN,
        SWITCH, SELECT_BRANCH, SELECT_CONSTANTS,
        ADD, SUBTRACT, MULTIPLY, DIVIDE, INTEGER_DIVIDE, MODULO, NEGATE,
        AND, OR, EQ, NE, LT, GT, LE, GE, NOT,   SELECT, BRANCH,
        VARIABLE, INTEGER_CONSTANT, REAL_CONSTANT, STRING_CONSTANT
    };

    static const string NODE_TYPE_STRINGS[] =
            {
                    "PROGRAM", "COMPOUND", "ASSIGN", "LOOP", "TEST", "IF", "WRITE", "WRITELN",
                    "SWITCH", "SELECT_BRANCH", "SELECT_CONSTANTS"
                    "ADD", "SUBTRACT", "MULTIPLY", "DIVIDE", "INTEGER_DIVIDE", "MODULO", "NEGATE",
                    "AND", "OR", "EQ", "NE", "LT", "GT", "LE", "GE", "NOT",    "SELECT", "BRANCH",
                    "VARIABLE", "INTEGER_CONSTANT", "REAL_CONSTANT", "STRING_CONSTANT"
            };

    constexpr NodeType PROGRAM          = NodeType::PROGRAM;
    constexpr NodeType COMPOUND         = NodeType::COMPOUND;
    constexpr NodeType ASSIGN           = NodeType::ASSIGN;
    constexpr NodeType LOOP             = NodeType::LOOP;
    constexpr NodeType TEST             = NodeType::TEST;
    constexpr NodeType IF               = NodeType::IF; // if added
    constexpr NodeType WRITE            = NodeType::WRITE;
    constexpr NodeType WRITELN          = NodeType::WRITELN;
    constexpr NodeType SWITCH           = NodeType::SWITCH; // switch added
    constexpr NodeType SELECT_BRANCH    = NodeType::SELECT_BRANCH;
    constexpr NodeType SELECT_CONSTANTS = NodeType::SELECT_CONSTANTS; // select constants added
    constexpr NodeType ADD              = NodeType::ADD;
    constexpr NodeType SUBTRACT         = NodeType::SUBTRACT;
    constexpr NodeType MULTIPLY         = NodeType::MULTIPLY;
    constexpr NodeType DIVIDE           = NodeType::DIVIDE;
    constexpr NodeType INTEGER_DIVIDE   = NodeType::INTEGER_DIVIDE; // integer divide added
    constexpr NodeType MODULO           = NodeType::MODULO; // mod added
    constexpr NodeType NEGATE           = NodeType::NEGATE; // negate added
    constexpr NodeType AND              = NodeType::AND; // and added
    constexpr NodeType OR               = NodeType::OR; // or added
    constexpr NodeType EQ               = NodeType::EQ;
    constexpr NodeType NE               = NodeType::NE; // Not Equal added
    constexpr NodeType LT               = NodeType::LT;
    constexpr NodeType GT               = NodeType::GT; // Greater Than added
    constexpr NodeType LE               = NodeType::LE; // Less or Equal added
    constexpr NodeType GE               = NodeType::GE; // Greater or Equal added
    constexpr NodeType NOT              = NodeType::NOT; // NOT added
    constexpr NodeType SELECT           = NodeType::SELECT;
    constexpr NodeType BRANCH           = NodeType::BRANCH;
    //constexpr NodeType SELECT_BRANCH    = NodeType::SELECT_BRANCH;
    constexpr NodeType VARIABLE         = NodeType::VARIABLE;
    constexpr NodeType INTEGER_CONSTANT = NodeType::INTEGER_CONSTANT;
    constexpr NodeType REAL_CONSTANT    = NodeType::REAL_CONSTANT;
    constexpr NodeType STRING_CONSTANT  = NodeType::STRING_CONSTANT;

    class Node
    {
    public:
        NodeType type;
        int lineNumber;
        string text;
        SymtabEntry *entry;
        Object value;
        vector<Node *> children;

        /**
         * Constructor
         * @param type node type.
         */
        Node(NodeType type)
                : type(type), lineNumber(0), entry(nullptr) {}

        /**
         * Adopt a child node.
         * @param child the child node.
         */
        void adopt(Node *child) { children.push_back(child); }
    };

}  // namespace intermediate

#endif //PASCALPROJECT_NODE_H
