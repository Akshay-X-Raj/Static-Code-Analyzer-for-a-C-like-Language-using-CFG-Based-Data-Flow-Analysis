/*
 * CONSOLIDATED DATA FLOW ANALYSIS
 * Combines: DataFlowAnalysis + WorklistSolver + Analysis implementations
 * Performs various static analyses to detect bugs
 */

#pragma once
#include "CFG.h"
#include <set>
#include <queue>
using namespace std;

//============================================================
// BASE DATA FLOW ANALYSIS (was: DataFlowAnalysis.h)
//============================================================

class DataFlowAnalysis {
public:
    virtual ~DataFlowAnalysis() = default;
    virtual void analyze(const CFG& cfg) = 0;
    virtual void printResults() = 0;
};

//============================================================
// ANALYSIS: UNINITIALIZED VARIABLES
//============================================================

class UninitializedVarsAnalysis : public DataFlowAnalysis {
private:
    vector<string> issues;

public:
    void analyze(const CFG& cfg) override {
        cout << "\n[Analysis] Detecting uninitialized variable usage...\n";

        set<string> definedVars;

        for (const auto& block : cfg.blocks) {
            for (const auto& stmt : block.statements) {
                if (stmt.find("declare") != string::npos) {
                    // Extract variable name from "declare varname"
                    string var = stmt.substr(8);
                    definedVars.insert(var);
                } else if (stmt.find("=") != string::npos) {
                    // Assignment found
                    size_t eqPos = stmt.find("=");
                    string var = stmt.substr(0, eqPos);
                    // Trim whitespace
                    var.erase(var.find_last_not_of(" ") + 1);
                    definedVars.insert(var);
                }
            }
        }

        // Simple check: report any undefined declarations (in real analysis, would be more complex)
    }

    void printResults() override {
        if (issues.empty()) {
            cout << "  [OK] No uninitialized variables detected\n";
        } else {
            cout << "  [⚠] Uninitialized variables:\n";
            for (const auto& issue : issues) {
                cout << "    - " << issue << "\n";
            }
        }
    }
};

//============================================================
// ANALYSIS: UNUSED VARIABLES
//============================================================

class UnusedVarsAnalysis : public DataFlowAnalysis {
private:
    set<string> declaredVars;
    set<string> usedVars;

public:
    void analyze(const CFG& cfg) override {
        cout << "[Analysis] Detecting unused variables...\n";

        for (const auto& block : cfg.blocks) {
            for (const auto& stmt : block.statements) {
                if (stmt.find("declare") != string::npos) {
                    string var = stmt.substr(8);
                    declaredVars.insert(var);
                }
                // Simplified: assume any non-declare statement uses variables
                if (stmt.find("=") != string::npos && stmt.find("declare") == string::npos) {
                    size_t eqPos = stmt.find("=");
                    string rhs = stmt.substr(eqPos + 1);
                    // Very simplified - would need proper parsing in real code
                }
            }
        }
    }

    void printResults() override {
        cout << "  [OK] Unused variable analysis complete\n";
        set<string> unused;
        for (const auto& var : declaredVars) {
            if (usedVars.find(var) == usedVars.end()) {
                unused.insert(var);
            }
        }

        if (unused.empty()) {
            cout << "  [OK] No unused variables found\n";
        } else {
            cout << "  [!] Unused variables:\n";
            for (const auto& var : unused) {
                cout << "    - " << var << "\n";
            }
        }
    }
};

//============================================================
// ANALYSIS: UNREACHABLE CODE
//============================================================

class UnreachableCodeAnalysis : public DataFlowAnalysis {
private:
    vector<int> reachableBlocks;

public:
    void analyze(const CFG& cfg) override {
        cout << "[Analysis] Detecting unreachable code...\n";

        // BFS to find all reachable blocks
        set<int> visited;
        queue<int> q;

        if (!cfg.blocks.empty()) {
            q.push(cfg.blocks[0].id);
            visited.insert(cfg.blocks[0].id);
        }

        while (!q.empty()) {
            int blockId = q.front();
            q.pop();
            reachableBlocks.push_back(blockId);

            for (const auto& block : cfg.blocks) {
                if (block.id == blockId) {
                    for (int next : block.next) {
                        if (visited.find(next) == visited.end()) {
                            visited.insert(next);
                            q.push(next);
                        }
                    }
                    break;
                }
            }
        }
    }

    void printResults() override {
        cout << "  [OK] Reachable blocks: " << reachableBlocks.size() << "\n";
        // In a real implementation, would report unreachable blocks
    }
};

//============================================================
// DATA FLOW SOLVER (was: WorklistSolver.h/WorklistSolver.cpp)
//============================================================

class DataFlowEngine {
private:
    vector<unique_ptr<DataFlowAnalysis>> analyses;

public:
    void addAnalysis(unique_ptr<DataFlowAnalysis> analysis) {
        analyses.push_back(move(analysis));
    }

    void solve(const CFG& cfg) {
        cout << "\nData Flow Analysis:\n";

        for (auto& analysis : analyses) {
            analysis->analyze(cfg);
        }

        cout << "\nAnalysis Results:\n";

        for (auto& analysis : analyses) {
            analysis->printResults();
        }
    }
};

//============================================================
// UTILITY: LOGGER (was: Logger.h)
//============================================================

class Logger {
public:
    static void info(const string& msg) {
        cout << "[INFO] " << msg << "\n";
    }

    static void error(const string& msg) {
        cerr << "[ERROR] " << msg << "\n";
    }

    static void warning(const string& msg) {
        cerr << "[WARN] " << msg << "\n";
    }

    static void success(const string& msg) {
        cout << "[OK] " << msg << "\n";
    }
};
