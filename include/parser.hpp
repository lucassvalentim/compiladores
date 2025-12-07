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
            void funcao_seq(compiler::SymbolTable &st);
            compiler::SymbolTable funcao();
            void nome_funcao();
            std::optional<std::vector<std::pair<std::string, compiler::DataType>>> lista_params();
            void lista_params2(std::vector<std::pair<std::string, compiler::DataType>> &list_ids);
            void tipo_retorno_funcao();
            void bloco(compiler::SymbolTable &st);
            void sequencia(compiler::SymbolTable &st);
            void declaracao(compiler::SymbolTable &st);
            std::optional<std::vector<std::string>> var_list();
            void var_list2(std::vector<std::string> &list_ids);
            compiler::DataType type();
            void comando(compiler::SymbolTable &st);
            void atribuicao_ou_chamada();
            void comando_se(compiler::SymbolTable &st);
            void comando_senao(compiler::SymbolTable &st);
            void expr();
            void expr_opc();
            void op_igual();
            void rel();
            void rel_opc();
            void op_rel();
            void adicao();
            void adicao_opc();
            void op_adicao();
            void termo();
            void termo_opc();
            void op_mult();
            void fator();
            void chamada_funcao();
            void lista_args();
            void lista_args2();
            void arg();
    };
}

#endif