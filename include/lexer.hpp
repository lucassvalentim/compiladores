#ifndef LEXER_HPP
#define LEXER_HPP

#include "token.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <regex>

namespace compiler{
    class Lexer {
        public:
            Lexer(std::istream &in);
            
            std::vector<Token> tokenize();
            
        private:
            std::istream &input;
            int line_number;

            Token nextToken();
            Token punctuationOrOperator(std::string &lexema);
            Token identifierOrKeyword(std::string &lexema);
            Token floatOrInteger(std::string &lexema);
            Token isChar(std::string &lexema);
            Token isString(std::string &lexema);
            Token makeToken(TokenType type, std::string lexema, int start, int end);
    };
}

#endif