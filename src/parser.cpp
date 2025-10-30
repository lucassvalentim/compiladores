#include "../include/parser.hpp"
#include "../include/token.hpp"
#include <iostream>

namespace compiler {
    Parser::Parser(std::vector<compiler::Token> &t) : tokens(t), index(0), error(false) {}

    // Inicializador do Parser
    void Parser::initialize_parser(){
        this->programa();
    }
    // Função que percorre o vetor de tokens
    void Parser::match(compiler::TokenType tok){
        if(tok == this->tokens[this->index].type){
            std::cout << "Token: " << compiler::table_converter[(int)tok] << " reconhecido na entrada\n";
            if(this->index + 1 < this->tokens.size())
            this->index++;
        }else{
            std::cout << "Erro sintático. Token: " << compiler::table_converter[(int)tok] << " nao esperado na entrada.\n";
            this->error = true;
            if(this->index + 1 < this->tokens.size())
            this->index++;
        }
    }
    // Descida recursiva
    void Parser::programa(){
        if(this->tokens[this->index].type == compiler::TokenType::FUNCTION){
            this->funcao();
            this->funcao_seq();
            if(this->tokens[this->index].type == compiler::TokenType::END_OF_FILE){
                match(compiler::TokenType::END_OF_FILE);
                std::cout << "Fim da analise sintatica\n";
            }
        }else if(!this->error){
            std::cout << "1 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::funcao_seq(){
        if(this->tokens[this->index].type == compiler::TokenType::FUNCTION){
            this->funcao();
            this->funcao_seq();
        }
    }
    void Parser::funcao(){
        if(this->tokens[this->index].type == compiler::TokenType::FUNCTION){
            this->match(compiler::TokenType::FUNCTION);
            this->nome_funcao();
            this->match(compiler::TokenType::LBRACKET);
            this->lista_params();
            this->match(compiler::TokenType::RBRACKET);
            this->tipo_retorno_funcao();
            this->bloco();
        }else if(!this->error){
            std::cout << "2 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::nome_funcao(){
        if(this->tokens[this->index].type == compiler::TokenType::ID){
            this->match(compiler::TokenType::ID);
        }else if(this->tokens[this->index].type == compiler::TokenType::MAIN){
            this->match(compiler::TokenType::MAIN);
        }else if(!this->error){
            std::cout << "3 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::lista_params(){
        if(this->tokens[this->index].type == compiler::TokenType::ID){
            this->match(compiler::TokenType::ID);
            this->match(compiler::TokenType::COLON);
            this->type();
            this->lista_params2();
        }
    }
    void Parser::lista_params2(){
        if(this->tokens[this->index].type == compiler::TokenType::COMMA){
            this->match(compiler::TokenType::COMMA);
            this->match(compiler::TokenType::ID);   
            this->match(compiler::TokenType::COLON);
            this->type();
            this->lista_params2();
        }
    }
    void Parser::tipo_retorno_funcao(){
        if(this->tokens[this->index].type == compiler::TokenType::ARROW){
            this->match(compiler::TokenType::ARROW);
            this->type();
        }
    }
    void Parser::bloco(){
        if(this->tokens[this->index].type == compiler::TokenType::LBRACE){
            this->match(compiler::TokenType::LBRACE);
            this->sequencia();
            this->match(compiler::TokenType::RBRACE);
        }else if(!this->error){
            std::cout << "4 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::sequencia(){
        if(this->tokens[this->index].type == compiler::TokenType::LET){
            this->declaracao();
            this->sequencia();
        }else if(this->tokens[this->index].type == compiler::TokenType::ID){
            this->comando();
            this->sequencia();
        }
    }
    void Parser::declaracao(){
        if(this->tokens[this->index].type == compiler::TokenType::LET){
            this->match(compiler::TokenType::LET);
            this->var_list();
            this->match(compiler::TokenType::COLON);
            this->type();
            this->match(compiler::TokenType::SEMICOLON); 
        }else if(!this->error){
            std::cout << "5 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::var_list(){
        if(this->tokens[this->index].type == compiler::TokenType::ID){
            this->match(compiler::TokenType::ID);
            this->var_list2();
        }else if(!this->error){
            std::cout << "6 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::var_list2(){
        if(this->tokens[this->index].type == compiler::TokenType::COMMA){
            this->match(compiler::TokenType::COMMA);
            this->match(compiler::TokenType::ID);
            this->var_list2();
        }
    }
    void Parser::type(){
        if(this->tokens[this->index].type == compiler::TokenType::INT){
            this->match(compiler::TokenType::INT);
        }else if(this->tokens[this->index].type == compiler::TokenType::FLOAT){
            this->match(compiler::TokenType::FLOAT);
        }else if(this->tokens[this->index].type == compiler::TokenType::CHAR){
            this->match(compiler::TokenType::CHAR);
        }else if(!this->error){
            std::cout << "7 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }// até o comando estava funcionando.
    void Parser::comando(){
        if(this->tokens[this->index].type == compiler::TokenType::ID){
            this->match(compiler::TokenType::ID);
            this->atribuicao_ou_chamada();
        }else if(this->tokens[this->index].type == compiler::TokenType::IF){
            this->comando_se();
        }else if(this->tokens[this->index].type == compiler::TokenType::WHILE){
            this->match(compiler::TokenType::WHILE);
            this->expr();
            this->bloco();
        }else if(this->tokens[this->index].type == compiler::TokenType::PRINTLN){
            this->match(compiler::TokenType::PRINTLN);
            this->match(compiler::TokenType::LBRACKET);
            this->match(compiler::TokenType::FMT_STRING);
            this->match(compiler::TokenType::COMMA);
            this->lista_args();
            this->match(compiler::TokenType::RBRACKET);
            this->match(compiler::TokenType::SEMICOLON);
        }else if(this->tokens[this->index].type == compiler::TokenType::RETURN){
            this->match(compiler::TokenType::RETURN);
            this->expr();
            this->match(compiler::TokenType::SEMICOLON);
        }else if(!this->error){
            std::cout << "8 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::atribuicao_ou_chamada(){
        if(this->tokens[this->index].type == compiler::TokenType::ASSIGN){
            this->match(compiler::TokenType::ASSIGN);
            this->expr();
            this->match(compiler::TokenType::SEMICOLON);
        }else if(this->tokens[this->index].type == compiler::TokenType::LBRACKET){
            this->match(compiler::TokenType::LBRACKET);
            this->lista_args();
            this->match(compiler::TokenType::RBRACKET);
            this->match(compiler::TokenType::SEMICOLON);
        }else if(!this->error){
            std::cout << "9 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::comando_se(){
        if(this->tokens[this->index].type == compiler::TokenType::IF){
            this->match(compiler::TokenType::IF);
            this->expr();
            this->bloco();
            this->comando_senao();
        }else if(this->tokens[this->index].type == compiler::TokenType::LBRACE){
            this->bloco();
        }else if(!this->error){
            std::cout << "10 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::comando_senao(){
        if(this->tokens[this->index].type == compiler::TokenType::ELSE){
            this->match(compiler::TokenType::ELSE);
            this->comando_se();
        }
    }
    // Falta implementar o expr;

    void Parser::print_vecotr(){
        for(compiler::Token ti : tokens){
            std::cout << "lexema: " << '"' << ti.lexeme << '"' << '\n';
            std::cout << "Token Type: " << compiler::table_converter[(int)ti.type] << '\n';
            std::cout << "Line Number: " << ti.lineNumber << '\n';
            std::cout << '\n';
        }
    }

    // int Parser::get_index(){
    //     return this->index;
    // }

}