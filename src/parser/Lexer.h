#pragma once
#include <string>
#include "Token.h"

using namespace std;

class Lexer {
    string code;
    size_t pos = 0;

public:
    Lexer(string input);
    char peek();
    char peekNext();
    char get();
    void skipWhitespaceAndComments();
    Token nextToken();
};
