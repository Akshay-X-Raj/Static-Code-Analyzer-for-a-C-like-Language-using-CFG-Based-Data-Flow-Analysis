#pragma once

#include <vector>
#include "../utils/Token.h"
#include "../ast/ASTNode.h"

using namespace std;

class Parser {
    vector<Token> tokens;
    int pos;

public:
    Parser(vector<Token> t);

    ASTNode* parseProgram();

private:
    Token peek();
    Token get();
    bool match(TokenType type);

    // ---------- STATEMENTS ----------
    ASTNode* parseStmt();
    ASTNode* parseDecl();
    ASTNode* parseAssign();
    ASTNode* parseIf();
    ASTNode* parsePrint();
    ASTNode* parseWhile();
    ASTNode* parseBlock();   

    // ---------- EXPRESSIONS ----------
    ASTNode* parseExpr();
    ASTNode* parseTerm();
    ASTNode* parseFactor();
    ASTNode* parseCondition();
};