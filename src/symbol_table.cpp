#include "../include/symbol_table.hpp"

namespace compiler {

    bool SymbolTable::insert(const SymbolEntry &entry) {
        // Verifica se o símbolo já existe
        if (symbols_.find(entry.name) != symbols_.end()) {
            return false; // já existe
        }
        symbols_[entry.name] = entry;
        return true;
    }

    std::optional<SymbolEntry> SymbolTable::find(const std::string &name) const {
        auto it = symbols_.find(name);
        if (it != symbols_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    bool SymbolTableList::insert_table(const std::string &scope_name, const SymbolTable &table) {
        // Verifica se já existe uma tabela com esse escopo
        if (tables_.find(scope_name) != tables_.end()) {
            return false; // já existe
        }
        tables_[scope_name] = table;
        return true;
    }

    std::optional<SymbolTable> SymbolTableList::find_table(const std::string &scope_name) const {
        auto it = tables_.find(scope_name);
        if (it != tables_.end()) {
            return it->second;
        }
        return std::nullopt;
    }
}
