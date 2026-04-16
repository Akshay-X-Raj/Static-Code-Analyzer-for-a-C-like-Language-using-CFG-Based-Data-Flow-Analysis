#include <iostream>
#include <fstream>
#include <sstream>

#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "cfg/CFG.h"
#include "symbolTable/SymbolTable.h"

using namespace std;

// -------- GLOBAL SYMBOL TABLE --------
SymbolTable symTable;

// ---------------- AST PRINT ----------------
void printAST(ASTNode* node, int depth = 0) {
    if (!node) return;

    for (int i = 0; i < depth; i++) cout << "  ";

    cout << node->type;
    if (node->value != "") cout << " (" << node->value << ")";
    cout << endl;

    for (auto child : node->children) {
        printAST(child, depth + 1);
    }
}

// ---------------- MAIN ----------------
int main() {

    cout << "===== COMPILER STARTED =====\n";

    // -------- READ INPUT FILE --------
    ifstream file("test.c");

    if (!file) {
        cout << "Error: File not found!\n";
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string code = buffer.str();

    cout << "\n===== INPUT CODE =====\n";
    cout << code << endl;

    // -------- LEXER --------
    Lexer lexer(code);
    vector<Token> tokens = lexer.tokenize();

    cout << "===== TOKENS =====\n";
    for (auto t : tokens) {
        cout << t.type << " : " << t.value << endl;
    }

    // -------- PARSER --------
    Parser parser(tokens);
    ASTNode* root = parser.parseProgram();   // ❗ if error → exit()

    // -------- AST --------
    cout << "\n===== AST =====\n";
    printAST(root);

    // -------- SYMBOL TABLE --------
    symTable.print();

    // -------- CFG --------
    CFG cfg;
    CFGNode* start = cfg.build(root);
    cfg.generateDOT(start);

    cout << "\nCFG generated successfully (cfg.dot)\n";

    // -------- DATA FLOW --------
    analyzeDataFlow(start);

    cout << "\n===== COMPILER FINISHED =====\n";

    return 0;
}