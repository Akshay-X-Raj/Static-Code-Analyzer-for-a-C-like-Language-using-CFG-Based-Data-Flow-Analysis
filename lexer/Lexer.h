#pragma once
#include <bits/stdc++.h>
#include "../utils/Token.h"

using namespace std;

class Lexer {
    string code;//stores current input program 
    int pos;//current positioning

public:
    Lexer(string input);//takes input from main.cpp

    vector<Token> tokenize();//converts code into tokens
};