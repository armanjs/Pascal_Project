

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
        PROGRAM, COMPOUND, ASSIGN, LOOP, TEST, WRITE, WRITELN,
        ADD, SUBTRACT, MULTIPLY, DIVIDE, EQ,NE, LT,LE,  GT, GE, NOT,NEGATE, IF,
        VARIABLE, INTEGER_CONSTANT, REAL_CONSTANT, STRING_CONSTANT, AND, OR, DIV_INTEGER, SELECT, SELECT_BRANCH, SELECT_CONSTANTS,
        SWITCH, INTEGER_DIVIDE, MODULO, NOT_NODE
    };

    static const string NODE_TYPE_STRINGS[] =
            {
                    "PROGRAM", "COMPOUND", "ASSIGN", "LOOP", "TEST",  "WRITE", "WRITELN",
                    "ADD", "SUBTRACT", "MULTIPLY", "DIVIDE", "EQ", "NE", "LT", "LE", "GT", "GE", "NOT", "NEGATE","IF"
                    "VARIABLE", "INTEGER_CONSTANT", "REAL_CONSTANT", "STRING_CONSTANT", "AND", "OR", "DIV_INTEGER", "SELECT",
                     "SELECT_BRANCH", "SELECT_CONSTANTS","SWITCH", "INTEGER_DIVIDE", "MODULO", "NOT_NODE"
            };

    constexpr NodeType PROGRAM          = NodeType::PROGRAM;
    constexpr NodeType COMPOUND         = NodeType::COMPOUND;
    constexpr NodeType ASSIGN           = NodeType::ASSIGN;
    constexpr NodeType LOOP             = NodeType::LOOP;
    constexpr NodeType TEST             = NodeType::TEST;
    constexpr NodeType WRITE            = NodeType::WRITE;
    constexpr NodeType WRITELN          = NodeType::WRITELN;
    constexpr NodeType ADD              = NodeType::ADD;
    constexpr NodeType SUBTRACT         = NodeType::SUBTRACT;
    constexpr NodeType MULTIPLY         = NodeType::MULTIPLY;
    constexpr NodeType DIVIDE           = NodeType::DIVIDE; //this one is for slash
    constexpr NodeType EQ               = NodeType::EQ;
    constexpr NodeType NE               = NodeType::NE; //added
    constexpr NodeType LT               = NodeType::LT; //added
    constexpr NodeType LE               = NodeType::LE; //added
    constexpr NodeType GT               = NodeType::GT; //added
    constexpr NodeType GE               = NodeType::GE; //added
    constexpr NodeType NOT              = NodeType::NOT; //added 
    constexpr NodeType NEGATE           = NodeType::NEGATE;//added
    constexpr NodeType IF               = NodeType::IF; //added 
    constexpr NodeType VARIABLE         = NodeType::VARIABLE;
    constexpr NodeType INTEGER_CONSTANT = NodeType::INTEGER_CONSTANT;
    constexpr NodeType REAL_CONSTANT    = NodeType::REAL_CONSTANT;
    constexpr NodeType STRING_CONSTANT  = NodeType::STRING_CONSTANT;
    constexpr NodeType AND              = NodeType::AND; //added 
    constexpr NodeType OR               = NodeType::OR; //added 
    constexpr NodeType DIV_INTEGER      = NodeType::DIV_INTEGER; //added
    constexpr NodeType SELECT           = NodeType::SELECT;
    constexpr NodeType SELECT_BRANCH           = NodeType::SELECT_BRANCH;
    constexpr NodeType SELECT_CONSTANTS           = NodeType::SELECT_CONSTANTS;
     constexpr NodeType SWITCH           = NodeType::SWITCH; 
     constexpr NodeType INTEGER_DIVIDE           = NodeType::INTEGER_DIVIDE; 
     constexpr NodeType MODULO        = NodeType::MODULO; 
      constexpr NodeType NOT_NODE       = NodeType::NOT_NODE; 
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