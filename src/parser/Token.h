#pragma once
#include <string>

enum TokenType {
    // Keywords
    KW_INT, KW_BOOL, KW_CHAR, KW_FLOAT,
    KW_TRUE, KW_FALSE,
    KW_IF, KW_ELSE, KW_WHILE, KW_FOR, KW_RETURN,

    // Identifiers and Literals
    ID, INT_LITERAL, FLOAT_LITERAL, CHAR_LITERAL,

    // Operators
    ASSIGN,          // =
    LOGICAL_OR,     // ||
    LOGICAL_AND,    // &&
    EQ, NEQ,        // ==, !=
    LT, GT, LTE, GTE, // <, >, <=, >=
    PLUS, MINUS,    // +, -
    MUL, DIV,       // *, /
    NOT,            // !

    // Punctuation
    SEMI,           // ;
    LPAREN, RPAREN, // (, )
    LBRACE, RBRACE, // {, }

    // End of File
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string value;
};
