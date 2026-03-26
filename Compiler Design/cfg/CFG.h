#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "../ast/ASTNode.h"

using namespace std;

class CFGNode {
public:
    string value;
    vector<CFGNode*> next;

    CFGNode(string val) {
        value = val;
    }
};

class CFG {
public:
    CFGNode* start;

    CFG();

    void build(vector<ASTNode*> program);
    void print(CFGNode* node);

    void generateDOT(CFGNode* node, ofstream& out);
    void exportToDOT();
};