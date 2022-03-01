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
            // Skip blanks, comments, and other whitespace characters.
            char ch = nextNonblankCharacter();

            if (isalpha(ch))      return Token::Word(ch, source);
            else if (isdigit(ch)) return Token::Number(ch, source);
            else if (ch == '\'')  return Token::String(ch, source);
            else                  return Token::SpecialSymbol(ch, source);
        }

    private:
        /**
         * Skip blanks, comments, and other whitespace characters
         * and return the next nonblank character.
         * @return the next nonblank character.
         */
        char nextNonblankCharacter()
        {
            char ch = source->currentChar();

            while ((ch == '{') || isspace(ch))
            {
                if (ch == '{')
                {
                    // Consume characters of the comment.
                    while ((ch != '}') && (ch != EOF)) ch = source->nextChar();
                }

                ch = source->nextChar();  // consume character
            }

            return ch;  // nonblank character
        }
    };

}  // namespace frontend

#endif //PASCALPROJECT_SCANNER_H
