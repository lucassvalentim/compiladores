#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

namespace compiler {
    enum class DataType {
        ERROR = -1,
        INT = 0,
        FLOAT,
        CHAR,
        VOID
    };

    struct FunctionRegister {
        std::string name;
        std::vector<std::string> arguments; // nomes dos argumentos (ou tipos, dependendo do uso)
        
        FunctionRegister() = default;
        FunctionRegister(const std::string &name_f, const std::vector<std::string> &args)
            : name(name_f), arguments(args) {}

        size_t num_args() const { return arguments.size(); }
    };
        
    struct SymbolEntry {
        std::string name;
        DataType type;
        bool is_parameter;
        int parameter_position;
        std::vector<FunctionRegister> calls;
        
        // Construtor
        SymbolEntry(std::string n, DataType t, bool is_param, int param_pos)
        : name(std::move(n)), type(t), is_parameter(is_param), parameter_position(param_pos) {}
        
        // Construtor de cópia
        SymbolEntry(const SymbolEntry&) = default;
        
        // Operador de atribuição por cópia
        SymbolEntry& operator=(const SymbolEntry&) = default;
        
        // Construtor de movimento
        SymbolEntry(SymbolEntry&& other) noexcept
        : name(std::move(other.name)),
        type(other.type),
        is_parameter(other.is_parameter),
        parameter_position(other.parameter_position),
        calls(other.calls) {}
        
        // Operador de atribuição por movimento
        SymbolEntry& operator=(SymbolEntry&& other) noexcept {
            if (this != &other) {
                name = std::move(other.name);
                type = other.type;
                is_parameter = other.is_parameter;
                parameter_position = other.parameter_position;
            }
            return *this;
        }
        
        void add_function_call(const FunctionRegister &call) {
            calls.push_back(call);
        }

        // Destrutor
        ~SymbolEntry() = default;
    };

    class SymbolTable {
        public:
            // Construtor padrão
            SymbolTable() : return_type(DataType::ERROR) {}
            
            // Construtor de cópia
            SymbolTable(const SymbolTable& other) 
                : symbols_(other.symbols_), return_type(other.return_type) {}
                
            
            // Operador de atribuição
            SymbolTable& operator=(const SymbolTable& other) {
                if (this != &other) {
                    symbols_ = other.symbols_;
                    return_type = other.return_type;
                }
                return *this;
            }
            
            // Construtor de movimento
            SymbolTable(SymbolTable&& other) noexcept
                : symbols_(std::move(other.symbols_)), return_type(std::move(other.return_type)) {}
            
            // Operador de atribuição por movimento
            SymbolTable& operator=(SymbolTable&& other) noexcept {
                if (this != &other) {
                    symbols_ = std::move(other.symbols_);
                    return_type = std::move(other.return_type);
                }
                return *this;
            }
            
            // Destrutor
            ~SymbolTable() = default;

            bool insert(SymbolEntry &&entry);
            std::optional<SymbolEntry> find(const std::string &name) const;
            const std::unordered_map<std::string, SymbolEntry>& get_all() const { return symbols_; }
            void set_return_type(DataType rt) {return_type = rt;}
            compiler::DataType get_return_type() {return return_type;}
        private:
            std::unordered_map<std::string, SymbolEntry> symbols_;
            DataType return_type;
    };

    class SymbolTableList {
        public:
            bool insert_table(const std::string &scope_name, SymbolTable &&table);
            std::optional<SymbolTable> find_table(const std::string &scope_name) const;
            const std::unordered_map<std::string, SymbolTable>& get_all() const { return tables_; }
        private:
            std::unordered_map<std::string, SymbolTable> tables_;
    };

}

#endif
