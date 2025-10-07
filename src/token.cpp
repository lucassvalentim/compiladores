#include "../include/token.hpp"
#include <string>
#include <map>

namespace compiler {
    
    std::map<int, std::string> table_converter = {
        {0 ,"FUNCTION"},
        {1 ,"MAIN"},
        {2 ,"LET"},
        {3 ,"INT"},
        {4 ,"FLOAT"},
        {5 ,"CHAR"},
        {6 ,"IF"},
        {7 ,"ELSE"},
        {8 ,"WHILE"},
        {9 ,"PRINTLN"},
        {10 ,"RETURN"},
        {11 ,"LBRACKET"},
        {12 ,"RBRACKET"},
        {13 ,"LBRACE"},
        {14 ,"RBRACE"},
        {15 ,"ARROW"},
        {16 ,"COLON"},
        {17 ,"SEMICOLON"},
        {18 ,"COMMA"},
        {19 ,"ASSIGN"},
        {20 ,"EQ"},
        {21 ,"NE"},
        {22 ,"GT"},
        {23 ,"GE"},
        {24 ,"LT"},
        {25 ,"LE"},
        {26 ,"PLUS"},
        {27 ,"MINUS"},
        {28 ,"MULT"},
        {29 ,"DIV"},
        {30 ,"ID"},
        {31 ,"INT_CONST"},
        {32 ,"FLOAT_CONST"},
        {33 ,"CHAR_LITERAL"},
        {34 ,"FMT_STRING"},
        {35 ,"END_OF_FILE"},
        {36 ,"ERROR"}};

}