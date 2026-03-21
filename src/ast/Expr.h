#pragma once
#include "ASTNode.h"
#include "../parser/Token.h"
#include <memory>

class Expr : public ASTNode {
public:
    virtual ~Expr() = default;
};

class BinaryExpr : public Expr {
public:
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
    BinaryExpr(std::unique_ptr<Expr> l, Token o, std::unique_ptr<Expr> r) : left(std::move(l)), op(o), right(std::move(r)) {}
};

class UnaryExpr : public Expr {
public:
    Token op;
    std::unique_ptr<Expr> right;
    UnaryExpr(Token o, std::unique_ptr<Expr> r) : op(o), right(std::move(r)) {}
};

class PrimaryExpr : public Expr {
public:
    Token value;
    PrimaryExpr(Token v) : value(v) {}
};
