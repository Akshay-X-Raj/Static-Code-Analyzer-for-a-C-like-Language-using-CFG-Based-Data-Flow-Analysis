#pragma once
#include <string>
using namespace std;

enum TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    SYMBOL,
    END
};

struct Token {
    TokenType type;
    string value;

    Token(TokenType t, string v) {
        type = t;
        value = v;
    }
};