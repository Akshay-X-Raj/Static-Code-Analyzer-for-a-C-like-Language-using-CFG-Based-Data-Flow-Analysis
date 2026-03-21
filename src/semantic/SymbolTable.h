#pragma once
#include <string>
#include <map>

using namespace std;

struct Symbol {
    string name;
    bool initialized = false;
};

extern map<string, Symbol> symbolTable;
