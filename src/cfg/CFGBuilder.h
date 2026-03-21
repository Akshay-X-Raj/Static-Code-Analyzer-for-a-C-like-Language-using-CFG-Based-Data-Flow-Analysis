#pragma once
#include <string>
#include "CFG.h"
#include "../ast/Stmt.h"

int createBlock(std::string stmt);

class CFGGenerator {
public:
    void generate(ProgramNode* prog);
private:
    void visitFunction(FunctionNode* fn);
    void visitStmt(Stmt* stmt, int& currentBlock, int exitBlock);
    
    // Simple helper to serialize an expression roughly
    std::string exprToString(Expr* expr);
};
