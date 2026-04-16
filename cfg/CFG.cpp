#include "CFG.h"
#include <fstream>
#include <set>
#include <iostream>
#include <algorithm>

using namespace std;

// ---------- CONSTRUCTOR ----------
CFG::CFG() {
    nodeCount = 0;
}

// ---------- CREATE NODE ----------
CFGNode* createNode(int &count, string label) {
    return new CFGNode(count++, label);
}

// ---------- GET EXPRESSION ----------
string getExpr(ASTNode* node) {

    if (node->type == "NUM" || node->type == "ID")
        return node->value;

    if (node->type == "OP") {
        return getExpr(node->children[0]) + " " +
               node->value + " " +
               getExpr(node->children[1]);
    }

    return "";
}

// ---------- GET CONDITION ----------
string getCondition(ASTNode* node) {
    return getExpr(node->children[0]) + " " +
           node->value + " " +
           getExpr(node->children[1]);
}

// ---------- BUILD STATEMENT ----------
CFGNode* buildStmt(ASTNode* stmt, CFGNode* prev, int &nodeCount) {

    CFGNode* node = nullptr;

    // ---------- DECL ----------
    if (stmt->type == "Decl") {
        node = createNode(nodeCount, stmt->valueType + " " + stmt->value);
        prev->next.push_back(node);
        return node;
    }

    // ---------- ASSIGN ----------
    if (stmt->type == "Assign") {
        string lhs = stmt->children[0]->value;
        string rhs = getExpr(stmt->children[1]);

        node = createNode(nodeCount, lhs + " = " + rhs);
        prev->next.push_back(node);
        return node;
    }
    // ---------- IF ----------
    if (stmt->type == "If") {

        string condStr = getCondition(stmt->children[0]);

        CFGNode* cond = createNode(nodeCount, "if (" + condStr + ")");
        prev->next.push_back(cond);

        // THEN branch
        CFGNode* thenEnd = buildStmt(stmt->children[1], cond, nodeCount);

        // ELSE branch
        CFGNode* elseEnd = nullptr;
        if (stmt->children.size() > 2) {
            elseEnd = buildStmt(stmt->children[2], cond, nodeCount);
        }

        // MERGE NODE
        CFGNode* merge = createNode(nodeCount, "merge");

        if (thenEnd) thenEnd->next.push_back(merge);
        if (elseEnd) elseEnd->next.push_back(merge);
        else cond->next.push_back(merge); // no else case

        return merge;
    }

    // ---------- PRINT (FIXED ONLY THIS PART) ----------
    if (stmt->type == "Print") {

        string label = "printf(" + stmt->value;

        if (!stmt->children.empty()) {
            label += ", " + getExpr(stmt->children[0]);
        }

        label += ")";

        node = createNode(nodeCount, label);

        if (prev) prev->next.push_back(node);

        return node;
    }

    // ---------- BLOCK ----------
    if (stmt->type == "Block") {
        CFGNode* curr = prev;

        for (auto child : stmt->children) {
            curr = buildStmt(child, curr, nodeCount);
        }

        return curr;
    }

    // ---------- WHILE ----------
    if (stmt->type == "While") {

        string condStr = getCondition(stmt->children[0]);

        CFGNode* cond = createNode(nodeCount, "while (" + condStr + ")");
        prev->next.push_back(cond);

        CFGNode* bodyStart = nullptr;
        CFGNode* bodyEnd = nullptr;

        if (stmt->children[1]->type == "Block") {
            CFGNode* curr = cond;

            for (auto child : stmt->children[1]->children) {
                if (!bodyStart) {
                    bodyStart = buildStmt(child, cond, nodeCount);
                    curr = bodyStart;
                } else {
                    curr = buildStmt(child, curr, nodeCount);
                }
            }

            bodyEnd = curr;
        } else {
            bodyStart = buildStmt(stmt->children[1], cond, nodeCount);
            bodyEnd = bodyStart;
        }

        if (bodyEnd)
            bodyEnd->next.push_back(cond);

        CFGNode* exitNode = createNode(nodeCount, "exit");

        cond->next.push_back(exitNode);

        return exitNode;
    }

    return prev;
}

// ---------- BUILD CFG ----------
CFGNode* CFG::build(ASTNode* root) {

    CFGNode* start = createNode(nodeCount, "Start");
    CFGNode* curr = start;

    for (auto stmt : root->children) {
        curr = buildStmt(stmt, curr, nodeCount);
    }

    CFGNode* end = createNode(nodeCount, "End");
    curr->next.push_back(end);

    return start;
}

// ---------- DOT ----------
void CFG::generateDOT(CFGNode* start) {

    ofstream file("cfg.dot");
    file << "digraph CFG {\n";

    set<CFGNode*> visited;

    function<void(CFGNode*)> dfs = [&](CFGNode* node) {

        if (!node || visited.count(node)) return;
        visited.insert(node);

        file << node->id << " [label=\"" << node->label << "\"];\n";

        for (auto nxt : node->next) {
            file << node->id << " -> " << nxt->id << ";\n";
            dfs(nxt);
        }
    };

    dfs(start);

    file << "}\n";
    file.close();
}

// ---------- DATA FLOW ----------
void analyzeDataFlow(CFGNode* start) {

    set<string> defined;
    set<string> used;
    set<CFGNode*> visited;

    function<void(CFGNode*)> dfs = [&](CFGNode* node) {

        if (!node || visited.count(node)) return;
        visited.insert(node);

        string label = node->label;

        if (label.find("int ") != string::npos) {
            string var = label.substr(label.find(" ") + 1);
            defined.insert(var);
        }

        if (label.find("=") != string::npos) {

            string lhs = label.substr(0, label.find("="));
            lhs.erase(remove(lhs.begin(), lhs.end(), ' '), lhs.end());

            defined.insert(lhs);
            used.insert(lhs);

            string rhs = label.substr(label.find("=") + 1);

            string temp = "";
            for (char c : rhs) {
                if (isalnum(c)) temp += c;
                else {
                    if (!temp.empty() && !isdigit(temp[0]))
                        used.insert(temp);
                    temp = "";
                }
            }
            if (!temp.empty() && !isdigit(temp[0]))
                used.insert(temp);
        }

        if (label.find("printf") != string::npos) {
            int comma = label.find(",");
            int end = label.find(")");

            if (comma != string::npos && end != string::npos) {
                string var = label.substr(comma + 1, end - comma - 1);
                var.erase(remove(var.begin(), var.end(), ' '), var.end());
                used.insert(var);
            }
        }

        for (auto nxt : node->next)
            dfs(nxt);
    };

    dfs(start);

    cout << "\n===== WARNINGS =====\n";
    for (auto var : defined) {
        if (used.find(var) == used.end()) {
            cout << "Warning: '" << var << "' declared but never used\n";
        }
    }
}