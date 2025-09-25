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
            if(c == ' ' || c == '\n') continue;

            lexema.push_back(c);
            if(!isalnum(c)){
                // if c == ' (39 eh o inteiro do caracter ')
                if(c == 39) {
                    return isChar(lexema);
                }

                if(c == '"'){
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

        return makeToken(TokenType:: END_OF_FILE, "terminou", 0, 0);
    }
    
    Token Lexer::punctuationOrOperator(std::string& lexema){
        if(lexema == "("){
            return makeToken(TokenType:: LBRACKET, lexema, 0, 0);    
        }
        if(lexema == ")"){
            return makeToken(TokenType:: RBRACKET, lexema, 0, 0);
        }
        if(lexema == "{"){
            return makeToken(TokenType:: LBRACE, lexema, 0, 0);
        }
        if(lexema == "}"){
            return makeToken(TokenType:: RBRACE, lexema, 0, 0);
        }
        if(lexema == ":"){
            return makeToken(TokenType:: COLON, lexema, 0, 0);
        }
        if(lexema == ";"){
            return makeToken(TokenType:: SEMICOLON, lexema, 0, 0);
        }
        if(lexema == ","){
            return makeToken(TokenType:: COMMA, lexema, 0, 0);
        }
        if(lexema == "+"){
            return makeToken(TokenType:: PLUS, lexema, 0, 0);
        }
        if(lexema == "*"){
            return makeToken(TokenType:: MULT, lexema, 0, 0);
        }
        if(lexema == "/"){
            return makeToken(TokenType:: DIV, lexema, 0, 0);
        }

        if(lexema == "-"){
            char c; input.get(c); lexema.push_back(c);
            if(lexema == "->"){
                return makeToken(TokenType:: ARROW, lexema, 0, 0);
            }else{
                lexema.pop_back();
                input.unget();
                return makeToken(TokenType:: MINUS, lexema, 0, 0);
            }
            
        }
        if(lexema == "!"){
            char c; input.get(c); lexema.push_back(c);
            if(lexema == "!="){
                return makeToken(TokenType:: NE, lexema, 0, 0);
            }else{
                lexema.pop_back();
                input.unget();
                return makeToken(TokenType:: END_OF_FILE, "erro", 0, 0);
            }
            
        }
        if(lexema == "="){
            char c; input.get(c); lexema.push_back(c);
            if(lexema == "=="){
                return makeToken(TokenType:: EQ, lexema, 0, 0);
            }else{
                lexema.pop_back();
                input.unget();
                return makeToken(TokenType:: ASSIGN, lexema, 0, 0);
            }
                
        }
        if(lexema == "<"){
            char c; input.get(c); lexema.push_back(c);
            if(lexema == "<="){
                return makeToken(TokenType:: LE, lexema, 0, 0);
            }else{
                lexema.pop_back();
                input.unget();
                return makeToken(TokenType:: LT, lexema, 0, 0);
            }
                
        }
        if(lexema == ">"){
            char c; input.get(c); lexema.push_back(c);
            if(lexema == ">="){
                return makeToken(TokenType:: GE, lexema, 0, 0);
            }else{
                lexema.pop_back();
                input.unget();
                return makeToken(TokenType:: GT, lexema, 0, 0);
            }
                
        }

        return makeToken(TokenType:: END_OF_FILE, "erro", 0, 0);
    }

    Token Lexer::identifierOrKeyword(std::string& lexema){
        char c;
        while(input.get(c) && (isalnum(c) || c == '_')){
            lexema.push_back(c);
        }

        input.unget();
        // block checks if it is a reserved word
        if(lexema == "fn"){
            return makeToken(TokenType:: FUNCTION, lexema, 0, 0);
        }
        if(lexema == "main"){
            return makeToken(TokenType:: MAIN, lexema, 0, 0);
        }
        if(lexema == "let"){
            return makeToken(TokenType:: LET, lexema, 0, 0);
        }
        if(lexema == "int"){
            return makeToken(TokenType:: INT, lexema, 0, 0);
        }
        if(lexema == "float"){
            return makeToken(TokenType:: FLOAT, lexema, 0, 0);
        }
        if(lexema == "char"){
            return makeToken(TokenType:: CHAR, lexema, 0, 0);
        }
        if(lexema == "if"){
            return makeToken(TokenType:: IF, lexema, 0, 0);
        }
        if(lexema == "else"){
            return makeToken(TokenType:: ELSE, lexema, 0, 0);
        }
        if(lexema == "while"){
            return makeToken(TokenType:: WHILE, lexema, 0, 0);
        }
        if(lexema == "println"){
            return makeToken(TokenType:: PRINTLN, lexema, 0, 0);
        }
        if(lexema == "return"){
            return makeToken(TokenType:: RETURN, lexema, 0, 0);
        }

        return makeToken(TokenType:: ID, lexema, 0 , 0);

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
                return makeToken(TokenType::FLOAT_CONST, lexema, 0, 0);
            }else{
                return makeToken(TokenType::END_OF_FILE, "erro no float", 0, 0);
            }
        }
    
        input.unget();
        return makeToken(TokenType::INT_CONST, lexema, 0, 0);
    }

    Token Lexer::isChar(std::string &lexema){
        std::regex alfabeto("[a-zA-Z0-9._(){}:;,!><+\\-*/='\\\"]");

        char c; input.get(c); lexema.push_back(c);
        if(std::regex_search(lexema.substr(lexema.size()-1, 1), alfabeto)){
            input.get(c); lexema.push_back(c);
            if(c == 39){
                return makeToken(TokenType::CHAR_LITERAL, lexema, 0 , 0);
            }else{
                input.unget();
                return makeToken(TokenType::END_OF_FILE, "erro na leitura do char", 0 , 0);
            }
        }

        input.unget();
        return makeToken(TokenType::END_OF_FILE, "erro na leitura do char", 0 , 0);
    }

    Token Lexer::isString(std::string &lexema){
        std::regex alfabeto("[a-zA-Z0-9._(){}%:;,!><+\\-*/='\\\" ]+");

        char c;
        while(input.get(c) && c != '"'){
            lexema.push_back(c);
            if(!std::regex_search(lexema.substr(lexema.size()-1, 1), alfabeto)){lexema.pop_back(); break;}
        }

        if(c == '"'){
            lexema.push_back(c);
            return makeToken(TokenType:: FMT_STRING, lexema, 0, 0);
        }else{
            input.unget();
            return makeToken(TokenType:: END_OF_FILE, "erro a ler a string", 0, 0);
        }
    }

    Token Lexer::makeToken(TokenType type, std::string lexema, int start, int end){
        return Token{type, lexema, line_number, start, end};
    }

}