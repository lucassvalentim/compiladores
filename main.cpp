#include "include/lexer.hpp"
#include "include/parser.hpp"
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

    compiler::Parser parser(tokens);
    std::cout << "-> Inicio da Analise Lexica:\n\n";
    for(compiler::Token ti : tokens){
            std::cout << "lexema: " << '"' << ti.lexeme << '"' << '\n';
            std::cout << "Token Type: " << compiler::table_converter[(int)ti.type] << '\n';
            std::cout << "Line Number: " << ti.lineNumber << '\n';
            std::cout << '\n';
        }
    std::cout << "Fim da Analise Lexica\n\n";
    
    std::cout << "-> Inicio da Analise Sintatica:\n\n";
    parser.initialize_parser();

    file.close();
    return 0;
}