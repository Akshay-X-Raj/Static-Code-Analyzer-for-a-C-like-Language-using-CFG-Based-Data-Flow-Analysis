#pragma once

#include <bits/stdc++.h>
#include "../ast/ASTNode.h"
#include "../symbolTable/SymbolTable.h"  

using namespace std;

class CFGNode {
public:
    int id;
    string label;
    vector<CFGNode*> next;

    set<string> use;
    set<string> def;

    CFGNode(int i, string l) {
        id = i;
        label = l;
    }
};

class CFG {
    int nodeCount;

public:
    CFG();

    CFGNode* build(ASTNode* root);
    void generateDOT(CFGNode* start);
};

// 🔥 DECLARE FUNCTION HERE
void analyzeDataFlow(CFGNode* start);