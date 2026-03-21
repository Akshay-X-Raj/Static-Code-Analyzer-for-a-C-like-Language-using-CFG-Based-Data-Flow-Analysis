#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Token.h"
#include "Lexer.h"
#include "ParserDriver.h"
#include "CFGBuilder.h"
#include "CFGPrinter.h"
#include "SemanticAnalyzer.h"

using namespace std;

int main() {
    ifstream file("tests/samples/test.c");
    if (!file.is_open()) {
        cout << "[ERROR] Could not open 'test.c'. Please create a file named 'test.c' in tests/samples/!\n";
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string code = buffer.str();

    if (code.length() == 0) {
        cout << "[ERROR] 'test.c' is completely empty! Please write some code for me to parse.\n";
        return 1;
    }

    Lexer lexer(code);
    vector<Token> tokens;
    while (true) {
        Token t = lexer.nextToken();
        tokens.push_back(t);
        if (t.type == END_OF_FILE) break;
    }

    cout << "Lexing completed.\n";

    ParserDriver parser(tokens);
    auto program = parser.parseProgram();

    cout << "Parsing completed.\n";

    SemanticAnalyzer semantic;
    semantic.analyze(program.get());

    cout << "Semantic constraints evaluated.\n";

    CFGGenerator generator;
    generator.generate(program.get());

    printCFG();
    exportDOT();
    
    cout << "\nDOT file exported -> cfg.dot\n\n";

    return 0;
}
