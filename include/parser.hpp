#ifndef PARSER_HPP
#define PARSER_HPP

#include "token.hpp"
#include <vector>
#include <stdbool.h>

namespace compiler{
    class Parser {
        public:
            Parser(std::vector<compiler::Token> &t);
            
            void print_vecotr();
            // int get_index();
            void initialize_parser();
        private:
            std::vector<compiler::Token> tokens;
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
            void lista_args();
    };
}

#endif