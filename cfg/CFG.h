#pragma once
#include <bits/stdc++.h>
#include "../ast/ASTNode.h"

using namespace std;

class CFGNode {
public:
    int id;//node number
    string label;//what the node represent
    vector<CFGNode*> next;//where control goes next

    CFGNode(int i, string l) {
        id = i;
        label = l;
    }
};

class CFG {
    int nodeCount;

public:
    CFG();

    CFGNode* build(ASTNode* root);//convert ast to cfg
    void generateDOT(CFGNode* start);//create dot file for graphviz
};