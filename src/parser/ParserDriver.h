#pragma once
#include <vector>
#include <memory>
#include "Token.h"
#include "../ast/Stmt.h"
#include "../ast/Expr.h"

using namespace std;

class ParserDriver {
    vector<Token> tokens;
    size_t pos = 0;

public:
    ParserDriver(vector<Token> t);
    
    // Helpers
    Token peek();
    Token get();
    bool match(TokenType type);
    Token consume(TokenType type, const string& err);

    // Entry point
    unique_ptr<ProgramNode> parseProgram();

    // Declarations and Statements
    unique_ptr<FunctionNode> parseFunction();
    unique_ptr<BlockStmt> parseBlock();
    unique_ptr<Stmt> parseStatement();
    unique_ptr<Stmt> parseDeclaration();
    unique_ptr<Stmt> parseAssignment();
    unique_ptr<Stmt> parseIfStatement();
    unique_ptr<Stmt> parseWhileStatement();
    unique_ptr<Stmt> parseForStatement();
    unique_ptr<Stmt> parseReturnStatement();
    bool isTypeToken(TokenType t);

    // Expressions
    unique_ptr<Expr> parseExpression();
    unique_ptr<Expr> parseLogicalOrExpr();
    unique_ptr<Expr> parseLogicalAndExpr();
    unique_ptr<Expr> parseEqualityExpr();
    unique_ptr<Expr> parseRelationalExpr();
    unique_ptr<Expr> parseAdditiveExpr();
    unique_ptr<Expr> parseMultiplicativeExpr();
    unique_ptr<Expr> parseUnaryExpr();
    unique_ptr<Expr> parsePrimary();
};
