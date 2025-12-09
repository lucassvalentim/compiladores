#include "../include/asa.hpp"
#include <iostream>

namespace compiler {

    namespace auxiliary_printing_function{
        inline void print_indent(int indent) {
            for(int i = 0; i < indent; i++) std::cout << "  ";
        }

        inline std::string data_type_to_string(compiler::DataTypeASA type) {
            switch(type) {
                case compiler::DataTypeASA::INT: return "int";
                case compiler::DataTypeASA::FLOAT: return "float";
                case compiler::DataTypeASA::CHAR: return "char";
                case compiler::DataTypeASA::VOID: return "void";
                default: return "undefined";
            }
        }
    }

    // Comandos do ASTNode
    NodeType compiler::ASTNode::get_node_type() { return node_type; }
    DataTypeASA compiler::ASTNode::get_date_type() { return data_type; }
    void compiler::ASTNode::set_data_type(DataTypeASA type) { data_type = type; }

    void compiler::ASTNode::add_child(std::shared_ptr<ASTNode> child) {
        childrens.push_back(child);
    }
    const std::vector<std::shared_ptr<ASTNode>>& compiler::ASTNode::get_childrens() const {
        return childrens;
    }
    std::shared_ptr<ASTNode> compiler::ASTNode::get_child(size_t index) const {
        if(index < childrens.size()) return childrens[index];
        return nullptr;
    }

    // Comandos do FunctionNode
    const std::string& compiler::FunctionNode::get_function_name () const { return function_name; }
    void compiler::FunctionNode::set_block(std::shared_ptr<ASTNode> block){
        childrens.clear();
        add_child(block);
    }
    void compiler::FunctionNode::print(int indent) const {
        auxiliary_printing_function::print_indent(indent);
        std::cout << "┌─ FUNCTION: " << function_name << "\n";
        auxiliary_printing_function::print_indent(indent);
        std::cout << "│  Tipo retorno: " << auxiliary_printing_function::data_type_to_string(data_type) << "\n";
        if(!childrens.empty()) {
            childrens[0]->print(indent + 1);
        }
        auxiliary_printing_function::print_indent(indent);
        std::cout << "└─ END FUNCTION\n";
    }

    // Comandos do o BlockNode
    void compiler::BlockNode::add_statement(std::shared_ptr<ASTNode> statement){
        add_child(statement);
    }
    void compiler::BlockNode::print(int indent) const {
        auxiliary_printing_function::print_indent(indent);
        std::cout << "┌─ BLOCK (";
        std::cout << childrens.size() << " comandos)\n";
        for(size_t i = 0; i < childrens.size(); i++) {
            auxiliary_printing_function::print_indent(indent);
            std::cout << "│  [" << (i+1) << "]\n";
            childrens[i]->print(indent + 1);
        }
        auxiliary_printing_function::print_indent(indent);
        std::cout << "└─ END BLOCK\n";
    }

    // Comando do no de atribuicao
    void compiler::AssignNode::set_assignment(std::shared_ptr<ASTNode> id, std::shared_ptr<ASTNode> expr) {
        childrens.clear();
        add_child(id);
        add_child(expr);
    }
    std::shared_ptr<ASTNode> compiler::AssignNode::get_id() const { 
        return childrens[0]; 
    }
    std::shared_ptr<ASTNode> compiler::AssignNode::get_expression() const { 
        return childrens[1];
    }
    void compiler::AssignNode::print(int indent) const {
        auxiliary_printing_function::print_indent(indent);
        std::cout << "┌─ ASSIGN\n";
        if(childrens.size() >= 2) {
            auxiliary_printing_function::print_indent(indent);
            std::cout << "│  ID (esquerda):\n";
            childrens[0]->print(indent + 1);
            auxiliary_printing_function::print_indent(indent);
            std::cout << "│  Expressao (direita):\n";
            childrens[1]->print(indent + 1);
        }
        auxiliary_printing_function::print_indent(indent);
        std::cout << "└─ END ASSIGN\n";
    }

    // Comando dos no de expressao relacional
    const std::string& compiler::RelOp::get_operator() const {
        return op;
    }
    void compiler::RelOp::set_operands(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
        childrens.clear();
        add_child(left);
        add_child(right);
    }
    void compiler::RelOp::print(int indent) const {
        auxiliary_printing_function::print_indent(indent);
        std::cout << "┌─ REL_OP: " << op << "\n";
        if(childrens.size() >= 2) {
            auxiliary_printing_function::print_indent(indent);
            std::cout << "│  Operando esquerdo:\n";
            childrens[0]->print(indent + 1);
            auxiliary_printing_function::print_indent(indent);
            std::cout << "│  Operando direito:\n";
            childrens[1]->print(indent + 1);
        }
        auxiliary_printing_function::print_indent(indent);
        std::cout << "└─ END REL_OP\n";
    }

    // Comando dos no de expressao aritmetica
    const std::string& compiler::AritOp::get_operator() const {
        return op;
    }
    void compiler::AritOp::set_operands(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right) {
        childrens.clear();
        add_child(left);
        add_child(right);
    }
    void compiler::AritOp::print(int indent) const {
        auxiliary_printing_function::print_indent(indent);
        std::cout << "┌─ ARIT_OP: " << op << "\n";
        if(childrens.size() >= 2) {
            auxiliary_printing_function::print_indent(indent);
            std::cout << "│  Operando esquerdo:\n";
            childrens[0]->print(indent + 1);
            auxiliary_printing_function::print_indent(indent);
            std::cout << "│  Operando direito:\n";
            childrens[1]->print(indent + 1);
        }
        auxiliary_printing_function::print_indent(indent);
        std::cout << "└─ END ARIT_OP\n";
    }

    // Comando da chamada de funcao
    const std::string& compiler::CallNode::get_name() const { 
        return function_name;
    }        
    void compiler::CallNode::add_argument(std::shared_ptr<ASTNode> arg) {
        add_child(arg);
    }
    size_t compiler::CallNode::num_arguments() const { 
        return childrens.size(); 
    }
    void compiler::CallNode::print(int indent) const {
        auxiliary_printing_function::print_indent(indent);
        std::cout << "┌─ CALL: " << function_name << "\n";
        auxiliary_printing_function::print_indent(indent);
        std::cout << "│  Numero de argumentos: " << childrens.size() << "\n";
        for(size_t i = 0; i < childrens.size(); i++) {
            auxiliary_printing_function::print_indent(indent);
            std::cout << "│  Arg[" << i << "]:\n";
            childrens[i]->print(indent + 1);
        }
        auxiliary_printing_function::print_indent(indent);
        std::cout << "└─ END CALL\n";
    }

    // Comandos dos no's folhas
    const std::string& compiler::IdNode::get_name() const {
        return name;
    }
    void compiler::IdNode::print(int indent) const {
        auxiliary_printing_function::print_indent(indent);
        std::cout << "● ID: " << name;
        std::cout << " (tipo: " << auxiliary_printing_function::data_type_to_string(data_type) << ")\n";
    }

    int compiler::IntConstNode::get_value() const {
        return value;
    }
    void compiler::IntConstNode::print(int indent) const {
        auxiliary_printing_function::print_indent(indent);
        std::cout << "● INT_CONST: " << value << "\n";
    }

    float compiler::FloatConstNode::get_value() const {
        return value;
    }
    void compiler::FloatConstNode::print(int indent) const {
        auxiliary_printing_function::print_indent(indent);
        std::cout << "● FLOAT_CONST: " << value << "\n";
    }

    char compiler::CharConstNode::get_value() const {
        return value;
    }
    void compiler::CharConstNode::print(int indent) const {
        auxiliary_printing_function::print_indent(indent);
        std::cout << "● CHAR_CONST: '" << value << "'\n";
    }

}