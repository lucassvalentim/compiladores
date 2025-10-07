#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <map>

namespace compiler {
    extern std::map<int, std::string> table_converter;
    
    enum class TokenType {
        FUNCTION,
        MAIN,
        LET,
        INT,
        FLOAT,
        CHAR,
        IF,
        ELSE,
        WHILE,
        PRINTLN,
        RETURN,
        LBRACKET,
        RBRACKET,
        LBRACE,
        RBRACE,
        ARROW,
        COLON,
        SEMICOLON,
        COMMA,
        ASSIGN,
        EQ,
        NE,
        GT,
        GE,
        LT,
        LE,
        PLUS,
        MINUS,
        MULT,
        DIV,
        ID,
        INT_CONST,
        FLOAT_CONST,
        CHAR_LITERAL,
        FMT_STRING,
        END_OF_FILE,
        ERROR
    };
    

    struct Token {
        TokenType type;
        std::string lexeme;
        int lineNumber;
    };
}

#endif