//
// Created by Arman Sadeghi on 2/16/22.
//

/**
 * Source class for a simple interpreter.
 *
 * (c) 2020 by Ronald Mak
 * Department of Computer Science
 * San Jose State University
 */

#ifndef PASCALPROJECT_SOURCE_H
#define PASCALPROJECT_SOURCE_H

#include <iostream>
#include <fstream>
#include <string>

namespace frontend {

    using namespace std;

    class Source
    {
    private:
        ifstream source;
        string sourceFileName;
        int  lineNum;      // current source line number
        char currentCh;    // current source character

    public:
        static const char EOL = '\n';

        /**
         * Constructor
         * @param sourceFileName the source file name.
         */
        Source(string sourceFileName) : lineNum(1)
        {
            source.open(sourceFileName);

            if (source.fail())
            {
                cout << "*** ERROR: Failed to open " << sourceFileName << endl;
                exit(-1);
            }

            currentCh = nextChar();  // read the first character of the file
        }

        /**
         * Getter.
         * @return the current source line number.
         */
        int lineNumber() const { return lineNum; }

        /**
         * Getter.
         * @return the current source character.
         */
        char currentChar() const { return currentCh; }

        /**
         * Read and return the next input source character.
         * @return the character, or EOF if at the end of the file.
         */
        char nextChar()
        {
            currentCh = source.get();

            if (source.eof()) currentCh = EOF;
            else if (currentCh == EOL) lineNum++;

            else if (source.fail())
            {
                cout << "*** ERROR: Failed to read " << sourceFileName << endl;
                exit(-1);
            }

            return currentCh;
        }
    };

}  // namespace frontend
#endif //PASCALPROJECT_SOURCE_H
