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

            void match(compiler::TokenType tok);
            void programa();
            void funcao_seq();
            void funcao();
            void nome_funcao();
            std::optional<std::vector<std::pair<std::string, compiler::DataType>>> lista_params();
            void lista_params2(std::vector<std::pair<std::string, compiler::DataType>> &list_ids);
            compiler::DataType tipo_retorno_funcao();
            void bloco(compiler::SymbolTable &st);
            void sequencia(compiler::SymbolTable &st);
            void declaracao(compiler::SymbolTable &st);
            std::optional<std::vector<std::string>> var_list();
            void var_list2(std::vector<std::string> &list_ids);
            compiler::DataType type();
            void comando(compiler::SymbolTable &st);
            void atribuicao_ou_chamada(compiler::SymbolTable &st);
            void comando_se(compiler::SymbolTable &st);
            void comando_senao(compiler::SymbolTable &st);
            void expr(compiler::SymbolTable &st);
            void expr_opc(compiler::SymbolTable &st);
            void op_igual();
            void rel(compiler::SymbolTable &st);
            void rel_opc(compiler::SymbolTable &st);
            void op_rel();
            void adicao(compiler::SymbolTable &st);
            void adicao_opc(compiler::SymbolTable &st);
            void op_adicao();
            void termo(compiler::SymbolTable &st);
            void termo_opc(compiler::SymbolTable &st);
            void op_mult();
            void fator(compiler::SymbolTable &st);
            void chamada_funcao(compiler::SymbolTable &st);
            void lista_args(compiler::SymbolTable &st);
            void lista_args2(compiler::SymbolTable &st);
            void arg(compiler::SymbolTable &st);
    };
}

#endif