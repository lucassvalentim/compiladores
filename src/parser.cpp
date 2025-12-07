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
            std::cout << "entrou\n";
            compiler::SymbolTable st = this->funcao();
            this->funcao_seq(st);
            if(this->tokens[this->index].type == compiler::TokenType::END_OF_FILE){
                match(compiler::TokenType::END_OF_FILE);
                std::cout << "\nFim da Analise Sintatica\n";
                
                std::cout << "\n==================== Tabela de simbolos ====================\n";
                std::cout << "Existem " << this->symbol_table_list.get_all().size() << " tabelas de simbolos" << std::endl;
                for(auto stl : this->symbol_table_list.get_all()){
                    std::cout << "Tabela de simbolos da " << stl.first << ": " << std::endl;
                    
                    for(auto se : stl.second.get_all()){
                        std::cout << "name: " << se.second.name << '\n' << "Type: " << int(se.second.type) << '\n' << "Is_param: " << se.second.is_parameter
                        << '\n' << "parameter_position: " << se.second.parameter_position << '\n' << "Call refers: " << "NULL\n"; 
                        std::cout << '\n';
                    }
                }
                std::cout << "\n=============================================================\n";
            }
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::funcao_seq(compiler::SymbolTable &st){
        if(this->tokens[this->index].type == compiler::TokenType::FUNCTION){
            this->funcao();
            this->funcao_seq(st);
        }
    }
    compiler::SymbolTable Parser::funcao(){
        if(this->tokens[this->index].type == compiler::TokenType::FUNCTION){
            compiler::SymbolTable st; // Tabela local para cada funcao
            this->match(compiler::TokenType::FUNCTION);

            // Recuperando o identificador relacionado a funcao
            std::string scope_name = "";
            if(this->index < this->tokens.size()){
                scope_name = this->tokens[this->index].lexeme;
            }
            
            this->nome_funcao();
            this->match(compiler::TokenType::LBRACKET);
            
            // Inserion a lista de parametros na tabela de simbolos
            std::optional<std::vector<std::pair<std::string, compiler::DataType>>> list_ids = this->lista_params();
            
            if(!list_ids){
                std::cout << "Erro ao inserir a lista de parametros\n";
            }

            int pos_param = 0;
            for(auto ids : *list_ids){
                std::cout << ids.first << ' ' << (int)ids.second << ' ' << pos_param << std::endl;
                compiler::SymbolEntry se(ids.first, ids.second, true, pos_param);
                pos_param = pos_param + 1;

                st.insert(se);
            }

            this->match(compiler::TokenType::RBRACKET);
            this->tipo_retorno_funcao();
            this->bloco(st);

            // Adicionando a tabela de simbolos local na lista de tabelas de simbos.
            this->symbol_table_list.insert_table(scope_name, st);
            
            return st;
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::nome_funcao(){
        if(this->tokens[this->index].type == compiler::TokenType::ID){
            this->match(compiler::TokenType::ID);
        }else if(this->tokens[this->index].type == compiler::TokenType::MAIN){
            this->match(compiler::TokenType::MAIN);
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    std::optional<std::vector<std::pair<std::string, compiler::DataType>>> Parser::lista_params(){
        std::vector<std::pair<std::string, compiler::DataType>> list_ids;
        if(this->tokens[this->index].type == compiler::TokenType::ID){

            std::string id_lexeme = this->tokens[this->index].lexeme;
            
            this->match(compiler::TokenType::ID);
            this->match(compiler::TokenType::COLON);
            
            compiler::DataType type_id = this->type();
            list_ids.push_back(std::make_pair(id_lexeme, type_id));
            
            this->lista_params2(list_ids);
        }

        if(list_ids.empty()){
            return std::nullopt;
        }

        return list_ids;
    }
    void Parser::lista_params2(std::vector<std::pair<std::string, compiler::DataType>> &list_ids){
        if(this->tokens[this->index].type == compiler::TokenType::COMMA){
            this->match(compiler::TokenType::COMMA);
        
            std::string id_lexeme = this->tokens[this->index].lexeme;
        
            this->match(compiler::TokenType::ID);   
            this->match(compiler::TokenType::COLON);
            
            compiler::DataType type_id = this->type();
            list_ids.push_back(std::make_pair(id_lexeme, type_id));
            
            this->lista_params2(list_ids);
        }
    }
    void Parser::tipo_retorno_funcao(){
        if(this->tokens[this->index].type == compiler::TokenType::ARROW){
            this->match(compiler::TokenType::ARROW);
            this->type();
        }
    }
    void Parser::bloco(compiler::SymbolTable &st){
        if(this->tokens[this->index].type == compiler::TokenType::LBRACE){
            this->match(compiler::TokenType::LBRACE);
            this->sequencia(st);//1
            this->match(compiler::TokenType::RBRACE);
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::sequencia(compiler::SymbolTable &st){
        if(this->tokens[this->index].type == compiler::TokenType::LET){
            this->declaracao(st);
            this->sequencia(st);
        }else if(this->tokens[this->index].type == compiler::TokenType::ID ||
                this->tokens[this->index].type == compiler::TokenType::IF  ||
                this->tokens[this->index].type == compiler::TokenType::WHILE ||
                this->tokens[this->index].type == compiler::TokenType::PRINTLN ||
                this->tokens[this->index].type == compiler::TokenType::RETURN){
            this->comando(st);
            this->sequencia(st);
        }
    }
    void Parser::declaracao(compiler::SymbolTable &st){
        if(this->tokens[this->index].type == compiler::TokenType::LET){
            this->match(compiler::TokenType::LET);
            
            // recupera todos o ids
            std::optional<std::vector<std::string>> list_ids = this->var_list();
            this->match(compiler::TokenType::COLON);

            // recupera o tipo de todos os ids
            compiler::DataType type_all_ids = this->type();
            
            if(!list_ids){
                std::cout << "Erro ao declarar as variáveis\n";
                return;
            }

            for(auto lexeme : *list_ids){
                compiler::SymbolEntry se(lexeme, type_all_ids, false, -1);
                st.insert(se);
            }

            this->match(compiler::TokenType::SEMICOLON);
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    std::optional<std::vector<std::string>> Parser::var_list(){
        std::vector<std::string> list_ids;
        if(this->tokens[this->index].type == compiler::TokenType::ID){
            // Lista que armazenará todos os ids encontrados na declaracao
            list_ids.push_back(this->tokens[this->index].lexeme);

            this->match(compiler::TokenType::ID);
            this->var_list2(list_ids);
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";    
        }
        
        if(list_ids.empty()){
            return std::nullopt;
        }

        return list_ids;
    }
    void Parser::var_list2(std::vector<std::string> &list_ids){
        if(this->tokens[this->index].type == compiler::TokenType::COMMA){
            this->match(compiler::TokenType::COMMA);

            // Lista que armazenará todos os ids encontrados na declaracao
            list_ids.push_back(this->tokens[this->index].lexeme);

            this->match(compiler::TokenType::ID);
            this->var_list2(list_ids);
        }
    }
    compiler::DataType Parser::type(){
        compiler::DataType type_ids;
        if(this->tokens[this->index].type == compiler::TokenType::INT){
            type_ids = compiler::DataType::INT;
            this->match(compiler::TokenType::INT);
        }else if(this->tokens[this->index].type == compiler::TokenType::FLOAT){
            type_ids = compiler::DataType::FLOAT;
            this->match(compiler::TokenType::FLOAT);
        }else if(this->tokens[this->index].type == compiler::TokenType::CHAR){
            type_ids = compiler::DataType::CHAR;
            this->match(compiler::TokenType::CHAR);
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
            type_ids = compiler::DataType::ERROR;
        }
        return type_ids;
    }
    void Parser::comando(compiler::SymbolTable &st){
        if(this->tokens[this->index].type == compiler::TokenType::ID){
            this->match(compiler::TokenType::ID);
            this->atribuicao_ou_chamada();
        }else if(this->tokens[this->index].type == compiler::TokenType::IF){
            this->comando_se(st);
        }else if(this->tokens[this->index].type == compiler::TokenType::WHILE){
            this->match(compiler::TokenType::WHILE);
            this->expr();
            this->bloco(st);
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
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
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
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::comando_se(compiler::SymbolTable &st){
        if(this->tokens[this->index].type == compiler::TokenType::IF){
            this->match(compiler::TokenType::IF);
            this->expr();
            this->bloco(st);
            this->comando_senao(st);
        }else if(this->tokens[this->index].type == compiler::TokenType::LBRACE){
            this->bloco(st);
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
    void Parser::comando_senao(compiler::SymbolTable &st){
        if(this->tokens[this->index].type == compiler::TokenType::ELSE){
            this->match(compiler::TokenType::ELSE);
            this->comando_se(st);
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
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
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
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
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
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
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
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
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
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
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
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
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
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
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
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
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
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
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
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }
    }
}