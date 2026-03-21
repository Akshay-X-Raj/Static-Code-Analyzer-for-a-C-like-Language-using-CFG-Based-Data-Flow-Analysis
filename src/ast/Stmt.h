#pragma once
#include "ASTNode.h"
#include "Expr.h"
#include "../parser/Token.h"
#include <vector>
#include <memory>

class Stmt : public ASTNode {
public:
    virtual ~Stmt() = default;
};

class BlockStmt : public Stmt {
public:
    std::vector<std::unique_ptr<Stmt>> statements;
    BlockStmt(std::vector<std::unique_ptr<Stmt>> stmts) : statements(std::move(stmts)) {}
};

class IfStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch; // can be null
    IfStmt(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> thenB, std::unique_ptr<Stmt> elseB)
        : condition(std::move(cond)), thenBranch(std::move(thenB)), elseBranch(std::move(elseB)) {}
};

class WhileStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
    WhileStmt(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> b)
        : condition(std::move(cond)), body(std::move(b)) {}
};

class ForStmt : public Stmt {
public:
    std::unique_ptr<Stmt> initializer; // DeclarationStmt or AssignmentStmt
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> update; // AssignmentStmt
    std::unique_ptr<Stmt> body;
    ForStmt(std::unique_ptr<Stmt> init, std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> up, std::unique_ptr<Stmt> b)
        : initializer(std::move(init)), condition(std::move(cond)), update(std::move(up)), body(std::move(b)) {}
};

class ReturnStmt : public Stmt {
public:
    std::unique_ptr<Expr> value;
    ReturnStmt(std::unique_ptr<Expr> val) : value(std::move(val)) {}
};

class DeclarationStmt : public Stmt {
public:
    Token type;
    Token name;
    DeclarationStmt(Token t, Token n) : type(t), name(n) {}
};

class AssignmentStmt : public Stmt {
public:
    Token name;
    std::unique_ptr<Expr> value;
    AssignmentStmt(Token n, std::unique_ptr<Expr> val) : name(n), value(std::move(val)) {}
};

class FunctionNode : public ASTNode {
public:
    Token type;
    Token name;
    std::unique_ptr<BlockStmt> body;
    FunctionNode(Token t, Token n, std::unique_ptr<BlockStmt> b) : type(t), name(n), body(std::move(b)) {}
};

class ProgramNode : public ASTNode {
public:
    std::vector<std::unique_ptr<FunctionNode>> functions;
};
