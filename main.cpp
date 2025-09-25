#include "include/lexer.hpp"
#include <istream>
#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <vector>

using std::cout;
using std::endl;

#define endl '\n';

int main(int argc, char *argv[]){

    if(argc != 2){
        throw std::runtime_error("the number of arguments is wrong");
    }

    std::ifstream file(argv[1]);
    
    if(!file.is_open()){
        throw std::runtime_error("erro opening file");
    }

    compiler::Lexer lexer(file);
    std::vector<compiler::Token> tokens = lexer.tokenize();    

    for(auto t : tokens){
        cout << "lexema: " << t.lexeme << endl;
        cout << "Type token: " << (int)t.type << endl;
        // cout << "line numb: " << t.lineNumber << endl;
    }

    file.close();

    return 0;
}