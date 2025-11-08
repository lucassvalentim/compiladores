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
        bool is_parameter = false;
        size_t parameter_position = 0;
        DataType return_type;
        std::vector<FunctionRegister> calls;

        SymbolEntry() = default;
        SymbolEntry(const std::string &n, 
                    DataType dt, 
                    bool is_p = false, 
                    size_t param_pos = 0)
            : name(n), type(dt), is_parameter(is_p), parameter_position(param_pos) {}

        void add_function_call(const FunctionRegister &call) {
            calls.push_back(call);
        }
    };

    class SymbolTable {
    public:
        bool insert(const SymbolEntry &entry);
        std::optional<SymbolEntry> find(const std::string &name) const;
        const std::unordered_map<std::string, SymbolEntry>& get_all() const { return symbols_; }
    private:
        std::unordered_map<std::string, SymbolEntry> symbols_;
    };

    class SymbolTableList {
    public:
        bool insert_table(const std::string &scope_name, const SymbolTable &table);
        std::optional<SymbolTable> find_table(const std::string &scope_name) const;
        const std::unordered_map<std::string, SymbolTable>& get_all() const { return tables_; }
    private:
        std::unordered_map<std::string, SymbolTable> tables_;
    };

}

#endif
