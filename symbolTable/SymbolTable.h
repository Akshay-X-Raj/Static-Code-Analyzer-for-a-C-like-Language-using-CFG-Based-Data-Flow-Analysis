#pragma once
#include <bits/stdc++.h>
using namespace std;

class SymbolTable {
public:
    map<string, string> table;

    void add(string name, string type) {
        if (table.find(name) != table.end()) {
            cout << "Semantic Error: Variable '" << name << "' already declared\n";
            exit(1);
        }
        table[name] = type;
    }

    bool exists(string name) {
        return table.find(name) != table.end();
    }

    string getType(string name) {
        if (!exists(name)) {
            cout << "Semantic Error: Variable '" << name << "' not declared\n";
            exit(1);
        }
        return table[name];
    }

    void checkUse(string name) {
        if (!exists(name)) {
            cout << "Semantic Error: Variable '" << name << "' used before declaration\n";
            exit(1);
        }
    }

void checkType(string var, string exprType) {

    if (!exists(var)) {
        cout << "Semantic Error: Variable '" << var << "' not declared\n";
        exit(1);
    }

    string varType = table[var];

    //  SAME TYPE
    if (varType == exprType) return;

    // int → float/double
    if ((varType == "float" || varType == "double") && exprType == "int")
        return;

    // int → char (ALLOW THIS)
    if (varType == "char" && exprType == "int")
        return;

    //  OTHERWISE ERROR
    cout << "Semantic Error: Type mismatch for variable '" << var << "'\n";
    exit(1);
}

    void print() {
        cout << "\n===== SYMBOL TABLE =====\n";
        for (auto it : table) {
            cout << it.first << " : " << it.second << endl;
        }
    }
};