#pragma once
#include <bits/stdc++.h>
using namespace std;

class SymbolTable {
public:
    map<string, string> table;

    // ---------- ADD ----------
    void add(string name, string type) {
        if (table.find(name) != table.end()) {
            cout << "Semantic Error: Variable '" << name << "' already declared\n";
            exit(1);
        }
        table[name] = type;
    }

    // ---------- EXISTS ----------
    bool exists(string name) {
        return table.find(name) != table.end();
    }

    // ---------- GET TYPE ----------
    string getType(string name) {
        if (!exists(name)) {
            cout << "Semantic Error: Variable '" << name << "' not declared\n";
            exit(1);
        }
        return table[name];
    }

    // ---------- CHECK USE ----------
    void checkUse(string name) {
        if (!exists(name)) {
            cout << "Semantic Error: Variable '" << name << "' used before declaration\n";
            exit(1);
        }
    }

    // ---------- TYPE COMPATIBILITY ----------
    bool isCompatible(string varType, string exprType) {

        if (varType == exprType) return true;

        // int → float/double allowed
        if ((varType == "float" || varType == "double") && exprType == "int")
            return true;


        return false;
    }

    // ---------- CHECK TYPE ----------
    void checkType(string var, string exprType) {

        string varType = getType(var);

        if (!isCompatible(varType, exprType)) {
            cout << "Semantic Error: Type mismatch for variable '" 
                 << var << "' (" << varType << " = " << exprType << ")\n";
            exit(1);
        }
    }

    // ---------- PRINT ----------
    void print() {
        cout << "\n===== SYMBOL TABLE =====\n";
        for (auto it : table) {
            cout << it.first << " : " << it.second << endl;
        }
    }
};