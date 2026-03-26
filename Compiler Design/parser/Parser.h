#pragma once
#include <vector>
#include "../utils/Token.h"
#include "../ast/ASTNode.h"

using namespace std;

class Parser {
private:
    vector<Token> tokens;
    int pos;

public:
    Parser(vector<Token> t);

    // 🔥 New functions
    ASTNode* parseStatement();
    vector<ASTNode*> parseProgram();

    // Utility
    Token peek();
    Token get();
};