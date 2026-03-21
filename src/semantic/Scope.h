#pragma once
#include <map>
#include <string>
#include "SymbolTable.h"

class Scope {
    Scope* parentScope;
    std::map<std::string, Symbol> localSymbols;

public:
    Scope(Scope* parent = nullptr);
    void defineSymbol(const std::string& name, const Symbol& sym);
    Symbol* resolveSymbol(const std::string& name);
};
