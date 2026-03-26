#include "DataFlow.h"
#include <iostream>

void DataFlow::analyze(ASTNode* root) {
    if (root == NULL) return;

    // Assignment case: a = something
    if (root->value == "=") {
        string var = root->left->value;
        defined.insert(var);

        ASTNode* right = root->right;

        // If right is operator
        if (right->left && right->right) {
            used.insert(right->left->value);
            used.insert(right->right->value);
        } else {
            used.insert(right->value);
        }
    }
}

void DataFlow::report() {
    cout << "\nData Flow Analysis:\n";

    for (auto u : used) {
        if (defined.find(u) == defined.end()) {
            cout << "Undefined Variable: " << u << endl;
        }
    }
}