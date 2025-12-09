#include "../include/symbol_table.hpp"

namespace compiler {

    bool SymbolTable::insert(SymbolEntry &&entry) {
        // try_emplace com std::move
        auto [it, inserted] = symbols_.try_emplace(entry.name, std::move(entry));
        return inserted;
    }   

    std::optional<SymbolEntry> SymbolTable::find(const std::string &name) const {
        auto it = symbols_.find(name);
        if (it != symbols_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    bool SymbolTableList::insert_table(const std::string &scope_name, SymbolTable &&table) {
    if (tables_.find(scope_name) != tables_.end()) {
        return false;
    }
    tables_[scope_name] = std::move(table);  // Move em vez de copiar
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
