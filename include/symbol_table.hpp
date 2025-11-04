#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include <vector>
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

    struct SymbolTable{
        std::string name;
        DataTypes data_type;
        bool is_parameter;
        size_t parameter_position;
        std::vector<FunctionRegister> call_refs;
        DataTypes return_type;

        // Construtores
        SymbolTable();
        SymbolTable(std::string n, 
        DataTypes dt, 
        bool is_p, 
        size_t param_pos,
        DataTypes ret = DataTypes::VOID);
        
        // Funções auxiliares
        void add_call_ref(const FunctionRegister &call);
        void set_return_type(DataTypes rt);
        DataTypes get_return_type() const;
    };

    class SymbolTableList{
        public:
            void insert_data_table(const SymbolTable &t, std::string name_function);
            bool get_data_table(const std::string &lexema, SymbolTable &t) const;
            std::unordered_map<std::string, SymbolTable> get_all();
        private:
            std::unordered_map<std::string, SymbolTable> symbol_table_list;
    };
}

#endif