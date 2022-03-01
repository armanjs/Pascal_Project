//
// Created by Arman Sadeghi on 2/16/22.
//

/**
 * A simple interpreter to demonstrate scanning, parsing, symbol tables,
 * parse trees, and interpreted program execution.
 *
 * (c) 2020 by Ronald Mak
 * Department of Computer Science
 * San Jose State University
 */

#include <string>

#include "frontend/Source.h"
#include "frontend/Scanner.h"
#include "frontend/Parser.h"
#include "frontend/Token.h"
#include "intermediate/ParseTreePrinter.h"
#include "backend/Executor.h"

using namespace std;
using namespace frontend;
using namespace intermediate;
using namespace backend;

void testScanner(Source *source);
void testParser(Scanner *scanner, Symtab *symtab);
void executeProgram(Parser *parser, Symtab *symtab);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: simple -{scan, parse, execute} sourceFileName" << endl;
        exit(-1);
    }

    Token::initialize();
    Parser::initialize();
    Executor::initialize();

    string operation      = argv[1];
    string sourceFileName = argv[2];

    Source *source = new Source(sourceFileName);

    if (operation == "-scan")
    {
        testScanner(source);
    }
    else if (operation == "-parse")
    {
        testParser(new Scanner(source), new Symtab());
    }
    else if (operation == "-execute")
    {
        Symtab *symtab = new Symtab();
        executeProgram(new Parser(new Scanner(source), symtab), symtab);
    }

    return 0;
}

/**
 * Test the scanner.
 * @param source the input source.
 */
void testScanner(Source *source)
{
    cout << "Tokens:" << endl << endl;

    Scanner *scanner = new Scanner(source);  // create the scanner

    // Loop to extract and print each token from the source one at a time.
    for (Token *token = scanner->nextToken();
         token->type != END_OF_FILE;
         token = scanner->nextToken())
    {
        printf("%14s : %s\n",
               TOKEN_TYPE_STRINGS[(int) token->type].c_str(),
               token->text.c_str());
    }
}

/**
 * Test the parser.
 * @param scanner the scanner.
 * @param symtab the symbol table.
 */
void testParser(Scanner *scanner, Symtab *symtab)
{
    Parser *parser = new Parser(scanner, symtab);  // create the parser
    Node *programNode = parser->parseProgram();    // and parse the program
    int errorCount = parser->getErrorCount();

    if (errorCount == 0)
    {
        cout << "Parse tree:" << endl << endl;

        ParseTreePrinter *printer = new ParseTreePrinter();
        printer->print(programNode);
    }
    else
    {
        cout << endl << "There were " << errorCount << " errors." << endl;
    }
}

/**
 * Test the executor.
 * @param parser the parser.
 * @param symtab the symbol table.
 */
void executeProgram(Parser *parser, Symtab *symtab)
{
    Node *programNode = parser->parseProgram();
    int errorCount = parser->getErrorCount();

    if (errorCount == 0)
    {
        Executor *executor = new Executor();
        executor->visit(programNode);
    }
    else
    {
        cout << endl << "There were " << errorCount << " errors." << endl;
    }
}
