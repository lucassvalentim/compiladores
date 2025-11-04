#include <string>
#include <vector>
#include "../include/symbol_table.hpp"

namespace compiler {
    // Implementações dos construtores
    compiler::FunctionRegister::FunctionRegister() = default;
    compiler::FunctionRegister::FunctionRegister(std::string name_f, size_t num_args, const std::vector<std::string> &args)
    : name_function(name_f), number_arguments(num_args), arguments(args) {}

    compiler::SymbolTable::SymbolTable() = default;
    compiler::SymbolTable::SymbolTable(std::string n, DataTypes dt, bool is_p, size_t param_pos, DataTypes ret)
    : name(n), data_type(dt), is_parameter(is_p), parameter_position(param_pos), return_type(ret) {}

    // Implementações das funções auxiliares do SymbolTable
    void compiler::SymbolTable::add_call_ref(const FunctionRegister &call) {
        call_refs.push_back(call);
    }
    void compiler::SymbolTable::set_return_type(DataTypes rt) { 
        return_type = rt; 
    }
    DataTypes compiler::SymbolTable::get_return_type() const { 
        return return_type; 
    }

    // Implementações das funções auxiliares do SymbolTableList
    void compiler::SymbolTableList::insert_data_table(const SymbolTable &t, std::string name_function) {
        symbol_table_list[name_function] = t;
    }
    bool compiler::SymbolTableList::get_data_table(const std::string &lexema, SymbolTable &t) const {
        auto it = symbol_table_list.find(lexema);
        if (it != symbol_table_list.end()) {
            t = it->second;
            return true;
        }
        return false;
    }
    std::unordered_map<std::string, SymbolTable> compiler::SymbolTableList::get_all(){
        return this->symbol_table_list;
    }

}