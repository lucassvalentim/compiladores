#include <bits/stdc++.h>
using namespace std;

enum typeToken{
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
    FMT_STRING
};

struct token{
    typeToken t;
    string lexema;
    int lineNumber;
    int start;
    int end;
};


int main(){

    ifstream arquivo("calculadora.p");
    if(!arquivo.is_open()){
        cout << "nao abriu" << endl;
        exit(1);
    }


    int state = 0;
    char c;
    vector<token> resultado;
    string aux = "";
    regex pattern1("[a-zA-Z]");
    regex pattern2("[a-zA-Z0-9_]");
    regex pattern3("[0-9]");
    regex pattern4("[0-9.]");
    regex alfabeto("[a-zA-Z0-9._(){}:;,+*/]");

    while(arquivo.get(c)){
        string lexema = "";
        lexema += c;

        if(state == 0){
            if(c == '('){
                resultado.push_back({LBRACKET, lexema, 0, 0, 0});
                state = 0;
            }
            if(c == ')'){
                
                resultado.push_back({RBRACKET, lexema, 0, 0, 0});
            }
            if(c == '{'){
                
                resultado.push_back({LBRACE, lexema, 0, 0, 0});
            }
            if(c == '}'){
                
                resultado.push_back({RBRACE, lexema, 0, 0, 0});
            }
            if(c == ':'){
                
                resultado.push_back({COLON, lexema, 0, 0, 0});
            }
            if(c == ';'){
                
                resultado.push_back({SEMICOLON, lexema, 0, 0, 0});
            }
            if(c == ','){
                
                resultado.push_back({COMMA, lexema, 0, 0, 0});
            }
            if(c == '+'){
                
                resultado.push_back({PLUS, lexema, 0, 0, 0});
            }
            if(c == '*'){
                
                resultado.push_back({MULT, lexema, 0, 0, 0});
            }
            if(c == '/'){
                
                resultado.push_back({DIV, lexema, 0, 0, 0});
            }
            if(c == '-'){
                state = 1;
            }
            if(c == '!'){
                state = 2;
            }
            if(c == '='){
                state = 3;
            }
            if(c == '<'){
                state = 4;
            }
            if(c == '>'){
                state = 5;
            }
            if(regex_search(lexema, pattern1)){
                state = 6;
            }
            if(regex_search(lexema, pattern3)){
                state = 7;
            }
            if(lexema == "'"){
                state = 10;
            }
        }else if(state == 1){
            if(c != '>'){
                state = 0;
                arquivo.unget();
            }
            if(c == '>'){
                
                resultado.push_back({ARROW, lexema, 0, 0, 0});
                state = 0;
            }
        }else if(state == 2){
            if(c == '='){
                
                state = 0;
                resultado.push_back({NE, lexema, 0, 0, 0});
            }
        }else if(state == 3){
            if(c == '='){
                
                state = 0;
                resultado.push_back({EQ, lexema, 0, 0, 0});
            }
            if(c != '='){
                state = 0;
                resultado.push_back({ASSIGN, lexema, 0, 0, 0});
            }
        }else if(state == 4){
            if(c == '='){
                
                state = 0;
                resultado.push_back({LE, lexema, 0, 0, 0});
            }
            if(c != '='){
                state = 0;
                resultado.push_back({LT, lexema, 0, 0, 0});
            }
        }else if(state == 5){
            if(c == '='){
                
                state = 0;
                resultado.push_back({GE, lexema, 0, 0, 0});
            }
            if(c != '='){
                state = 0;
                resultado.push_back({GT, lexema, 0, 0, 0});
            }
        }else if(state == 6){
            if(regex_search(lexema, pattern2)){
                aux += lexema;
                resultado.push_back({ID, aux, 0, 0, 0});
                state = 6;
            }else{
                aux = "";
                state = 0;
                arquivo.unget();   
            }
        }else if(state == 7){
            if(!regex_search(lexema, pattern4)){
                aux += lexema;
                resultado.push_back({INT_CONST, aux, 0, 0, 0});
                state = 0;
            }else if(regex_search(lexema, pattern3)){
                state = 7;
                arquivo.unget();   
            }else if(c == '.'){
                aux += lexema;
                state = 8;
            }
        }else if(state == 8){
            if(regex_search(lexema, pattern3)){
                aux += lexema;
                state = 9;
            }
        }else if(state == 9){
            if(!regex_search(lexema, pattern3)){
                resultado.push_back({FLOAT_CONST, aux, 0, 0, 0});
                state = 0;
                arquivo.unget();
                aux = "";   
            }else if(regex_search(lexema, pattern3)){
                aux += lexema;
                state = 9;
            }
        }else if(state == 10){
            if(regex_search(lexema, alfabeto)){
                resultado.push_back({FLOAT_CONST, lexema, 0, 0, 0});
                state = 0;
                arquivo.unget();
                aux = "";   
            }else if(regex_search(lexema, pattern3)){
                aux += lexema;
                state = 9;
            }
        }
        
        
    }
    cout << endl;

    arquivo.close();
    return 0;
}