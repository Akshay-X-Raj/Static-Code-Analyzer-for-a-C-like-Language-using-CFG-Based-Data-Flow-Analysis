#pragma once
#include "../ast/Stmt.h"
#include "Scope.h"

class SemanticAnalyzer {
public:
    SemanticAnalyzer();
    ~SemanticAnalyzer();
    
    void analyze(ProgramNode* prog);
    
private:
    Scope* currentScope;
    void visitFunction(FunctionNode* fn);
    void visitStmt(Stmt* stmt);
};
