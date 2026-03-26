#pragma once
#include <string>

using namespace std;

class ASTNode {
public:
    string value;
    ASTNode* left;
    ASTNode* right;

    // 🔥 NEW: for IF / WHILE body
    ASTNode* body;

    ASTNode(string val) {
        value = val;
        left = NULL;
        right = NULL;
        body = NULL;
    }
};