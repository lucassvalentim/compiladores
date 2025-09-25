#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

namespace compiler {
    
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
        END_OF_FILE
    };

    struct Token {
        TokenType type;
        std::string lexeme;
        int lineNumber;
        int start;
        int end;
    };
}

#endif