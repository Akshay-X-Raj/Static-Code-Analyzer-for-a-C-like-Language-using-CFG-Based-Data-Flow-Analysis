#include "CFGPrinter.h"
#include "CFG.h"
#include <iostream>
#include <fstream>

using namespace std;

void printCFG() {
    cout << "\nCFG STRUCTURE:\n";
    for (int i = 0; i < cfg.size(); i++) {
        cout << "Block " << cfg[i].id << ": " << cfg[i].statements[0] << "\n";
        for (int j = 0; j < cfg[i].next.size(); j++) {
            cout << "   -> Block " << cfg[i].next[j] << "\n";
        }
    }
}

void exportDOT() {
    ofstream dot("cfg.dot");
    dot << "digraph CFG {\n";
    for (int i = 0; i < cfg.size(); i++) {
        dot << "B" << cfg[i].id << " [label=\"" << cfg[i].statements[0] << "\"];\n";
        for (int j = 0; j < cfg[i].next.size(); j++) {
            dot << "B" << cfg[i].id << " -> B" << cfg[i].next[j] << ";\n";
        }
    }
    dot << "}\n";
}
