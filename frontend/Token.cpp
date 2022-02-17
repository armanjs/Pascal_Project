//
// Created by Arman Sadeghi on 2/12/22.
//

/**
 * Token class for a simple interpreter.
 *
 * (c) 2020 by Ronald Mak
 * Department of Computer Science
 * San Jose State University
 */
#include <string>
#include <map>
#include <ctype.h>

#include "../Object.h"
#include "Token.h"

namespace frontend {

    using namespace std;

    map<string, TokenType> Token::reservedWords;

    void Token::initialize()
    {
        reservedWords["PROGRAM"] = PROGRAM;
        reservedWords["BEGIN"]   = BEGIN;
        reservedWords["END"]     = END;
        reservedWords["REPEAT"]  = REPEAT;
        reservedWords["UNTIL"]   = UNTIL;
        reservedWords["WRITE"]   = WRITE;
        reservedWords["WRITELN"] = WRITELN;
        reservedWords["DIV"]     = DIV; // DIV added
        reservedWords["MOD"]     = MOD; // MOD added
        reservedWords["AND"]     = AND; // AND added
        reservedWords["OR"]      = OR; // OR added
        reservedWords["NOT"]     = NOT; // NOT added
        reservedWords["CONST"]   = CONST; // CONST added
        reservedWords["TYPE"]    = TYPE; // TYPE added
        reservedWords["VAR"]     = VAR; // VAR added
        reservedWords["PROCEDURE"] = PROCEDURE; // PROCEDURE added
        reservedWords["FUNCTION"] = FUNCTION; // FUNCTION added
        reservedWords["WHILE"]   = WHILE; // WHILE added
        reservedWords["DO"]      = DO; // DO added
        reservedWords["FOR"]     = FOR; // FOR added
        reservedWords["TO"]      = TO; // TO added
        reservedWords["DOWNTO"]  = DOWNTO; // DOWNTO added
        reservedWords["IF"]      = IF; // IF added
        reservedWords["THEN"]    = THEN; // THEN added
        reservedWords["ELSE"]    = ELSE; // ELSE added
        reservedWords["CASE"]    = CASE; // CASE added
        reservedWords["OF"]      = OF; // OF added

    }

    Token *Token::Word(char firstChar, Source *source)
    {

        Token *token = new Token(firstChar);
        token->lineNumber = source->lineNumber();

        // Loop to get the rest of the characters of the word token.
        // Append letters and digits to the token.
        for (char ch = source->nextChar(); isalnum(ch); ch = source->nextChar())
        {
            token->text += ch;
        }

        // Is it a reserved word or an identifier?
        string upper = toUpperCase(token->text);
        if (Token::reservedWords.find(upper) != Token::reservedWords.end())
        {
            token->type = Token::reservedWords[upper];
        }
        else
        {
            token->type = TokenType::IDENTIFIER;
        }

        return token;
    }

    Token *Token::Number(char firstChar, Source *source)
    {
        Token *token = new Token(firstChar);
        int pointCount = 0;

        // Loop to get the rest of the characters of the number token.
        // Append digits to the token.
        for (char ch = source->nextChar();
             isdigit(ch) || (ch == '.');
             ch = source->nextChar())
        {
            if (ch == '.') pointCount++;
            token->text += ch;
        }

        // Integer constant.
        if (pointCount == 0)
        {
            token->type    = TokenType::INTEGER;
            token->value.L = stol(token->text);
            token->value.D = token->value.L;  // allow using integer value as double
        }

            // Real constant.
        else if (pointCount == 1)
        {
            token->type    = TokenType::REAL;
            token->value.D = stod(token->text);
        }

        else {
            token->lineNumber = source->lineNumber();
            tokenError(token, "Invalid number");
        }

        return token;
    }

    Token *Token::String(char firstChar, Source *source)
    {
        Token *token = new Token(firstChar);  // the leading '
        token->lineNumber = source->lineNumber();

        for (char ch = source->nextChar(); ch != '\''; ch = source->nextChar())
        {
            token->text += ch;
            if(ch == EOF){
                if (token->text.size () > 0)
                    token->text.resize (token->text.size () - 1);
                token->type = TokenType::STRING;
                tokenError(token, "String not closed");
                return token;
            }
        }
//        bool nextLine;
//        char ch = source->currentChar();
//        if (ch == '\n'){
//            ch = source->nextChar();
//            if (isalpha(ch))
//                nextLine = true;
//        }
//
//        for (char chOuter = source->nextChar(); chOuter != '\n' && nextLine; chOuter = source->nextChar()){
//            token->text += chOuter;
//            if(ch == EOF){
//                if (token->text.size () > 0)
//                    token->text.resize (token->text.size () - 1);
//                token->type = TokenType::STRING;
//                tokenError(token, "String not closed");
//                return token;
//            }
//        }

        token->text += '\'';  // the closing quote
        source->nextChar();  // consume the closing quote

        if (token->text.length() > 3 || token->text.length() == 2){
            token->type = TokenType::STRING;
        }
        else token->type = TokenType::CHARACTER;

        // Don't include the leading and trailing '.
        token->value.S = token->text.substr(1, token->text.length() - 2);

        return token;
    }

    Token *Token::SpecialSymbol(char firstChar, Source *source)
    {
        Token *token = new Token(firstChar);

        switch (firstChar)
        {
            case '.' :
            {
                char nextChar = source->nextChar();

                // Is it the .. symbol?
                if (nextChar == '.') { // .. added
                    token->text += nextChar;
                    token->type = TokenType::DOT_DOT;
                }
                    // No, it's just the . symbol
                else {
                    token->type = TokenType::PERIOD;
                    return token;  // already consumed .
                }
                break;
            }
            case ',' : token->type = TokenType::COMMA;      break;
            case ';' : token->type = TokenType::SEMICOLON;  break;
            case '+' : token->type = TokenType::PLUS;       break;
            case '-' : token->type = TokenType::MINUS;      break;
            case '*' : token->type = TokenType::STAR;       break;
            case '/' : token->type = TokenType::SLASH;      break;
            case '(' : token->type = TokenType::LPAREN;     break;
            case ')' : token->type = TokenType::RPAREN;     break;
            case '=' : token->type = TokenType::EQUALS;     break;
                //case '\'': token->type = TokenType::APOSTROPHE; break; // ' added
            case '[' : token->type = TokenType::LBRACKET;   break; // [ added
            case ']' : token->type = TokenType::RBRACKET;   break; // ] added
            case '^' : token->type = TokenType::CIRCUMFLEX; break; // ^ added

            case '<' : {
                char nextChar = source->nextChar();
                token->text += nextChar;

                // Is it the >= symbol
                if (nextChar == '=') {
                    token->type = TokenType::LESS_THAN_EQUALS; // < = added
                }
                    // is it the <> symbol
                else if (nextChar == '>') {
                    token->type = TokenType::NOT_EQUALS; // <> added
                }
                    // No its just the > symbol
                else {
                    token->type = TokenType::LESS_THAN; // < added
                    return token; // already consumed <
                }
                break;
            }

            case '>' :
            {
                char nextChar = source->nextChar();
                token->text += nextChar;

                // Is it the >= symbol
                if (nextChar == '=') {
                    token->type = TokenType::GREATER_THAN_EQUALS; // > = added
                }
                    // No its just the > symbol
                else {
                    token->type = TokenType::GREATER_THAN; // > added
                    return token; // already consumed >
                }
                break;
            }

            case ':' :
            {
                char nextChar = source->nextChar();
                token->text += nextChar;

                // Is it the := symbol?
                if (nextChar == '=')
                {
                    token->type = TokenType::COLON_EQUALS;
                }

                    // No, it's just the : symbol.
                else
                {
                    token->type = TokenType::COLON;
                    return token;  // already consumed :
                }

                break;
            }
            case '{':
            {
                token->type = TokenType::COMMENT;
                while (true) { // continue doing
                    char nextChar = source->nextChar();
                    if (nextChar == '}'){ // until } is found
                        break;}
                }
                //delete token; // destructor
                break;
            }
            case '?':
            {
                token->type = TokenType::ERROR;
                token->lineNumber = source->lineNumber();
                tokenError(token, "Invalid token");
                break;

            }
            case '}':
            {
                token->type = TokenType::ERROR;
                token->lineNumber = source->lineNumber();
                tokenError(token, "Invalid token");
                break;
            }


            case EOF : token->type = END_OF_FILE; break;

            default: token->type = TokenType::ERROR;
        }

        source->nextChar();  // consume the special symbol
        return token;
    }

    void Token::tokenError(Token *token, string message)
    {
        printf("TOKEN ERROR at line %d: %s at '%s'\n",
               token->lineNumber, message.c_str(), token->text.c_str());
    }

}  // namespace frontend
