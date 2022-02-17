//
// Created by Arman Sadeghi on 2/16/22.
//

#ifndef PASCALPROJECT_SCANNER_H
#define PASCALPROJECT_SCANNER_H

#include "Source.h"
#include "Token.h"

namespace frontend {

    using namespace std;

    class Scanner
    {
    private:
        Source *source;

    public:
        /**
         * Constructor.
         * @param source the input source.
         */
        Scanner(Source *source) : source(source) {}

        /**
         * Extract the next token from the source.
         * @return the token.
         */
        Token *nextToken()
        {
            char ch = source->currentChar();

            // Skip blanks and other whitespace characters.
            while (isspace(ch)) ch = source->nextChar();

            if (isalpha(ch))      return Token::Word(ch, source);
            else if (isdigit(ch)) return Token::Number(ch, source);
            else if (ch == '\'')  return Token::String(ch, source);
            else                  return Token::SpecialSymbol(ch, source);
        }
    };

}  // namespace frontend
#endif //PASCALPROJECT_SCANNER_H
