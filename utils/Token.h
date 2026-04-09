#pragma once
#include <string>

using namespace std;

// Token types
enum TokenType {

    // Keywords
    KW_INT,
    KW_CHAR,
    KW_FLOAT,
    KW_DOUBLE,
    KW_IF,
    KW_ELSE,
    KW_PRINTF,
    KW_RETURN,

    // Identifiers & values
    IDENTIFIER,
    NUMBER,
    STRING,

    // Operators
    ASSIGN,     // =
    REL_OP,     // > < >= <= == !=

    PLUS,       // +
    MINUS,      // -
    MUL,        // *
    DIV,        // /

    // Symbols
    SEMICOLON,  // ;
    LPAREN,     // (
    RPAREN,     // )
    COMMA,      // ,

    // End
    END_OF_FILE
};

// Token structure
struct Token {
    TokenType type;
    string value;
};