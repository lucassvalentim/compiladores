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
            void print_vecotr();
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
            void lista_params();
            void lista_params2();
            void tipo_retorno_funcao();
            void bloco();
            void sequencia();
            void declaracao();
            void var_list();
            void var_list2();
            void type();
            void comando();
            void atribuicao_ou_chamada();
            void comando_se();
            void comando_senao();
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