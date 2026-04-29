#pragma once
#include <string>

using namespace std;

// Token types
enum TokenType {

    // Keywords
    KW_INT,        // 0
    KW_CHAR,       // 1
    KW_FLOAT,      // 2
    KW_DOUBLE,     // 3
    KW_IF,         // 4
    KW_ELSE,       // 5
    KW_WHILE,      // 6
    KW_PRINTF,     // 7

    // Identifiers & values
    IDENTIFIER,    // 8
    NUMBER,        // 9
    STRING,        // 10

    // Operators
    ASSIGN,        // 11   (=)
    REL_OP,        // 12   (> < >= <= == !=)

    PLUS,          // 13   (+)
    MINUS,         // 14   (-)
    MUL,           // 15   (*)
    DIV,           // 16   (/)

    // Symbols
    SEMICOLON,     // 17   (;)
    LPAREN,        // 18   ( )
    RPAREN,        // 19   ( )
    LBRACE,        // 20   {
    RBRACE,        // 21    }
    COMMA,         // 22   (,)

    // End
    END_OF_FILE    // 23
};

// Token structure
struct Token {
    TokenType type;  // token number (from enum)
    string value;    // actual text (e.g., "int", "x", "5")
    int line;        // line number in input
};