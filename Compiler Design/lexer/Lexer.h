#pragma once
#include <string>
#include <vector>
#include "../utils/Token.h"
using namespace std;

class Lexer {
private:
    string source;
    int pos;

public:
    Lexer(string input);

    vector<Token> tokenize();

    char peek();
    char get();
    void skipWhitespace();

    bool isKeyword(string word);
};