#pragma once
#include "ASTNode.h"
#include "Expr.h"
#include "Stmt.h"

class ASTBuilder {
public:
    ASTBuilder();
    ASTNode* buildPlaceholder();
};
