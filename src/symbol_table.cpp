#include <string>
#include <vector>
#include "../include/symbol_table.hpp"

namespace compiler {
    // Implementações dos construtores
    compiler::FunctionRegister::FunctionRegister() = default;
    compiler::FunctionRegister::FunctionRegister(std::string name_f, size_t num_args, const std::vector<std::string> &args)
    : name_function(name_f), number_arguments(num_args), arguments(args) {}

    compiler::DataTable::DataTable() = default;
    compiler::DataTable::DataTable(std::string n, DataTypes dt, bool is_p, size_t param_pos)
    : name(n), data_type(dt), is_parameter(is_p), parameter_position(param_pos) {}

    // Implementações das funções auxiliares do DataTable
    void compiler::DataTable::add_call_ref(const FunctionRegister &call) {
        functions_calls.push_back(call);
    }

    // Implementações das funções auxiliares do SymbolTableList
    void compiler::SymbolTable::insert_data_table(const DataTable &t) {
        symbol_table[t.name] = t;
    }
    bool compiler::SymbolTable::get_data_table(std::string &lexema, DataTable &t) const {
        auto it = symbol_table.find(lexema);
        if (it != symbol_table.end()) {
            t = it->second;
            return true;
        }
        return false;
    }
    bool compiler::SymbolTable::get_symbol_table(std::unordered_map<std::string, DataTable> &symbol_table) const{
        symbol_table = this->symbol_table;
        return true;
    }
}