#include <iostream>
#include <regex>
#include "../include/lexer.hpp"

namespace compiler {

    Lexer::Lexer(std::istream &in) : input(in), line_number(1) {}
    
    std::vector<compiler::Token>  Lexer::tokenize(){
        std::vector<Token> tokens;
        while(true){
            Token t = nextToken();
            tokens.push_back(t);
            if(t.type == TokenType::END_OF_FILE) break;
        }

        return tokens;
    }

    Token Lexer::nextToken(){
        char c;
        std::string lexema;
        while(input.get(c)){
            if(c == ' ') continue;
            if(c == '\n') {line_number++; continue;}

            lexema.push_back(c);
            if(!isalnum(c)){
                // 39 eh o inteiro do caracter
                if(c == 39) {
                    // desconsidera o caracter ' no lexema
                    lexema.pop_back(); 
                    return isChar(lexema);
                }

                if(c == '"'){
                    // desconsidera o caracter " no lexema
                    lexema.pop_back(); 
                    return isString(lexema);
                }
                
                return punctuationOrOperator(lexema);
            }
            
            if(isalpha(c)){
                return identifierOrKeyword(lexema);
            }

            if(isdigit(c)){
                return floatOrInteger(lexema);
            }

        }

        return makeToken(TokenType:: END_OF_FILE, "EOF");
    }
    
    Token Lexer::punctuationOrOperator(std::string& lexema){
        if(lexema == "("){
            return makeToken(TokenType:: LBRACKET, lexema);    
        }
        if(lexema == ")"){
            return makeToken(TokenType:: RBRACKET, lexema);
        }
        if(lexema == "{"){
            return makeToken(TokenType:: LBRACE, lexema);
        }
        if(lexema == "}"){
            return makeToken(TokenType:: RBRACE, lexema);
        }
        if(lexema == ":"){
            return makeToken(TokenType:: COLON, lexema);
        }
        if(lexema == ";"){
            return makeToken(TokenType:: SEMICOLON, lexema);
        }
        if(lexema == ","){
            return makeToken(TokenType:: COMMA, lexema);
        }
        if(lexema == "+"){
            return makeToken(TokenType:: PLUS, lexema);
        }
        if(lexema == "*"){
            return makeToken(TokenType:: MULT, lexema);
        }
        if(lexema == "/"){
            return makeToken(TokenType:: DIV, lexema);
        }

        if(lexema == "-"){
            char c; input.get(c); lexema.push_back(c);
            if(lexema == "->"){
                return makeToken(TokenType:: ARROW, lexema);
            }else{
                lexema.pop_back();
                input.unget();
                return makeToken(TokenType:: MINUS, lexema);
            }
            
        }
        if(lexema == "!"){
            char c; input.get(c); lexema.push_back(c);
            if(lexema == "!="){
                return makeToken(TokenType:: NE, lexema);
            }else{
                lexema.pop_back();
                input.unget();
                return makeToken(TokenType:: ERROR, "read ! and missing =");
            }
            
        }
        if(lexema == "="){
            char c; input.get(c); lexema.push_back(c);
            if(lexema == "=="){
                return makeToken(TokenType:: EQ, lexema);
            }else{
                lexema.pop_back();
                input.unget();
                return makeToken(TokenType:: ASSIGN, lexema);
            }
                
        }
        if(lexema == "<"){
            char c; input.get(c); lexema.push_back(c);
            if(lexema == "<="){
                return makeToken(TokenType:: LE, lexema);
            }else{
                lexema.pop_back();
                input.unget();
                return makeToken(TokenType:: LT, lexema);
            }
                
        }
        if(lexema == ">"){
            char c; input.get(c); lexema.push_back(c);
            if(lexema == ">="){
                return makeToken(TokenType:: GE, lexema);
            }else{
                lexema.pop_back();
                input.unget();
                return makeToken(TokenType:: GT, lexema);
            }
                
        }

        return makeToken(TokenType:: ERROR, "ERROR in punctuation or Operator");
    }

    Token Lexer::identifierOrKeyword(std::string& lexema){
        char c;
        while(input.get(c) && (isalnum(c) || c == '_')){
            lexema.push_back(c);
        }

        input.unget();
        // block checks if it is a reserved word
        if(lexema == "fn"){
            return makeToken(TokenType:: FUNCTION, lexema);
        }
        if(lexema == "main"){
            return makeToken(TokenType:: MAIN, lexema);
        }
        if(lexema == "let"){
            return makeToken(TokenType:: LET, lexema);
        }
        if(lexema == "int"){
            return makeToken(TokenType:: INT, lexema);
        }
        if(lexema == "float"){
            return makeToken(TokenType:: FLOAT, lexema);
        }
        if(lexema == "char"){
            return makeToken(TokenType:: CHAR, lexema);
        }
        if(lexema == "if"){
            return makeToken(TokenType:: IF, lexema);
        }
        if(lexema == "else"){
            return makeToken(TokenType:: ELSE, lexema);
        }
        if(lexema == "while"){
            return makeToken(TokenType:: WHILE, lexema);
        }
        if(lexema == "println"){
            return makeToken(TokenType:: PRINTLN, lexema);
        }
        if(lexema == "return"){
            return makeToken(TokenType:: RETURN, lexema);
        }

        return makeToken(TokenType:: ID, lexema);

    }

    Token Lexer::floatOrInteger(std::string &lexema){
        char c;
        while(input.get(c) && isdigit(c)){
            lexema.push_back(c);
        }

        if(c == '.'){
            lexema.push_back(c); input.get(c);
            if(isdigit(c)){
                do{
                    lexema.push_back(c);
                }while(input.get(c) && isdigit(c));
                input.unget();
                return makeToken(TokenType::FLOAT_CONST, lexema);
            }else{
                return makeToken(TokenType::ERROR, "Float erro");
            }
        }
    
        input.unget();
        return makeToken(TokenType::INT_CONST, lexema);
    }

    Token Lexer::isChar(std::string &lexema){
        std::regex alfabeto("[a-zA-Z0-9._(){}:;,!><+\\-*/='\\\" ]");

        char c; input.get(c); lexema.push_back(c);
        if(std::regex_search(lexema.substr(lexema.size()-1, 1), alfabeto)){
            input.get(c);
            if(c == 39){
                return makeToken(TokenType::CHAR_LITERAL, lexema);
            }else{
                input.unget();
                return makeToken(TokenType::ERROR, "char error");
            }
        }

        input.unget();
        return makeToken(TokenType::ERROR, "char error");
    }

    Token Lexer::isString(std::string &lexema){
        std::regex alfabeto("[a-zA-Z0-9._(){}%:;,!><+\\-*/='\\\" ]+");

        char c;
        while(input.get(c) && c != '"'){
            lexema.push_back(c);
            if(!std::regex_search(lexema.substr(lexema.size()-1, 1), alfabeto)){lexema.pop_back(); break;}
        }

        if(c == '"'){
            return makeToken(TokenType:: FMT_STRING, lexema);
        }else{
            input.unget();
            return makeToken(TokenType:: ERROR, "string error");
        }
    }

    Token Lexer::makeToken(TokenType type, std::string lexema){
        return Token{type, lexema, line_number};
    }

}