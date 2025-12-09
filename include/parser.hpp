#ifndef PARSER_HPP
#define PARSER_HPP

#include "token.hpp"
#include "symbol_table.hpp"
#include "asa.hpp"
#include <vector>
#include <stdbool.h>
#include <unordered_map>
#include <memory>

namespace compiler{
    class Parser {
        public:
            Parser(std::vector<compiler::Token> &t);
            void initialize_parser();
        private:
            std::vector<compiler::Token> tokens;
            compiler::SymbolTableList symbol_table_list;
            std::unordered_map<std::string, std::shared_ptr<ASTNode>> ast_list;
            size_t index;
            bool error;

            void imprimir_table_symbol();   
            void imprimir_ast();   
            void match(compiler::TokenType tok);
            void programa();
            void funcao_seq();
            void funcao();
            void nome_funcao();
            std::optional<std::vector<std::pair<std::string, compiler::DataType>>> lista_params();
            void lista_params2(std::vector<std::pair<std::string, compiler::DataType>> &list_ids);
            compiler::DataType tipo_retorno_funcao();
            std::shared_ptr<compiler::ASTNode> bloco(compiler::SymbolTable &symbol_table_local);
            std::shared_ptr<compiler::ASTNode> sequencia(compiler::SymbolTable &symbol_table_local, std::shared_ptr<compiler::BlockNode> block_node);
            void declaracao(compiler::SymbolTable &symbol_table_local);
            std::optional<std::vector<std::string>> var_list();
            void var_list2(std::vector<std::string> &list_ids);
            compiler::DataType type();
            std::shared_ptr<compiler::ASTNode> comando(compiler::SymbolTable &symbol_table_local);
            std::shared_ptr<compiler::ASTNode> atribuicao_ou_chamada(compiler::SymbolTable &symbol_table_local, std::string &id_lexeme);
            std::shared_ptr<compiler::ASTNode> comando_se(compiler::SymbolTable &symbol_table_local);
            std::shared_ptr<compiler::ASTNode> comando_senao(compiler::SymbolTable &symbol_table_local);
            std::shared_ptr<compiler::ASTNode> expr(compiler::SymbolTable &symbol_table_local);
            std::shared_ptr<compiler::ASTNode> expr_opc(compiler::SymbolTable &symbol_table_local, std::shared_ptr<compiler::ASTNode> left_leaf_knot);
            void op_igual();
            std::shared_ptr<compiler::ASTNode> rel(compiler::SymbolTable &symbol_table_local);
            std::shared_ptr<compiler::ASTNode> rel_opc(compiler::SymbolTable &symbol_table_local, std::shared_ptr<compiler::ASTNode> left_leaf_knot);
            void op_rel();
            std::shared_ptr<compiler::ASTNode> adicao(compiler::SymbolTable &symbol_table_local);
            std::shared_ptr<compiler::ASTNode> adicao_opc(compiler::SymbolTable &symbol_table_local, std::shared_ptr<compiler::ASTNode> left_leaf_knot);
            void op_adicao();
            std::shared_ptr<compiler::ASTNode> termo(compiler::SymbolTable &symbol_table_local);
            std::shared_ptr<compiler::ASTNode> termo_opc(compiler::SymbolTable &symbol_table_local, std::shared_ptr<compiler::ASTNode> left_leaf_knot);
            void op_mult();
            std::shared_ptr<compiler::ASTNode> fator(compiler::SymbolTable &symbol_table_local);
            std::shared_ptr<compiler::ASTNode> chamada_funcao(compiler::SymbolTable &symbol_table_local, std::string &id_lexeme);
            void lista_args(compiler::SymbolTable &symbol_table_local, std::vector<std::string> &arguments, std::vector<std::shared_ptr<ASTNode>>& argument_nodes);
            void lista_args2(compiler::SymbolTable &symbol_table_local, std::vector<std::string> &arguments, std::vector<std::shared_ptr<ASTNode>>& argument_nodes);
            void arg(compiler::SymbolTable &symbol_table_local, std::vector<std::string> &arguments, std::vector<std::shared_ptr<ASTNode>>& argument_nodes);
    };
}

#endif