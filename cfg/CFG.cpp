#include "CFG.h"
#include <fstream>
#include <set>
#include "../symbolTable/SymbolTable.h"

using namespace std;

extern SymbolTable symTable;

CFG::CFG() {
    nodeCount = 0;
}

// Create node
CFGNode* createNode(int &count, string label) {
    return new CFGNode(count++, label);
}

// Expression to string
string getExpr(ASTNode* node) {
    if (!node) return "";

    if (node->type == "NUM")
        return node->value;

    if (node->type == "ID")
        return node->value;

    if (node->type == "NEG")
        return "-" + getExpr(node->children[0]);

    if (node->type == "OP") {
        string left = getExpr(node->children[0]);
        string right = getExpr(node->children[1]);
        return left + " " + node->value + " " + right;
    }

    if (node->type == "Condition") {
        string left = getExpr(node->children[0]);
        string op   = node->children[1]->value;
        string right= getExpr(node->children[2]);
        return left + " " + op + " " + right;
    }

    if (!node->children.empty())
        return getExpr(node->children[0]);

    return node->value;
}

// -------- BUILD CFG --------

CFGNode* CFG::build(ASTNode* root) {
    if (!root) return nullptr;

    CFGNode* start = createNode(nodeCount, "Start");
    CFGNode* current = start;

    for (auto stmt : root->children) {

        // -------- DECL --------
        if (stmt->type == "Decl") {
            string var = stmt->value;
            string type = symTable.getType(var);

            // 🔥 CHECK IF INITIALIZED
            if (!stmt->children.empty()) {
                string val = getExpr(stmt->children[0]);
                CFGNode* node = createNode(nodeCount, var + " = " + val);
                current->next.push_back(node);
                current = node;
            } else {
                CFGNode* node = createNode(nodeCount, type + " " + var);
                current->next.push_back(node);
                current = node;
            }
        }

        // -------- ASSIGN --------
        else if (stmt->type == "Assign") {
            string var = stmt->children[0]->value;
            string val = getExpr(stmt->children[1]);

            CFGNode* node = createNode(nodeCount, var + " = " + val);
            current->next.push_back(node);
            current = node;
        }

        // -------- PRINT --------
        else if (stmt->type == "Print") {
            CFGNode* node = createNode(nodeCount, "printf(" + stmt->value + ")");
            current->next.push_back(node);
            current = node;
        }

        // -------- RETURN --------
        else if (stmt->type == "Return") {
            string val = getExpr(stmt->children[0]);

            CFGNode* node = createNode(nodeCount, "return " + val);
            current->next.push_back(node);

            //  terminate flow after return
            CFGNode* endNode = createNode(nodeCount, "End");
            node->next.push_back(endNode);

            return start;  // stop further processing
        }

        // -------- IF --------
        else if (stmt->type == "If") {

            ASTNode* cond = stmt->children[0];
            ASTNode* thenStmt = stmt->children[1];
            ASTNode* elseStmt = stmt->children[2];

            string condition = getExpr(cond);
            CFGNode* condNode = createNode(nodeCount, "if (" + condition + ")");
            current->next.push_back(condNode);

            // THEN
            CFGNode* thenNode;
            if (thenStmt->type == "Assign") {
                string var = thenStmt->children[0]->value;
                string val = getExpr(thenStmt->children[1]);
                thenNode = createNode(nodeCount, var + " = " + val);
            } else {
                thenNode = createNode(nodeCount, "stmt");
            }

            // ELSE
            CFGNode* elseNode;
            if (elseStmt->type == "Assign") {
                string var = elseStmt->children[0]->value;
                string val = getExpr(elseStmt->children[1]);
                elseNode = createNode(nodeCount, var + " = " + val);
            } else {
                elseNode = createNode(nodeCount, "stmt");
            }

            condNode->next.push_back(thenNode);
            condNode->next.push_back(elseNode);

            CFGNode* mergeNode = createNode(nodeCount, "merge");
            thenNode->next.push_back(mergeNode);
            elseNode->next.push_back(mergeNode);

            current = mergeNode;
        }
    }

    CFGNode* end = createNode(nodeCount, "End");
    current->next.push_back(end);

    return start;
}

// -------- PRINT CFG --------

void printCFGUtil(CFGNode* node, ofstream &out, set<int> &visited) {
    if (!node || visited.count(node->id)) return;

    visited.insert(node->id);

    out << node->id << " [label=\"" << node->label << "\"];\n";

    for (auto nextNode : node->next) {
        out << node->id << " -> " << nextNode->id << ";\n";
        printCFGUtil(nextNode, out, visited);
    }
}

void CFG::generateDOT(CFGNode* start) {
    ofstream out("cfg.dot");

    out << "digraph CFG {\n";
    out << "node [shape=ellipse];\n";

    set<int> visited;
    printCFGUtil(start, out, visited);

    out << "}\n";

    out.close();
}