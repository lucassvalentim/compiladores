#include "../include/parser.hpp"
#include "../include/token.hpp"
#include "../include/symbol_table.hpp"
#include "../include/asa.hpp"
#include <iostream>
#include <iomanip>

namespace parser_auxiliary{
    inline std::string data_type_to_string(compiler::DataType type) {
        switch(type) {
            case compiler::DataType::INT: return "int";
            case compiler::DataType::FLOAT: return "float";
            case compiler::DataType::CHAR: return "char";
            default: return "void";
        }
    }

    inline compiler::DataTypeASA convert_types(compiler::DataType type) {
        switch(type) {
            case compiler::DataType::INT: return compiler::DataTypeASA::INT;
            case compiler::DataType::FLOAT: return compiler::DataTypeASA::FLOAT;
            case compiler::DataType::CHAR: return compiler::DataTypeASA::CHAR;
            default: return compiler::DataTypeASA::VOID;
        }
    }
}

namespace compiler {
    Parser::Parser(std::vector<compiler::Token> &t) : tokens(t), index(0), error(false) {}
    
    // Funcao para imprimir o resultado final
    void compiler::Parser::imprimir_table_symbol(){
        std::cout << "\n========================================\n";
        std::cout << "        FIM DA ANALISE SINTATICA\n";
        std::cout << "========================================\n\n";

        std::cout << "╔════════════════════════════════════════╗\n";
        std::cout << "║     TABELA DE SIMBOLOS - RESUMO       ║\n";
        std::cout << "╚════════════════════════════════════════╝\n";
        std::cout << "  Total de tabelas: " << this->symbol_table_list.get_all().size() << "\n\n";

        for(auto stl : this->symbol_table_list.get_all()){
            std::cout << "┌────────────────────────────────────────┐\n";
            std::cout << "│ ESCOPO: " << std::left << std::setw(28) << stl.first << "│\n";
            std::cout << "├────────────────────────────────────────┤\n";
            
            int symbol_count = 0;
            for(auto se : stl.second.get_all()){
                symbol_count++;
                std::cout << "│ [" << symbol_count << "] " << std::left << std::setw(32) << se.second.name << "│\n";
                std::cout << "│     Tipo: " << std::left << std::setw(27) << parser_auxiliary::data_type_to_string(se.second.type) << "│\n";
                std::cout << "│     Parametro: " << std::left << std::setw(22) << (se.second.is_parameter ? "Sim" : "Nao") << "│\n";
                
                if(se.second.is_parameter){
                    std::cout << "│     Posicao param: " << std::left << std::setw(18) << se.second.parameter_position << "│\n";
                }
                
                // Chamadas
                if(se.second.calls.size() > 0){
                    std::cout << "│                                        │\n";
                    std::cout << "│     Chamadas referenciadas:            │\n";
                    
                    for(auto call : se.second.calls){
                        std::cout << "│       -> " << std::left << std::setw(27) << call.name << "│\n";
                        std::cout << "│          Args(" << call.num_args() << "): ";
                        
                        std::string args_str = "[";
                        size_t i = 0;
                        for(auto args : call.arguments){
                            args_str += args;
                            if(i < call.num_args() - 1) args_str += ", ";
                            i++;
                        }
                        args_str += "]";
                        
                        std::cout << std::left << std::setw(18) << args_str << "│\n";
                    }
                }
                
                std::cout << "├────────────────────────────────────────┤\n";
            }
            
            std::cout << "│ TIPO DE RETORNO: " << std::left << std::setw(20) << parser_auxiliary::data_type_to_string((stl.second.get_return_type())) << "│\n";
            std::cout << "└────────────────────────────────────────┘\n\n";
        }

        std::cout << "========================================\n";
        std::cout << "      FIM DA TABELA DE SIMBOLOS\n";
        std::cout << "========================================\n";
    }

    void compiler::Parser::imprimir_ast(){
        std::cout << "\n\n";

        std::cout << "╔════════════════════════════════════════╗\n";
        std::cout << "║     ARVORE SINTATICA ABSTRATA          ║\n";
        std::cout << "╚════════════════════════════════════════╝\n";
        
        std::cout << "\n\n";
        for(auto &ast : this->ast_list){
            ast.second->print();
            std::cout << "\n\n";
        }
                
        std::cout << "========================================\n";
        std::cout << "   FIM DA ARVORE SINTATICA ABSTRATA\n";
        std::cout << "========================================\n";
    }

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
                this->imprimir_table_symbol();
                this->imprimir_ast();
            }
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
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
            compiler::SymbolTable symbol_local_table; // Tabela local para cada funcao
            this->match(compiler::TokenType::FUNCTION);
            
            // Recuperando o identificador relacionado a funcao
            std::string scope_name = "";
            if(this->index < this->tokens.size()){
                scope_name = this->tokens[this->index].lexeme;
            }
            
            // CRIAR NO DE FUNCAO
            auto function_node = std::make_shared<FunctionNode>(scope_name);

            this->nome_funcao();
            this->match(compiler::TokenType::LBRACKET);
            
            // Inserir a lista de parametros na tabela de simbolos
            std::optional<std::vector<std::pair<std::string, compiler::DataType>>> list_ids = this->lista_params();
            
            if(list_ids){
                int pos_param = 0;
                for(auto ids : *list_ids){
                    symbol_local_table.insert(
                        compiler::SymbolEntry(ids.first, ids.second, true, pos_param)
                    );
                    pos_param = pos_param + 1;
                }
            }

            this->match(compiler::TokenType::RBRACKET);

            // Recupera o tipo de retorno da funcao
            DataType return_type = this->tipo_retorno_funcao();
            symbol_local_table.set_return_type(return_type);
            function_node->set_data_type(parser_auxiliary::convert_types(return_type));
            
            auto block_node = this->bloco(symbol_local_table);
            function_node->add_child(block_node); // Imprime a arvore

            // Adicionando a tabela de simbolos local na lista de tabelas de simbos.
            this->symbol_table_list.insert_table(scope_name, std::move(symbol_local_table));
            
            // Adiciona a arvore de sintaxe abstrata local na lista das arvores
            this->ast_list[scope_name] = function_node;
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

            // Captura do lexema do ID identificado para criar lista de parametros
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
    compiler::DataType Parser::tipo_retorno_funcao(){
        compiler::DataType return_type = compiler::DataType::ERROR;
        if(this->tokens[this->index].type == compiler::TokenType::ARROW){
            this->match(compiler::TokenType::ARROW);
            return_type = this->type();
        }
        return return_type;
    }
    std::shared_ptr<compiler::ASTNode> Parser::bloco(compiler::SymbolTable &symbol_local_table){
        auto block_node = std::make_shared<BlockNode>();
        if(this->tokens[this->index].type == compiler::TokenType::LBRACE){
            this->match(compiler::TokenType::LBRACE);
            this->sequencia(symbol_local_table, block_node);
            this->match(compiler::TokenType::RBRACE);
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }

        return block_node;
    }
    std::shared_ptr<compiler::ASTNode> Parser::sequencia(compiler::SymbolTable &symbol_local_table, std::shared_ptr<compiler::BlockNode> block_node){
        if(this->tokens[this->index].type == compiler::TokenType::LET){
            this->declaracao(symbol_local_table);
            this->sequencia(symbol_local_table, block_node);
        }else if(this->tokens[this->index].type == compiler::TokenType::ID ||
                this->tokens[this->index].type == compiler::TokenType::IF  ||
                this->tokens[this->index].type == compiler::TokenType::WHILE ||
                this->tokens[this->index].type == compiler::TokenType::PRINTLN ||
                this->tokens[this->index].type == compiler::TokenType::RETURN){
            
            auto comando_node = this->comando(symbol_local_table);
            if(comando_node){
                block_node->add_statement(comando_node);
            }

            this->sequencia(symbol_local_table, block_node);
        }

        return nullptr;
    }
    void Parser::declaracao(compiler::SymbolTable &symbol_local_table){
        if(this->tokens[this->index].type == compiler::TokenType::LET){
            this->match(compiler::TokenType::LET);
            
            // recupera todos o ids
            std::optional<std::vector<std::string>> list_ids = this->var_list();
            this->match(compiler::TokenType::COLON);

            // recupera o tipo de todos os ids
            compiler::DataType type_all_ids = this->type();
            
            // Adiciona na tabela de simbolos todas as variaveis declaradas
            if(list_ids){
                for(auto lexeme : *list_ids){
                    if(!symbol_local_table.find(lexeme)){
                        symbol_local_table.insert(
                            compiler::SymbolEntry(lexeme, type_all_ids, false, -1)
                        );
                    }else{
                        std::cout << "A variavel " << "'" << lexeme << "'" << " ja foi declarada\n";
                    }
                }
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

            // Lista que armazenara todos os ids encontrados na declaracao
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
    std::shared_ptr<compiler::ASTNode> Parser::comando(compiler::SymbolTable &symbol_local_table){
        if(this->tokens[this->index].type == compiler::TokenType::ID){
            std::string id_lexeme = this->tokens[this->index].lexeme;
            this->match(compiler::TokenType::ID);

            // No para criacao de chamada de funcao ou atribuicao
            auto call_or_assign_node =  this->atribuicao_ou_chamada(symbol_local_table, id_lexeme);
            
            return call_or_assign_node;
        }else if(this->tokens[this->index].type == compiler::TokenType::IF){
            return this->comando_se(symbol_local_table);
        }else if(this->tokens[this->index].type == compiler::TokenType::WHILE){
            this->match(compiler::TokenType::WHILE);
            auto node_expr = this->expr(symbol_local_table);
            auto node_block = this->bloco(symbol_local_table);

            auto while_node = std::make_shared<compiler::WhileNode>();
            while_node->set_while(node_expr, node_block);

            return while_node;
        }else if(this->tokens[this->index].type == compiler::TokenType::PRINTLN){
            symbol_local_table.insert(
                compiler::SymbolEntry(this->tokens[this->index].lexeme, compiler::DataType::VOID, false, -1)
            );

            this->match(compiler::TokenType::PRINTLN);
            this->match(compiler::TokenType::LBRACKET);
            this->match(compiler::TokenType::FMT_STRING);
            this->match(compiler::TokenType::COMMA);

            std::vector<std::string> arguments; // Lista de argumentos que sera passado para a funcao "println"
            std::vector<std::shared_ptr<ASTNode>> argument_nodes; 
            this->lista_args(symbol_local_table, arguments, argument_nodes);

            // Adiciona o comando PRINTLN na arvore
            auto println_node = std::make_shared<compiler::PrintNode>();
            
            // Adiciona os argumentos da chamda de funcao
            for(auto child : argument_nodes){
                println_node->set_argument(child);
            }

            this->match(compiler::TokenType::RBRACKET);
            this->match(compiler::TokenType::SEMICOLON);

            return println_node;
        }else if(this->tokens[this->index].type == compiler::TokenType::RETURN){
            this->match(compiler::TokenType::RETURN);
            auto expr_node = this->expr(symbol_local_table);
            
            auto return_node = std::make_shared<compiler::ReturnNode>();
            return_node->set_expression(expr_node);

            this->match(compiler::TokenType::SEMICOLON);
            return return_node;
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }

        return nullptr;
    }
    std::shared_ptr<compiler::ASTNode> Parser::atribuicao_ou_chamada(compiler::SymbolTable &symbol_local_table, std::string &id_lexeme){
        if(this->tokens[this->index].type == compiler::TokenType::ASSIGN){
            this->match(compiler::TokenType::ASSIGN);
            auto expr_return = this->expr(symbol_local_table);
            this->match(compiler::TokenType::SEMICOLON);
            
            if(!symbol_local_table.find(id_lexeme)){
                std::cout << "A variavel " << "'" << id_lexeme << "'" <<  " nao foi declarada ou passada por parametros\n";
            }
             
            auto id_knot = std::make_shared<compiler::IdNode>(id_lexeme);
            auto assign_node = std::make_shared<compiler::AssignNode>();
            assign_node->set_assignment(id_knot, expr_return);
            return assign_node;
        }else if(this->tokens[this->index].type == compiler::TokenType::LBRACKET){
            this->match(compiler::TokenType::LBRACKET);

            std::vector<std::string> arguments;
            std::vector<std::shared_ptr<ASTNode>> argument_nodes;
            this->lista_args(symbol_local_table, arguments, argument_nodes);

            // Criar o node de chamada de funcao
            auto call_node = std::make_shared<CallNode>(id_lexeme);
            
            // Adicionar os argumentos da chamda de funcao
            for(auto child : argument_nodes){
                call_node->add_argument(child);
            }

            this->match(compiler::TokenType::RBRACKET);
            this->match(compiler::TokenType::SEMICOLON);

            // Criacao do function registe na funcao chamada
            compiler::FunctionRegister function_register(id_lexeme, arguments);

            // Inserir na tabela a chamada de funcao
            std::optional<compiler::SymbolEntry> symbol_entry = symbol_local_table.find(id_lexeme);
            if(symbol_entry){
                (*symbol_entry).add_function_call(function_register);
                symbol_local_table.insert(
                    std::move((*symbol_entry))
                );
            }else{
                // Recupera a tabela de simbolos da funcao chamada
                std::optional<SymbolTable> temp_symbol_table = this->symbol_table_list.find_table(id_lexeme);
                // Cria um entrada para inserir na tabela de simbolos
                compiler::SymbolEntry symbol_entry_local(id_lexeme, (*temp_symbol_table).get_return_type(), false, -1);
                symbol_entry_local.add_function_call(function_register);
                symbol_local_table.insert(
                        std::move((symbol_entry_local))
                );                
            }

            return call_node;
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }

        return nullptr;
    }
    std::shared_ptr<compiler::ASTNode> Parser::comando_se(compiler::SymbolTable &symbol_local_table){
        if(this->tokens[this->index].type == compiler::TokenType::IF){
            this->match(compiler::TokenType::IF);
            
            auto condition_node = this->expr(symbol_local_table);
            auto then_block = this->bloco(symbol_local_table);
            
            std::shared_ptr<compiler::IfNode> if_condition;
            if_condition = std::make_shared<compiler::IfNode>();
            if_condition->set_if(condition_node, then_block);

            auto else_node = this->comando_senao(symbol_local_table);

            if(else_node){
                if_condition->set_if_else(condition_node, then_block, else_node);
            }

            return if_condition;
        }else if(this->tokens[this->index].type == compiler::TokenType::LBRACE){
            return this->bloco(symbol_local_table);
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }

        return nullptr;
    }
    std::shared_ptr<compiler::ASTNode> Parser::comando_senao(compiler::SymbolTable &symbol_local_table){
        if(this->tokens[this->index].type == compiler::TokenType::ELSE){
            this->match(compiler::TokenType::ELSE);
            return this->comando_se(symbol_local_table);
        }

        return nullptr;
    }
    std::shared_ptr<compiler::ASTNode> Parser::expr(compiler::SymbolTable &symbol_local_table){
        std::shared_ptr<compiler::ASTNode> leaf_knot;
        if(this->tokens[this->index].type == compiler::TokenType::ID || 
            this->tokens[this->index].type == compiler::TokenType::INT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::FLOAT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::CHAR_LITERAL ||
            this->tokens[this->index].type == compiler::TokenType::LBRACKET){
                leaf_knot = this->rel(symbol_local_table);
                leaf_knot = this->expr_opc(symbol_local_table, leaf_knot);
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }

        return leaf_knot;
    }
    std::shared_ptr<compiler::ASTNode> Parser::expr_opc(compiler::SymbolTable &symbol_local_table, std::shared_ptr<compiler::ASTNode> left_leaf_knot){
        if(this->tokens[this->index].type == compiler::TokenType::EQ ||
            this->tokens[this->index].type == compiler::TokenType::NE){
            auto rel_knot = std::make_shared<compiler::RelOp>(this->tokens[this->index].lexeme);
            this->op_igual();
            
            // Recupera o operando da direita
            std::shared_ptr<compiler::ASTNode> right_leaf_knot = this->rel(symbol_local_table);;
            rel_knot->set_operands(left_leaf_knot, right_leaf_knot);

            return this->expr_opc(symbol_local_table, rel_knot);
        }

        return left_leaf_knot;
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
    std::shared_ptr<compiler::ASTNode> Parser::rel(compiler::SymbolTable &symbol_local_table){
        std::shared_ptr<compiler::ASTNode> leaf_knot;
        if(this->tokens[this->index].type == compiler::TokenType::ID || 
            this->tokens[this->index].type == compiler::TokenType::INT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::FLOAT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::CHAR_LITERAL ||
            this->tokens[this->index].type == compiler::TokenType::LBRACKET){
                leaf_knot = this->adicao(symbol_local_table);
                leaf_knot = this->rel_opc(symbol_local_table, leaf_knot);
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }

        return leaf_knot;
    }
    std::shared_ptr<compiler::ASTNode> Parser::rel_opc(compiler::SymbolTable &symbol_local_table, std::shared_ptr<compiler::ASTNode> left_leaf_knot){
        if(this->tokens[this->index].type == compiler::TokenType::LT || 
            this->tokens[this->index].type == compiler::TokenType::LE ||
            this->tokens[this->index].type == compiler::TokenType::GT ||
            this->tokens[this->index].type == compiler::TokenType::GE){
                auto rel_knot = std::make_shared<compiler::RelOp>(this->tokens[this->index].lexeme);
                this->op_rel();
                
                // Recupera o operando da direita
                std::shared_ptr<compiler::ASTNode> right_leaf_knot = this->adicao(symbol_local_table);
                rel_knot->set_operands(left_leaf_knot, right_leaf_knot);

                return this->rel_opc(symbol_local_table, rel_knot);
            }

        return left_leaf_knot;
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
    std::shared_ptr<compiler::ASTNode> Parser::adicao(compiler::SymbolTable &symbol_local_table){
        std::shared_ptr<compiler::ASTNode> leaf_knot;
        if(this->tokens[this->index].type == compiler::TokenType::ID || 
            this->tokens[this->index].type == compiler::TokenType::INT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::FLOAT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::CHAR_LITERAL ||
            this->tokens[this->index].type == compiler::TokenType::LBRACKET){
                leaf_knot = this->termo(symbol_local_table);
                leaf_knot = this->adicao_opc(symbol_local_table, leaf_knot);
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }

        return leaf_knot;
    }
    std::shared_ptr<compiler::ASTNode> Parser::adicao_opc(compiler::SymbolTable &symbol_local_table, std::shared_ptr<compiler::ASTNode> left_leaf_knot){
        if(this->tokens[this->index].type == compiler::TokenType::PLUS ||
            this->tokens[this->index].type == compiler::TokenType::MINUS){
                auto arithmetic_knot = std::make_shared<compiler::AritOp>(this->tokens[this->index].lexeme);
                this->op_adicao();
                
                // Recupera o operando da direita
                std::shared_ptr<compiler::ASTNode> right_leaf_knot = this->termo(symbol_local_table);
                arithmetic_knot->set_operands(left_leaf_knot, right_leaf_knot);
                
                return this->adicao_opc(symbol_local_table, arithmetic_knot);
            }

        return left_leaf_knot;
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
    std::shared_ptr<compiler::ASTNode> Parser::termo(compiler::SymbolTable &symbol_local_table){
        std::shared_ptr<compiler::ASTNode> leaf_knot;
        if(this->tokens[this->index].type == compiler::TokenType::ID || 
            this->tokens[this->index].type == compiler::TokenType::INT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::FLOAT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::CHAR_LITERAL ||
            this->tokens[this->index].type == compiler::TokenType::LBRACKET){
                leaf_knot = this->fator(symbol_local_table);
                leaf_knot = this->termo_opc(symbol_local_table, leaf_knot);   
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        } 

        return leaf_knot;
    }
    std::shared_ptr<compiler::ASTNode> Parser::termo_opc(compiler::SymbolTable &symbol_local_table, std::shared_ptr<compiler::ASTNode> left_leaf_knot){
        if(this->tokens[this->index].type == compiler::TokenType::MULT ||
            this->tokens[this->index].type == compiler::TokenType::DIV){
            auto arithmetic_knot = std::make_shared<compiler::AritOp>(this->tokens[this->index].lexeme);
            this->op_mult();
            
            // Recupera o operando da direita
            std::shared_ptr<compiler::ASTNode> right_leaf_knot = this->fator(symbol_local_table);
            arithmetic_knot->set_operands(left_leaf_knot, right_leaf_knot);

            return this->termo_opc(symbol_local_table, arithmetic_knot);
        }

        return left_leaf_knot;
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
    std::shared_ptr<compiler::ASTNode> Parser::fator(compiler::SymbolTable &symbol_local_table){
        std::shared_ptr<compiler::ASTNode> leaf_knot;

        if(this->tokens[this->index].type == compiler::TokenType::ID){
            // lexema do id atual avaliado para verificao se eh ou nao uma funcao 
            std::string id_lexeme = this->tokens[this->index].lexeme;
            this->match(compiler::TokenType::ID);

            leaf_knot = this->chamada_funcao(symbol_local_table, id_lexeme);
        }else if(this->tokens[this->index].type == compiler::TokenType::INT_CONST){
            leaf_knot = std::make_shared<compiler::IntConstNode>(std::stoi(this->tokens[this->index].lexeme));
            this->match(compiler::TokenType::INT_CONST);
        }else if (this->tokens[this->index].type == compiler::TokenType::FLOAT_CONST){
            leaf_knot = std::make_shared<compiler::FloatConstNode>(std::stof(this->tokens[this->index].lexeme));
            this->match(compiler::TokenType::FLOAT_CONST);
        }else if (this->tokens[this->index].type == compiler::TokenType::CHAR_LITERAL){
            leaf_knot = std::make_shared<compiler::CharConstNode>(this->tokens[this->index].lexeme[0]);
            this->match(compiler::TokenType::CHAR_LITERAL);
        }else if (this->tokens[this->index].type == compiler::TokenType::LBRACKET){
            this->match(compiler::TokenType::LBRACKET);
            leaf_knot = this->expr(symbol_local_table);
            this->match(compiler::TokenType::RBRACKET);
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }

        return leaf_knot;
    }
    std::shared_ptr<compiler::ASTNode> Parser::chamada_funcao(compiler::SymbolTable &symbol_local_table, std::string &id_lexeme){
        if(this->tokens[this->index].type == compiler::TokenType::LBRACKET){
            
            this->match(compiler::TokenType::LBRACKET);
            
            std::vector<std::string> arguments;
            std::vector<std::shared_ptr<ASTNode>> argument_nodes;
            this->lista_args(symbol_local_table, arguments, argument_nodes);

            // Cria o node de chamada de funcao
            auto call_node = std::make_shared<CallNode>(id_lexeme);
            
            // Adiciona os argumentos da chamda de funcao
            for(auto child : argument_nodes){
                call_node->add_argument(child);
            }
            
            this->match(compiler::TokenType::RBRACKET);
            
            // Cria um function register;
            compiler::FunctionRegister function_register(id_lexeme, arguments);
            
            // Inserir na tabela a chamada de funcao
            std::optional<compiler::SymbolEntry> symbol_entry = symbol_local_table.find(id_lexeme);
            if(symbol_entry){
                (*symbol_entry).add_function_call(function_register);
                symbol_local_table.insert(
                    std::move((*symbol_entry))
                );
            }else{
                // Recupera a tabela de simbolos da funcao chamada
                std::optional<SymbolTable> temp_symbol_table = this->symbol_table_list.find_table(id_lexeme);
                // Criando e inserindo uma entrada para a tabela de simbolos
                compiler::SymbolEntry symbol_entry_local(id_lexeme, (*temp_symbol_table).get_return_type(), false, -1);
                symbol_entry_local.add_function_call(function_register);
                symbol_local_table.insert(
                        std::move((symbol_entry_local))
                );                
            }

            return call_node;
        }else{
            // Caso seja um identificado e nao uma chamada de funcao
            if(!symbol_local_table.find(id_lexeme)){
                std::cout << "A VARIAVEL " << "'" << id_lexeme << "'" << " nao foi declarada nem passada por parametros\n";
            }
        }
        
        return std::make_shared<compiler::IdNode>(id_lexeme);
    }
    void Parser::lista_args(compiler::SymbolTable &symbol_local_table, std::vector<std::string> &arguments, std::vector<std::shared_ptr<ASTNode>>& argument_nodes){
        if(this->tokens[this->index].type == compiler::TokenType::ID ||
            this->tokens[this->index].type == compiler::TokenType::INT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::FLOAT_CONST ||
            this->tokens[this->index].type == compiler::TokenType::CHAR_LITERAL){
                this->arg(symbol_local_table, arguments, argument_nodes);
                this->lista_args2(symbol_local_table, arguments, argument_nodes);
        }
    }
    void Parser::lista_args2(compiler::SymbolTable &symbol_local_table, std::vector<std::string> &arguments, std::vector<std::shared_ptr<ASTNode>>& argument_nodes){
        if(this->tokens[this->index].type == compiler::TokenType::COMMA){
            this->match(compiler::TokenType::COMMA);
            this->arg(symbol_local_table, arguments, argument_nodes);
            this->lista_args2(symbol_local_table, arguments, argument_nodes);
        }
    }
    void Parser::arg(compiler::SymbolTable &symbol_local_table, std::vector<std::string> &arguments, std::vector<std::shared_ptr<ASTNode>>& argument_nodes){
        std::string arg_lexeme = "";
        std::shared_ptr<compiler::ASTNode> arg_node;
        if(this->tokens[this->index].type == compiler::TokenType::ID){
            // lexema do id atual da chamada para possivel chamada de funcao 
            std::string id_lexeme = this->tokens[this->index].lexeme;
            
            this->match(compiler::TokenType::ID);
            
            // Recuperando o lexema do argumento atual
            arg_lexeme = id_lexeme;
            // Recuperando o node do ID
            arg_node = this->chamada_funcao(symbol_local_table, id_lexeme);

        }else if(this->tokens[this->index].type == compiler::TokenType::INT_CONST){
            // Recuperando o lexema do argumento atual
            arg_lexeme = this->tokens[this->index].lexeme;

            // // Instanciando o literal atual
            arg_node = std::make_shared<IntConstNode>(std::stoi(arg_lexeme));

            this->match(compiler::TokenType::INT_CONST);
        }else if (this->tokens[this->index].type == compiler::TokenType::FLOAT_CONST){
            // Recuperando o lexema do argumento atual
            arg_lexeme = this->tokens[this->index].lexeme;

            // // Instanciando o literal atual
            arg_node = std::make_shared<FloatConstNode>(std::stof(arg_lexeme));
          
            this->match(compiler::TokenType::FLOAT_CONST);
        }else if (this->tokens[this->index].type == compiler::TokenType::CHAR_LITERAL){
            // Recuperando o lexema do argumento atual
            arg_lexeme = this->tokens[this->index].lexeme;

            // Instanciando o literal atual
            arg_node = std::make_shared<CharConstNode>(arg_lexeme[0]);
          
            this->match(compiler::TokenType::CHAR_LITERAL);
        }else if(!this->error){
            std::cout << "Erro sintatico na linha " << this->tokens[this->index].lineNumber << ": " <<
            this->tokens[this->index].lexeme << " nao esperado na entrada\n";
        }

        arguments.push_back(arg_lexeme);
        argument_nodes.push_back(arg_node);
    }
}