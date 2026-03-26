#include <iostream>
#include <vector>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "cfg/CFG.h"
#include "dataflow/DataFlow.h"

using namespace std;

int main() {

    string code =
        "int a = b + c;\n"
        "if (a)\n"
        "a = a + 1;\n"
        "while (a)\n"
        "a = a - 1;";

    Lexer lexer(code);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    vector<ASTNode*> program = parser.parseProgram();

    CFG cfg;
    cfg.build(program);

    cout << "CFG:\n";
    cfg.print(cfg.start);

    cfg.exportToDOT();

    DataFlow df;
    df.analyze(program[0]); // simple

    return 0;
}