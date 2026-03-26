#include "CFG.h"
#include <iostream>
#include <fstream>

using namespace std;

CFG::CFG() {
    start = NULL;
}

void CFG::build(vector<ASTNode*> program) {

    CFGNode* startNode = new CFGNode("START");
    CFGNode* endNode = new CFGNode("END");

    CFGNode* prev = startNode;

    for (int i = 0; i < program.size(); i++) {

        ASTNode* root = program[i];

        // 🔥 ASSIGNMENT
        if (root->value == "=") {

            string expr = root->right->left->value + " " +
                          root->right->value + " " +
                          root->right->right->value;

            CFGNode* exprNode = new CFGNode(expr);
            prev->next.push_back(exprNode);
            prev = exprNode;

            string assign = root->left->value + " = " + expr;
            CFGNode* assignNode = new CFGNode(assign);

            prev->next.push_back(assignNode);
            prev = assignNode;
        }

        // 🔥 IF
        else if (root->value == "if") {

            CFGNode* cond = new CFGNode("IF");
            prev->next.push_back(cond);

            ASTNode* body = root->body;

            string stmt = body->left->value + " = " +
                          body->right->left->value + " " +
                          body->right->value + " " +
                          body->right->right->value;

            CFGNode* trueNode = new CFGNode(stmt);
            CFGNode* merge = new CFGNode("MERGE");

            cond->next.push_back(trueNode);
            cond->next.push_back(merge);

            trueNode->next.push_back(merge);

            prev = merge;
        }

        // 🔥 WHILE
        else if (root->value == "while") {

            CFGNode* cond = new CFGNode("WHILE");
            prev->next.push_back(cond);

            ASTNode* body = root->body;

            string stmt = body->left->value + " = " +
                          body->right->left->value + " " +
                          body->right->value + " " +
                          body->right->right->value;

            CFGNode* bodyNode = new CFGNode(stmt);

            cond->next.push_back(bodyNode);
            bodyNode->next.push_back(cond);

            CFGNode* exitNode = new CFGNode("EXIT");
            cond->next.push_back(exitNode);

            prev = exitNode;
        }
    }

    prev->next.push_back(endNode);

    start = startNode;
}

void CFG::print(CFGNode* node) {
    if (node == NULL) return;

    cout << node->value << " -> ";

    for (int i = 0; i < node->next.size(); i++) {
        cout << node->next[i]->value << " ";
    }

    cout << endl;

    for (int i = 0; i < node->next.size(); i++) {
        print(node->next[i]);
    }
}

void CFG::generateDOT(CFGNode* node, ofstream& out) {
    if (node == NULL) return;

    for (int i = 0; i < node->next.size(); i++) {
        out << "\"" << node->value << "\" -> \""
            << node->next[i]->value << "\";\n";

        generateDOT(node->next[i], out);
    }
}

void CFG::exportToDOT() {
    ofstream out("cfg.dot");

    out << "digraph CFG {\n";
    out << "rankdir=TB;\n";

    generateDOT(start, out);

    out << "}\n";
    out.close();
}