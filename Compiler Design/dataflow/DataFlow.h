#pragma once
#include <set>
#include <string>
#include "../ast/ASTNode.h"

using namespace std;

class DataFlow {
public:
    set<string> defined;
    set<string> used;

    void analyze(ASTNode* root);
    void report();
};