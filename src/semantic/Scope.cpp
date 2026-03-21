#include "Scope.h"

Scope::Scope(Scope* parent) : parentScope(parent) {}

void Scope::defineSymbol(const std::string& name, const Symbol& sym) {
    localSymbols[name] = sym;
}

Symbol* Scope::resolveSymbol(const std::string& name) {
    if (localSymbols.count(name) > 0) {
        return &localSymbols[name];
    }
    if (parentScope != nullptr) {
        return parentScope->resolveSymbol(name);
    }
    return nullptr;
}
