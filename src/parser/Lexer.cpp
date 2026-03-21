#include "Lexer.h"
#include <cctype>

Lexer::Lexer(string input) {
    code = input;
    pos = 0;
}

char Lexer::peek() {
    if (pos < code.length()) return code[pos];
    return '\0';
}

char Lexer::peekNext() {
    if (pos + 1 < code.length()) return code[pos + 1];
    return '\0';
}

char Lexer::get() {
    char c = code[pos];
    pos++;
    return c;
}

void Lexer::skipWhitespaceAndComments() {
    while (true) {
        char c = peek();
        if (isspace(c)) {
            get();
        } else if (c == '/' && peekNext() == '/') {
            // Single line comment
            while (peek() != '\n' && peek() != '\0') get();
        } else if (c == '/' && peekNext() == '*') {
            // Multi line comment
            get(); get(); // consume /*
            while (peek() != '\0') {
                if (peek() == '*' && peekNext() == '/') {
                    get(); get(); // consume */
                    break;
                }
                get();
            }
        } else {
            break;
        }
    }
}

Token Lexer::nextToken() {
    skipWhitespaceAndComments();

    Token t;
    char c = peek();

    if (c == '\0') {
        t.type = END_OF_FILE;
        t.value = "";
        return t;
    }

    // Letters (Identifiers and Keywords)
    if (isalpha(c) || c == '_') {
        string word = "";
        while (isalnum(peek()) || peek() == '_') word += get();

        if (word == "int") t.type = KW_INT;
        else if (word == "bool") t.type = KW_BOOL;
        else if (word == "char") t.type = KW_CHAR;
        else if (word == "float") t.type = KW_FLOAT;
        else if (word == "true") t.type = KW_TRUE;
        else if (word == "false") t.type = KW_FALSE;
        else if (word == "if") t.type = KW_IF;
        else if (word == "else") t.type = KW_ELSE;
        else if (word == "while") t.type = KW_WHILE;
        else if (word == "for") t.type = KW_FOR;
        else if (word == "return") t.type = KW_RETURN;
        else t.type = ID;

        t.value = word;
        return t;
    }

    // Numbers (Int and Float)
    if (isdigit(c)) {
        string num = "";
        while (isdigit(peek())) num += get();

        if (peek() == '.') {
            num += get(); // consume '.'
            while (isdigit(peek())) num += get();
            t.type = FLOAT_LITERAL;
        } else {
            t.type = INT_LITERAL;
        }
        t.value = num;
        return t;
    }

    // Characters ('x')
    if (c == '\'') {
        get(); // consume '
        t.value = string(1, get());
        get(); // consume '
        t.type = CHAR_LITERAL;
        return t;
    }

    // Operators and Punctuation
    string op(1, get());

    if (op == "=") {
        if (peek() == '=') { op += get(); t.type = EQ; }
        else { t.type = ASSIGN; }
    } else if (op == "!") {
        if (peek() == '=') { op += get(); t.type = NEQ; }
        else { t.type = NOT; }
    } else if (op == "<") {
        if (peek() == '=') { op += get(); t.type = LTE; }
        else { t.type = LT; }
    } else if (op == ">") {
        if (peek() == '=') { op += get(); t.type = GTE; }
        else { t.type = GT; }
    } else if (op == "&") {
        if (peek() == '&') { op += get(); t.type = LOGICAL_AND; }
        // else bitwise AND not perfectly supported, treat as unknown or single
    } else if (op == "|") {
        if (peek() == '|') { op += get(); t.type = LOGICAL_OR; }
    } else if (op == "+") t.type = PLUS;
    else if (op == "-") t.type = MINUS;
    else if (op == "*") t.type = MUL;
    else if (op == "/") t.type = DIV;
    else if (op == ";") t.type = SEMI;
    else if (op == "(") t.type = LPAREN;
    else if (op == ")") t.type = RPAREN;
    else if (op == "{") t.type = LBRACE;
    else if (op == "}") t.type = RBRACE;

    t.value = op;
    return t;
}
