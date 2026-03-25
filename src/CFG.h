/*
 * CONSOLIDATED CONTROL FLOW GRAPH
 * Combines: BasicBlock + CFG + CFGBuilder + CFGPrinter
 * Builds and displays the control flow graph
 */

#pragma once
#include "Parser.h"
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

//============================================================
// BASIC BLOCK (was: BasicBlock.h)
//============================================================

class BasicBlock {
public:
    int id;
    vector<string> statements;
    vector<int> next;  // IDs of successor blocks

    BasicBlock(int i) : id(i) {}

    void addStatement(const string& stmt) {
        statements.push_back(stmt);
    }

    void addSuccessor(int blockId) {
        next.push_back(blockId);
    }

    void print() const {
        cout << "  Block " << id << ":\n";
        for (const auto& stmt : statements) {
            cout << "    - " << stmt << "\n";
        }
        cout << "    -> Next blocks: ";
        if (next.empty()) {
            cout << "(end)";
        } else {
            for (int n : next) cout << n << " ";
        }
        cout << "\n";
    }
};

//============================================================
// CONTROL FLOW GRAPH (was: CFG.h)
//============================================================

class CFG {
public:
    vector<BasicBlock> blocks;
    int nextBlockId;

    CFG() : nextBlockId(1) {}

    int createBlock() {
        BasicBlock block(nextBlockId);
        blocks.push_back(block);
        return nextBlockId++;
    }

    void addStatementToBlock(int blockId, const string& stmt) {
        for (auto& block : blocks) {
            if (block.id == blockId) {
                block.addStatement(stmt);
                break;
            }
        }
    }

    void connectBlocks(int fromId, int toId) {
        for (auto& block : blocks) {
            if (block.id == fromId) {
                // Check if edge already exists to avoid duplicates
                bool edgeExists = false;
                for (int successor : block.next) {
                    if (successor == toId) {
                        edgeExists = true;
                        break;
                    }
                }
                if (!edgeExists) {
                    block.addSuccessor(toId);
                }
                break;
            }
        }
    }

    void print() const {
        cout << "\nControl Flow Graph:\n";
        for (const auto& block : blocks) {
            block.print();
        }
        cout << "\n";
    }

    void exportDOT(const string& filename) const {
        ofstream file(filename);
        file << "digraph CFG {\n";
        file << "  rankdir=TB;\n\n";

        // Nodes
        for (const auto& block : blocks) {
            file << "  B" << block.id << " [label=\"Block " << block.id << "\\n";
            for (const auto& stmt : block.statements) {
                file << stmt << "\\n";
            }
            file << "\", shape=box];\n";
        }

        file << "\n";

        // Edges
        for (const auto& block : blocks) {
            for (int next : block.next) {
                file << "  B" << block.id << " -> B" << next << ";\n";
            }
        }

        file << "}\n";
        file.close();
        cout << "[OK] CFG exported to " << filename << "\n";
    }
};

//============================================================
// CFG BUILDER (was: CFGBuilder.h/CFGBuilder.cpp)
//============================================================

class CFGBuilder {
private:
    CFG cfg;
    int currentBlockId;
    int exitBlockId;

public:
    CFGBuilder() : currentBlockId(-1), exitBlockId(-1) {}

    CFG getCFG() const {
        return cfg;
    }

    void generate(ProgramNode* program) {
        cout << "\nCFG Generation:\n";

        if (!program) return;

        for (auto& func : program->functions) {
            cout << "[->] Building CFG for function: " << func->name.value << "\n";
            visitFunction(func.get());
        }
    }

    void printCFG() const {
        cfg.print();
    }

    void exportDOT(const string& filename = "cfg.dot") const {
        cfg.exportDOT(filename);
    }

private:
    void visitFunction(FunctionNode* fn) {
        // Create entry block
        currentBlockId = cfg.createBlock();
        exitBlockId = cfg.createBlock();

        if (fn->body) {
            visitBlock(fn->body.get(), currentBlockId, exitBlockId);
        }

        cout << "  [OK] CFG created with " << cfg.blocks.size() << " blocks\n";
    }

    void visitBlock(BlockStmt* block, int entryBlock, int exitBlock) {
        if (!block) return;

        int currentBlock = entryBlock;

        for (auto& stmt : block->statements) {
            visitStmt(stmt.get(), currentBlock, exitBlock);
        }

        // Connect to exit
        cfg.connectBlocks(currentBlock, exitBlock);
    }

    void visitStmt(Stmt* stmt, int& currentBlock, int exitBlock) {
        if (!stmt) return;

        if (auto decl = dynamic_cast<DeclarationStmt*>(stmt)) {
            cfg.addStatementToBlock(currentBlock, 
                "declare " + decl->name.value);
        }
        else if (auto assign = dynamic_cast<AssignmentStmt*>(stmt)) {
            cfg.addStatementToBlock(currentBlock, 
                assign->name.value + " = ...");
        }
        else if (auto ifStmt = dynamic_cast<IfStmt*>(stmt)) {
            // Create branches
            int thenBlock = cfg.createBlock();
            int elseBlock = cfg.createBlock();
            int joinBlock = cfg.createBlock();

            cfg.addStatementToBlock(currentBlock, "if (...) then else");
            cfg.connectBlocks(currentBlock, thenBlock);
            cfg.connectBlocks(currentBlock, elseBlock);

            if (ifStmt->thenBranch) {
                visitStmt(ifStmt->thenBranch.get(), thenBlock, joinBlock);
            }
            cfg.connectBlocks(thenBlock, joinBlock);

            if (ifStmt->elseBranch) {
                visitStmt(ifStmt->elseBranch.get(), elseBlock, joinBlock);
            }
            cfg.connectBlocks(elseBlock, joinBlock);

            currentBlock = joinBlock;
        }
        else if (auto whileStmt = dynamic_cast<WhileStmt*>(stmt)) {
            // Create loop blocks
            int loopBlock = cfg.createBlock();
            int bodyBlock = cfg.createBlock();
            int exitBranchBlock = cfg.createBlock();

            cfg.connectBlocks(currentBlock, loopBlock);
            cfg.addStatementToBlock(loopBlock, "while (...)");
            cfg.connectBlocks(loopBlock, bodyBlock);
            cfg.connectBlocks(loopBlock, exitBranchBlock);

            if (whileStmt->body) {
                visitStmt(whileStmt->body.get(), bodyBlock, loopBlock);
            }

            currentBlock = exitBranchBlock;
        }
        else if (auto forStmt = dynamic_cast<ForStmt*>(stmt)) {
            // Create loop blocks
            if (forStmt->initializer) {
                int initBlock = currentBlock;
                visitStmt(forStmt->initializer.get(), initBlock, exitBlock);
                currentBlock = initBlock;
            }

            int loopCheckBlock = cfg.createBlock();
            int loopBodyBlock = cfg.createBlock();
            int loopUpdateBlock = cfg.createBlock();
            int loopExitBlock = cfg.createBlock();

            cfg.connectBlocks(currentBlock, loopCheckBlock);
            cfg.addStatementToBlock(loopCheckBlock, "for-condition");
            cfg.connectBlocks(loopCheckBlock, loopBodyBlock);
            cfg.connectBlocks(loopCheckBlock, loopExitBlock);

            if (forStmt->body) {
                visitStmt(forStmt->body.get(), loopBodyBlock, loopUpdateBlock);
            }
            cfg.connectBlocks(loopBodyBlock, loopUpdateBlock);

            if (forStmt->update) {
                visitStmt(forStmt->update.get(), loopUpdateBlock, loopCheckBlock);
            }
            cfg.connectBlocks(loopUpdateBlock, loopCheckBlock);

            currentBlock = loopExitBlock;
        }
        else if (auto returnStmt = dynamic_cast<ReturnStmt*>(stmt)) {
            cfg.addStatementToBlock(currentBlock, "return ...");
            cfg.connectBlocks(currentBlock, exitBlock);
        }
        else if (auto block = dynamic_cast<BlockStmt*>(stmt)) {
            visitBlock(block, currentBlock, exitBlock);
        }
    }
};
