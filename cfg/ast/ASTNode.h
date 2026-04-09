#pragma once
#include <bits/stdc++.h>
using namespace std;

class ASTNode {
public:
    string type;
    string value;
    vector<ASTNode*> children;

    ASTNode(string t, string v = "") {
        type = t;
        value = v;
    }
};
