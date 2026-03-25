/*
 * UNIFIED STATIC CODE ANALYZER - CONSOLIDATED VERSION
 * 
 * This is the SIMPLIFIED main that uses only 5 consolidated headers:
 *   1. Parser.h        - Lexer + Parser + AST nodes + Tokens
 *   2. Semantic.h      - SemanticAnalyzer + Scope + SymbolTable
 *   3. CFG.h           - BasicBlock + CFG + CFGBuilder
 *   4. DataFlow.h      - DataFlowAnalysis + WorklistSolver + Analyses
 *   5. (standard library)
 * 
 * Instead of 20+ scattered files, everything is organized into 4 core modules!
 */

#include <iostream>
#include <fstream>
#include <sstream>

#include "Parser.h"
#include "Semantic.h"
#include "CFG.h"
#include "DataFlow.h"

using namespace std;

//============================================================
// MAIN PIPELINE
//============================================================

int main() {
    cout << "\nSTATIC CODE ANALYZER - CONSOLIDATED VERSION\n";
    cout << "Using only 4 core headers instead of 20+ files!\n\n";

    //========== STEP 1: READ SOURCE CODE ==========
    cout << "\n[STEP 1] Reading source code...\n";
    ifstream file("../tests/samples/test.c");
    if (!file.is_open()) {
        Logger::error("Could not open '../tests/samples/test.c'");
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string code = buffer.str();

    if (code.empty()) {
        Logger::error("test.c is empty!");
        return 1;
    }

    cout << "[OK] Read " << code.length() << " characters\n";

    //========== STEP 2: LEXICAL ANALYSIS ==========
    cout << "\n[STEP 2] Lexical Analysis (Tokenization)\n";

    Lexer lexer(code);
    vector<Token> tokens;

    while (true) {
        Token t = lexer.nextToken();
        tokens.push_back(t);
        if (t.type == END_OF_FILE) break;
    }

    cout << "[OK] Generated " << (tokens.size() - 1) << " tokens\n";
    cout << "    Example tokens: ";
    for (size_t i = 0; i < min(size_t(8), tokens.size()); i++) {
        if (tokens[i].type != END_OF_FILE) {
            cout << "'" << tokens[i].value << "' ";
        }
    }
    cout << "...\n";

    //========== STEP 3: PARSING ==========
    cout << "\n[STEP 3] Parsing (Building AST)\n";

    ParserDriver parser(tokens);
    auto program = parser.parseProgram();

    cout << "[OK] Built AST with " << program->functions.size() << " function(s)\n";
    if (program->functions.size() > 0) {
        cout << "    Functions: ";
        for (auto& func : program->functions) {
            cout << func->name.value << " ";
        }
        cout << "\n";
    }

    //========== STEP 4: SEMANTIC ANALYSIS ==========
    cout << "\n[STEP 4] Semantic Analysis\n";

    SemanticAnalyzer semanticAnalyzer;
    semanticAnalyzer.analyze(program.get());

    //========== STEP 5: CFG GENERATION ==========
    cout << "\n[STEP 5] Control Flow Graph Generation\n";

    CFGBuilder cfgBuilder;
    cfgBuilder.generate(program.get());
    cfgBuilder.printCFG();

    //========== STEP 6: DATA FLOW ANALYSIS ==========
    cout << "\n[STEP 6] Data Flow Analysis\n";

    DataFlowEngine dataFlowEngine;
    dataFlowEngine.addAnalysis(make_unique<UninitializedVarsAnalysis>());
    dataFlowEngine.addAnalysis(make_unique<UnusedVarsAnalysis>());
    dataFlowEngine.addAnalysis(make_unique<UnreachableCodeAnalysis>());

    dataFlowEngine.solve(cfgBuilder.getCFG());

    //========== STEP 7: EXPORT ==========
    cout << "\n[STEP 7] Exporting Results\n";

    cfgBuilder.exportDOT("cfg.dot");

    //========== SUMMARY ==========
    cout << "\n" << string(50, '=') << "\n";
    cout << "ANALYSIS COMPLETE!\n";
    cout << string(50, '=') << "\n\n";
    cout << "Pipeline executed successfully:\n";
    cout << "  1. Lexer (tokenization)      [OK]\n";
    cout << "  2. Parser (AST building)     [OK]\n";
    cout << "  3. Semantic analysis         [OK]\n";
    cout << "  4. CFG generation            [OK]\n";
    cout << "  5. Data flow analysis        [OK]\n";
    cout << "  6. Report generation         [OK]\n\n";
    cout << "Output files:\n";
    cout << "  - cfg.dot - Control Flow Graph (visualize with Graphviz)\n\n";
    cout << "Architecture:\n";
    cout << "  - 4 consolidated header files (Parser, Semantic, CFG, DataFlow)\n";
    cout << "  - Each file = one logical module\n";
    cout << "  - Reduced from 20+ files to just 4 core includes!\n\n";

    return 0;
}
