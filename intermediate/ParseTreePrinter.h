//
// Created by Arman Sadeghi on 2/16/22.
//

#ifndef PASCALPROJECT_PARSETREEPRINTER_H
#define PASCALPROJECT_PARSETREEPRINTER_H

#include <string>
#include <vector>

#include "Node.h"

namespace intermediate {

    using namespace std;

    class ParseTreePrinter
    {
    private:
        static const string INDENT_SIZE;

        string indentation;  // indentation of a line
        string line;         // output line

    public:
        ParseTreePrinter() : indentation(""), line("") {}

        /**
         * Print a parse tree.
         * @param node the parse tree's root node.
         */
        void print(Node *node);

    private:
        /**
         * Print a parse tree node's child nodes.
         * @param children the array list of child nodes.
         */
        void printChildren(vector<Node *> children);

        /**
         * Print an output line.
         */
        void printLine();
    };

}  // namespace intermediate

#endif //PASCALPROJECT_PARSETREEPRINTER_H
