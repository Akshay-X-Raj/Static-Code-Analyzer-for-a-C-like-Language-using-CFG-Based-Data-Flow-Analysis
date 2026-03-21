#include "SemanticAnalyzer.h"
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer() {
    currentScope = new Scope(nullptr);
}

SemanticAnalyzer::~SemanticAnalyzer() {
    delete currentScope;
}

void SemanticAnalyzer::analyze(ProgramNode* prog) {
    for (auto& fn : prog->functions) {
        visitFunction(fn.get());
    }
}

void SemanticAnalyzer::visitFunction(FunctionNode* fn) {
    Symbol s; s.name = fn->name.value; s.initialized = true;
    currentScope->defineSymbol(fn->name.value, s);
    
    Scope* prevScope = currentScope;
    currentScope = new Scope(prevScope);
    
    if (fn->body) visitStmt(fn->body.get());
    
    Scope* toDelete = currentScope;
    currentScope = prevScope;
    delete toDelete;
}

void SemanticAnalyzer::visitStmt(Stmt* stmt) {
    if (!stmt) return;
    if (auto decl = dynamic_cast<DeclarationStmt*>(stmt)) {
        Symbol s; s.name = decl->name.value; s.initialized = false;
        currentScope->defineSymbol(decl->name.value, s);
    } else if (auto asgn = dynamic_cast<AssignmentStmt*>(stmt)) {
        Symbol* s = currentScope->resolveSymbol(asgn->name.value);
        if (!s) {
            std::cerr << "[Semantic Error] Use of undeclared variable: " << asgn->name.value << std::endl;
        } else {
            s->initialized = true;
        }
    } else if (auto blk = dynamic_cast<BlockStmt*>(stmt)) {
        Scope* prevScope = currentScope;
        currentScope = new Scope(prevScope);
        
        for (auto& s : blk->statements) {
            visitStmt(s.get());
        }
        
        Scope* toDelete = currentScope;
        currentScope = prevScope;
        delete toDelete;
    } else if (auto ifS = dynamic_cast<IfStmt*>(stmt)) {
        visitStmt(ifS->thenBranch.get());
        if (ifS->elseBranch) visitStmt(ifS->elseBranch.get());
    } else if (auto whl = dynamic_cast<WhileStmt*>(stmt)) {
        visitStmt(whl->body.get());
    } else if (auto forS = dynamic_cast<ForStmt*>(stmt)) {
        Scope* prevScope = currentScope;
        currentScope = new Scope(prevScope);
        
        visitStmt(forS->initializer.get());
        visitStmt(forS->update.get());
        visitStmt(forS->body.get());
        
        Scope* toDelete = currentScope;
        currentScope = prevScope;
        delete toDelete;
    }
}
