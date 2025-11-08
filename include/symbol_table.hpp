#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include <vector>
#include <set>
#include <unordered_map>

namespace compiler {

    enum class DataTypes{
        ERROR = -1,
        INT = 0,
        FLOAT = 1,
        CHAR = 2,
        VOID = 3
    };

    struct FunctionRegister{
        std::string name_function;
        size_t number_arguments;
        std::vector<std::string> arguments;
        
        // Construtores
        FunctionRegister();
        FunctionRegister(std::string name_f, size_t num_args, const std::vector<std::string> &args);
    };

    struct DataTable{
        std::string name;
        DataTypes data_type;
        bool is_parameter;
        size_t parameter_position;
        std::vector<FunctionRegister> functions_calls;
        DataTypes return_type;

        // Construtores
        DataTable();
        DataTable(std::string n, 
        DataTypes dt, 
        bool is_p, 
        size_t param_pos);
        
        // Funções auxiliares
        void add_call_ref(const FunctionRegister &call);
    };

    class SymbolTable{
        public:
            void insert_data_table(const DataTable &t);
            bool get_data_table(std::string &lexema, DataTable &t) const;
            bool get_symbol_table(std::unordered_map<std::string, DataTable> &symbol_table) const;
        private:
            std::unordered_map<std::string, DataTable> symbol_table;
    };

    class SymbolTableList{
        public:
            void insert_data_table(const SymbolTable &t, std::string name_function);
            bool get_data_table(const std::string &lexema, SymbolTable &st) const;
            std::unordered_map<std::string, DataTable> get_all();
        private:
            std::unordered_map<std::string, SymbolTable> symbol_table_list;
    };
}

#endif