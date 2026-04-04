#pragma once

#include <vector>
#include "../utils/Token.h"
#include "../ast/ASTNode.h"

using namespace std;

class Parser {
    vector<Token> tokens; //input from lexer
    int pos;//current position

public:
    Parser(vector<Token> t);

    ASTNode* parseProgram();

private:
    Token peek();
    Token get();
    bool match(TokenType type);
//some inportant functions of parser
    ASTNode* parseStmt(); //decides which statement
    ASTNode* parseDecl();//int x
    ASTNode* parseAssign(); //x=5
    ASTNode* parseIf();//if else
    ASTNode* parsePrint();//printf

    ASTNode* parseExpr();//expression
    ASTNode* parseTerm();
    ASTNode* parseFactor();

    ASTNode* parseCondition();//a>b
};