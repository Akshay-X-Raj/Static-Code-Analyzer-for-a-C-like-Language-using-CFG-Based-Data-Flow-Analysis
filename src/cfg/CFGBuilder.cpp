#include "CFGBuilder.h"

std::vector<BasicBlock> cfg;
int blockCounter = 0;

int createBlock(std::string stmt) {
    BasicBlock b;
    b.id = blockCounter + 1;
    blockCounter = blockCounter + 1;
    b.statements.push_back(stmt);
    cfg.push_back(b);
    return b.id;
}

std::string CFGGenerator::exprToString(Expr* expr) {
    if (!expr) return "";
    if (auto p = dynamic_cast<PrimaryExpr*>(expr)) return p->value.value;
    if (auto u = dynamic_cast<UnaryExpr*>(expr)) return u->op.value + exprToString(u->right.get());
    if (auto b = dynamic_cast<BinaryExpr*>(expr)) return exprToString(b->left.get()) + " " + b->op.value + " " + exprToString(b->right.get());
    return "";
}

void CFGGenerator::generate(ProgramNode* prog) {
    for (auto& fn : prog->functions) {
        visitFunction(fn.get());
    }
}

void CFGGenerator::visitFunction(FunctionNode* fn) {
    int entryBlock = createBlock("Func " + fn->name.value);
    int exitBlock = createBlock("EndFunc " + fn->name.value);
    
    int currentBlock = entryBlock;
    
    if (fn->body) {
        for (auto& stmt : fn->body->statements) {
            visitStmt(stmt.get(), currentBlock, exitBlock);
        }
    }
    
    // ensure last block connects to exit block unless it already returned
    bool hasExit = false;
    for (int nextId : cfg[currentBlock-1].next) {
        if (nextId == exitBlock) hasExit = true;
    }
    if (!hasExit && currentBlock != exitBlock) {
        cfg[currentBlock-1].next.push_back(exitBlock);
    }
}

void CFGGenerator::visitStmt(Stmt* stmt, int& currentBlock, int exitBlock) {
    if (auto decl = dynamic_cast<DeclarationStmt*>(stmt)) {
        int b = createBlock(decl->type.value + " " + decl->name.value);
        cfg[currentBlock-1].next.push_back(b);
        currentBlock = b;
    } else if (auto asgn = dynamic_cast<AssignmentStmt*>(stmt)) {
        int b = createBlock(asgn->name.value + " = " + exprToString(asgn->value.get()));
        cfg[currentBlock-1].next.push_back(b);
        currentBlock = b;
    } else if (auto ifS = dynamic_cast<IfStmt*>(stmt)) {
        int condB = createBlock("if (" + exprToString(ifS->condition.get()) + ")");
        cfg[currentBlock-1].next.push_back(condB);
        
        int endIf = createBlock("END_IF");
        
        int thenCurrent = condB;
        visitStmt(ifS->thenBranch.get(), thenCurrent, exitBlock);
        cfg[thenCurrent-1].next.push_back(endIf);
        
        if (ifS->elseBranch) {
            int elseCurrent = condB;
            visitStmt(ifS->elseBranch.get(), elseCurrent, exitBlock);
            cfg[elseCurrent-1].next.push_back(endIf);
        } else {
            cfg[condB-1].next.push_back(endIf);
        }
        
        currentBlock = endIf;
    } else if (auto whl = dynamic_cast<WhileStmt*>(stmt)) {
        int condB = createBlock("while (" + exprToString(whl->condition.get()) + ")");
        cfg[currentBlock-1].next.push_back(condB);
        
        int endWhl = createBlock("END_WHILE");
        
        int bodyCurrent = condB;
        visitStmt(whl->body.get(), bodyCurrent, exitBlock);
        cfg[bodyCurrent-1].next.push_back(condB); // loop back
        
        cfg[condB-1].next.push_back(endWhl); // exit loop
        currentBlock = endWhl;
    } else if (auto ret = dynamic_cast<ReturnStmt*>(stmt)) {
        int b = createBlock("return " + exprToString(ret->value.get()));
        cfg[currentBlock-1].next.push_back(b);
        cfg[b-1].next.push_back(exitBlock);
        currentBlock = b; // code after this in block is effectively unreachable in simple CFG
    } else if (auto blk = dynamic_cast<BlockStmt*>(stmt)) {
        for (auto& s : blk->statements) {
            visitStmt(s.get(), currentBlock, exitBlock);
        }
    } else if (auto forS = dynamic_cast<ForStmt*>(stmt)) {
        // Init
        if (forS->initializer) visitStmt(forS->initializer.get(), currentBlock, exitBlock);
        
        int condB = createBlock("for_cond (" + exprToString(forS->condition.get()) + ")");
        cfg[currentBlock-1].next.push_back(condB);
        
        int endFor = createBlock("END_FOR");
        
        int bodyCurr = condB;
        visitStmt(forS->body.get(), bodyCurr, exitBlock);
        
        if (forS->update) visitStmt(forS->update.get(), bodyCurr, exitBlock);
        cfg[bodyCurr-1].next.push_back(condB); // Loop back
        
        cfg[condB-1].next.push_back(endFor);
        currentBlock = endFor;
    }
}
