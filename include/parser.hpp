#ifndef PARSER_HPP
#define PARSER_HPP

#include "token.hpp"
#include "symbol_table.hpp"
#include <vector>
#include <stdbool.h>

namespace compiler{
    class Parser {
        public:
            Parser(std::vector<compiler::Token> &t);
            void initialize_parser();
        private:
            std::vector<compiler::Token> tokens;
            compiler::SymbolTableList symbol_table_list;
            size_t index;
            bool error;

            void imprimir_parser();   
            void match(compiler::TokenType tok);
            void programa();
            void funcao_seq();
            void funcao();
            void nome_funcao();
            std::optional<std::vector<std::pair<std::string, compiler::DataType>>> lista_params();
            void lista_params2(std::vector<std::pair<std::string, compiler::DataType>> &list_ids);
            compiler::DataType tipo_retorno_funcao();
            void bloco(compiler::SymbolTable &symbol_table_local);
            void sequencia(compiler::SymbolTable &symbol_table_local);
            void declaracao(compiler::SymbolTable &symbol_table_local);
            std::optional<std::vector<std::string>> var_list();
            void var_list2(std::vector<std::string> &list_ids);
            compiler::DataType type();
            void comando(compiler::SymbolTable &symbol_table_local);
            void atribuicao_ou_chamada(compiler::SymbolTable &symbol_table_local, std::string &id_lexeme);
            void comando_se(compiler::SymbolTable &symbol_table_local);
            void comando_senao(compiler::SymbolTable &symbol_table_local);
            void expr(compiler::SymbolTable &symbol_table_local);
            void expr_opc(compiler::SymbolTable &symbol_table_local);
            void op_igual();
            void rel(compiler::SymbolTable &symbol_table_local);
            void rel_opc(compiler::SymbolTable &symbol_table_local);
            void op_rel();
            void adicao(compiler::SymbolTable &symbol_table_local);
            void adicao_opc(compiler::SymbolTable &symbol_table_local);
            void op_adicao();
            void termo(compiler::SymbolTable &symbol_table_local);
            void termo_opc(compiler::SymbolTable &symbol_table_local);
            void op_mult();
            void fator(compiler::SymbolTable &symbol_table_local);
            void chamada_funcao(compiler::SymbolTable &symbol_table_local, std::string &id_lexeme);
            void lista_args(compiler::SymbolTable &symbol_table_local, std::vector<std::string> &arguments);
            void lista_args2(compiler::SymbolTable &symbol_table_local, std::vector<std::string> &arguments);
            void arg(compiler::SymbolTable &symbol_table_local, std::vector<std::string> &arguments);
    };
}

#endif