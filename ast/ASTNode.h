#pragma once
#include <bits/stdc++.h>
using namespace std;

class ASTNode {
public:
    string type;        // Decl, Assign, Print, etc.
    string value;       // variable name / operator / literal
    string valueType;   //  int, float, char, etc.
    vector<ASTNode*> children;

    ASTNode(string t, string v = "", string vt = "") {
        type = t;
        value = v;
        valueType = vt;   //  store type
    }
};