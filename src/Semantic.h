/*
 * CONSOLIDATED SEMANTIC ANALYSIS
 * Combines: SemanticAnalyzer + Scope + SymbolTable
 * Handles variable tracking, scope management, and semantic checking
 */

#pragma once
#include "Parser.h"
#include <map>
#include <iostream>
using namespace std;

//============================================================
// SYMBOL & SYMBOL TABLE (was: SymbolTable.h)
//============================================================

struct Symbol {
    string name;
    bool initialized = false;
    string type;
};

// Global symbol table
map<string, Symbol> globalSymbolTable;

//============================================================
// SCOPE MANAGEMENT (was: Scope.h/Scope.cpp)
//============================================================

class Scope {
public:
    Scope* parentScope;
    map<string, Symbol> localSymbols;

public:
    Scope(Scope* parent = nullptr) : parentScope(parent) {}

    void defineSymbol(const string& name, const Symbol& sym) {
        localSymbols[name] = sym;
    }

    Symbol* resolveSymbol(const string& name) {
        if (localSymbols.find(name) != localSymbols.end()) {
            return &localSymbols[name];
        }
        if (parentScope != nullptr) {
            return parentScope->resolveSymbol(name);
        }
        return nullptr;
    }

    bool isDeclared(const string& name) {
        return resolveSymbol(name) != nullptr;
    }

    bool isInitialized(const string& name) {
        Symbol* sym = resolveSymbol(name);
        return sym != nullptr && sym->initialized;
    }

    void markInitialized(const string& name) {
        Symbol* sym = resolveSymbol(name);
        if (sym != nullptr) {
            sym->initialized = true;
        }
    }
};

//============================================================
// SEMANTIC ANALYZER (was: SemanticAnalyzer.h/SemanticAnalyzer.cpp)
//============================================================

class SemanticAnalyzer {
private:
    Scope* currentScope;
    map<string, bool> usedVariables;
    vector<string> warnings;

public:
    SemanticAnalyzer() : currentScope(nullptr) {}

    ~SemanticAnalyzer() {
        if (currentScope) delete currentScope;
    }

    void analyze(ProgramNode* program) {
        cout << "\nSemantic Analysis:\n";

        if (!program) return;

        for (auto& func : program->functions) {
            visitFunction(func.get());
        }

        // Print warnings
        if (warnings.empty()) {
            cout << "  [OK] No issues found\n";
        } else {
            cout << "  [!] Found " << warnings.size() << " warning(s):\n";
            for (const auto& w : warnings) {
                cout << "    " << w << "\n";
            }
        }
    }

    vector<string> getWarnings() const {
        return warnings;
    }

private:
    void visitFunction(FunctionNode* fn) {
        cout << "[->] Analyzing function: " << fn->name.value << "\n";

        // Create new scope for function
        Scope* prevScope = currentScope;
        currentScope = new Scope(nullptr);

        if (fn->body) {
            visitBlock(fn->body.get());
        }

        // Clean up scope
        delete currentScope;
        currentScope = prevScope;
    }

    void visitBlock(BlockStmt* block) {
        if (!block) return;
        for (auto& stmt : block->statements) {
            visitStmt(stmt.get());
        }
    }

    void visitStmt(Stmt* stmt) {
        if (!stmt) return;

        if (auto decl = dynamic_cast<DeclarationStmt*>(stmt)) {
            if (currentScope->isDeclared(decl->name.value)) {
                warnings.push_back("[!] Variable '" + decl->name.value + "' already declared");
            } else {
                Symbol sym;
                sym.name = decl->name.value;
                sym.type = decl->type.value;
                sym.initialized = false;
                currentScope->defineSymbol(sym.name, sym);
                cout << "  [OK] Variable declared: " << sym.name << "\n";
            }
        }
        else if (auto assign = dynamic_cast<AssignmentStmt*>(stmt)) {
            if (!currentScope->isDeclared(assign->name.value)) {
                warnings.push_back("[!] Variable '" + assign->name.value + "' not declared");
            } else {
                currentScope->markInitialized(assign->name.value);
                usedVariables[assign->name.value] = true;
                cout << "  [OK] Variable assigned: " << assign->name.value << "\n";
            }
            if (assign->value) {
                checkExpressionVariables(assign->value.get());
            }
        }
        else if (auto ifStmt = dynamic_cast<IfStmt*>(stmt)) {
            if (ifStmt->condition) {
                checkExpressionVariables(ifStmt->condition.get());
            }
            if (ifStmt->thenBranch) {
                visitStmt(ifStmt->thenBranch.get());
            }
            if (ifStmt->elseBranch) {
                visitStmt(ifStmt->elseBranch.get());
            }
        }
        else if (auto whileStmt = dynamic_cast<WhileStmt*>(stmt)) {
            if (whileStmt->condition) {
                checkExpressionVariables(whileStmt->condition.get());
            }
            if (whileStmt->body) {
                visitStmt(whileStmt->body.get());
            }
        }
        else if (auto forStmt = dynamic_cast<ForStmt*>(stmt)) {
            if (forStmt->initializer) {
                visitStmt(forStmt->initializer.get());
            }
            if (forStmt->condition) {
                checkExpressionVariables(forStmt->condition.get());
            }
            if (forStmt->update) {
                visitStmt(forStmt->update.get());
            }
            if (forStmt->body) {
                visitStmt(forStmt->body.get());
            }
        }
        else if (auto returnStmt = dynamic_cast<ReturnStmt*>(stmt)) {
            if (returnStmt->value) {
                checkExpressionVariables(returnStmt->value.get());
            }
        }
        else if (auto block = dynamic_cast<BlockStmt*>(stmt)) {
            visitBlock(block);
        }
    }

    void checkExpressionVariables(Expr* expr) {
        if (!expr) return;

        if (auto binExpr = dynamic_cast<BinaryExpr*>(expr)) {
            checkExpressionVariables(binExpr->left.get());
            checkExpressionVariables(binExpr->right.get());
        }
        else if (auto unaryExpr = dynamic_cast<UnaryExpr*>(expr)) {
            checkExpressionVariables(unaryExpr->right.get());
        }
        else if (auto primary = dynamic_cast<PrimaryExpr*>(expr)) {
            if (primary->value.type == ID) {
                if (!currentScope->isDeclared(primary->value.value)) {
                    warnings.push_back("[!] Variable '" + primary->value.value + "' not declared");
                } else if (!currentScope->isInitialized(primary->value.value)) {
                    warnings.push_back("[!] Variable '" + primary->value.value + "' may not be initialized");
                }
                usedVariables[primary->value.value] = true;
            }
        }
    }
};
