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
}