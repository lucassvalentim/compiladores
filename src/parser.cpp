#include "../include/parser.hpp"
#include "../include/token.hpp"
#include "../include/symbol_table.hpp"
#include <iostream>

namespace compiler {
    Parser::Parser(std::vector<compiler::Token> &t) : tokens(t), index(0), error(false) {}

    // Inicializador do Parser
    void Parser::initialize_parser(){
        this->programa();
    }
    // Funcao que verifica a corretude do token
    void Parser::match(compiler::TokenType tok){
        if(this->index < this->tokens.size() && tok == this->tokens[this->index].type){
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
                std::cout << "\nFim da analise sintatica\n";

                std::cout << "\n==================== Tabela de simbolos ====================\n";
                std::cout << "Existem " << this->symbol_table_list.get_all().size() << " tabelas de simbolos" << std::endl;
                for(auto stl : this->symbol_table_list.get_all()){
                    std::cout << "Tabela de simbolos da " << stl.first << ": []" << std::endl;
                }
                std::cout << "\n=============================================================\n";
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
            compiler::SymbolTable st;
            
            std::string scope_name = "";
            if(this->index < this->tokens.size()){
                scope_name = this->tokens[this->index].lexeme;
            }
            this->symbol_table_list.insert_table(scope_name, st);

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
            this->sequencia();//1
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
        }else if(this->tokens[this->index].type == compiler::TokenType::ID ||
                this->tokens[this->index].type == compiler::TokenType::IF  ||
                this->tokens[this->index].type == compiler::TokenType::WHILE ||
                this->tokens[this->index].type == compiler::TokenType::PRINTLN ||
                this->tokens[this->index].type == compiler::TokenType::RETURN){
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
    }
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
    void Parser::expr(){
        if(this->tokens[this->index].type == compiler::TokenType::ID || 
            this->tokens[this->index].type == compiler::TokenType::INT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::FLOAT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::CHAR_LITERAL ||
            this->tokens[this->index].type == compiler::TokenType::LBRACKET){
                this->rel();
                this->expr_opc();
        }else if(!this->error){
            std::cout << "11 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::expr_opc(){
        if(this->tokens[this->index].type == compiler::TokenType::EQ ||
            this->tokens[this->index].type == compiler::TokenType::NE){
            this->op_igual();
            this->rel();
            this->expr_opc();
        }
    }
    void Parser::op_igual(){
        if(this->tokens[this->index].type == compiler::TokenType::EQ){
            this->match(compiler::TokenType::EQ);
        }else if(this->tokens[this->index].type == compiler::TokenType::NE){
            this->match(compiler::TokenType::NE);
        }else if(!this->error){
            std::cout << "12 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::rel(){
        if(this->tokens[this->index].type == compiler::TokenType::ID || 
            this->tokens[this->index].type == compiler::TokenType::INT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::FLOAT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::CHAR_LITERAL ||
            this->tokens[this->index].type == compiler::TokenType::LBRACKET){
                this->adicao();
                this->rel_opc();
        }else if(!this->error){
            std::cout << "12 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::rel_opc(){
        if(this->tokens[this->index].type == compiler::TokenType::LT || 
            this->tokens[this->index].type == compiler::TokenType::LE ||
            this->tokens[this->index].type == compiler::TokenType::GT ||
            this->tokens[this->index].type == compiler::TokenType::GE){
                this->op_rel();
                this->adicao();
                this->rel_opc();
            }
    }
    void Parser::op_rel(){
        if(this->tokens[this->index].type == compiler::TokenType::LT){
            this->match(compiler::TokenType::LT);
        } else if(this->tokens[this->index].type == compiler::TokenType::LE){
            this->match(compiler::TokenType::LE);
        }else if(this->tokens[this->index].type == compiler::TokenType::GT){
            this->match(compiler::TokenType::GT);
        }else if(this->tokens[this->index].type == compiler::TokenType::GE){
            this->match(compiler::TokenType::GE);
        }else if(!this->error){
            std::cout << "13 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::adicao(){
        if(this->tokens[this->index].type == compiler::TokenType::ID || 
            this->tokens[this->index].type == compiler::TokenType::INT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::FLOAT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::CHAR_LITERAL ||
            this->tokens[this->index].type == compiler::TokenType::LBRACKET){
                this->termo();
                this->adicao_opc();   
        }else if(!this->error){
            std::cout << "14 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }       
    }
    void Parser::adicao_opc(){
        if(this->tokens[this->index].type == compiler::TokenType::PLUS ||
            this->tokens[this->index].type == compiler::TokenType::MINUS){
                this->op_adicao();
                this->termo();
                this->adicao_opc();
            }       
    }
    void Parser::op_adicao(){
        if(this->tokens[this->index].type == compiler::TokenType::PLUS){
            this->match(compiler::TokenType::PLUS);
        }else if(this->tokens[this->index].type == compiler::TokenType::MINUS){
            this->match(compiler::TokenType::MINUS);
        }else if(!this->error){
            std::cout << "15 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        } 
    }
    void Parser::termo(){
        if(this->tokens[this->index].type == compiler::TokenType::ID || 
            this->tokens[this->index].type == compiler::TokenType::INT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::FLOAT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::CHAR_LITERAL ||
            this->tokens[this->index].type == compiler::TokenType::LBRACKET){
                this->fator();
                this->termo_opc();   
        }else if(!this->error){
            std::cout << "16 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        } 
    }
    void Parser::termo_opc(){
        if(this->tokens[this->index].type == compiler::TokenType::MULT ||
        this->tokens[this->index].type == compiler::TokenType::DIV){
            this->op_mult();
            this->fator();
            this->termo_opc();
        }
    }
    void Parser::op_mult(){
        if(this->tokens[this->index].type == compiler::TokenType::MULT){
            this->match(compiler::TokenType::MULT);
        }else if(this->tokens[this->index].type == compiler::TokenType::DIV){
            this->match(compiler::TokenType::DIV);
        }else if(!this->error){
            std::cout << "17 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::fator(){
        if(this->tokens[this->index].type == compiler::TokenType::ID){
            this->match(compiler::TokenType::ID);
            this->chamada_funcao();
        }else if(this->tokens[this->index].type == compiler::TokenType::INT_CONST){
          this->match(compiler::TokenType::INT_CONST);
        }else if (this->tokens[this->index].type == compiler::TokenType::FLOAT_CONST){
          this->match(compiler::TokenType::FLOAT_CONST);
        }else if (this->tokens[this->index].type == compiler::TokenType::CHAR_LITERAL){
          this->match(compiler::TokenType::CHAR_LITERAL);
        }else if (this->tokens[this->index].type == compiler::TokenType::LBRACKET){
            this->match(compiler::TokenType::LBRACKET);
            this->expr();
            this->match(compiler::TokenType::RBRACKET);
        }else if(!this->error){
            std::cout << "18 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::chamada_funcao(){
        if(this->tokens[this->index].type == compiler::TokenType::LBRACKET){
            this->match(compiler::TokenType::LBRACKET);
            this->lista_args();
            this->match(compiler::TokenType::RBRACKET);
        }
    }
    void Parser::lista_args(){
        if(this->tokens[this->index].type == compiler::TokenType::ID ||
            this->tokens[this->index].type == compiler::TokenType::INT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::FLOAT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::CHAR_LITERAL){
                this->arg();
                this->lista_args2();
        }
    }
    void Parser::lista_args2(){
        if(this->tokens[this->index].type == compiler::TokenType::COMMA){
            this->match(compiler::TokenType::COMMA);
            this->arg();
            this->lista_args2();
        }
    }
    void Parser::arg(){
        if(this->tokens[this->index].type == compiler::TokenType::ID){
            this->match(compiler::TokenType::ID);
            this->chamada_funcao();
        }else if(this->tokens[this->index].type == compiler::TokenType::INT_CONST){
          this->match(compiler::TokenType::INT_CONST);
        }else if (this->tokens[this->index].type == compiler::TokenType::FLOAT_CONST){
          this->match(compiler::TokenType::FLOAT_CONST);
        }else if (this->tokens[this->index].type == compiler::TokenType::CHAR_LITERAL){
          this->match(compiler::TokenType::CHAR_LITERAL);
        }else if(!this->error){
            std::cout << "19 Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::print_vecotr(){
        for(compiler::Token ti : tokens){
            std::cout << "lexema: " << '"' << ti.lexeme << '"' << '\n';
            std::cout << "Token Type: " << compiler::table_converter[(int)ti.type] << '\n';
            std::cout << "Line Number: " << ti.lineNumber << '\n';
            std::cout << '\n';
        }
    }
}