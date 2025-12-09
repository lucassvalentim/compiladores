#include "../include/symbol_table.hpp"

namespace compiler {

    bool SymbolTable::insert(SymbolEntry &&entry) {
         // Primeiro tenta inserir normalmente
        auto [it, inserted] = symbols_.try_emplace(entry.name, std::move(entry));
        
        if (!inserted) {
            SymbolEntry && entry_temp = std::move(entry);
            symbols_.erase(it);
            symbols_.emplace(entry_temp.name, std::move(entry_temp));
        }
        
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
        tables_[scope_name] = std::move(table);
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
