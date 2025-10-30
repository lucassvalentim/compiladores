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
    parser.print_vecotr();
    // cout << "index: " << parser.get_index() << '\n';

    parser.initialize_parser();
    file.close();

    return 0;
}