#include "Lexer.h"
#include <bits/stdc++.h>

using namespace std;

Lexer::Lexer(string input) {
    code = input;
    pos = 0;
}

// ---------------- TOKENIZE ----------------
vector<Token> Lexer::tokenize() {

    vector<Token> tokens;
    int line = 1;   // LINE TRACK

    while (pos < code.size()) {

        char current = code[pos];

        // --------- SPACES ----------
        if (isspace(current)) {
            if (current == '\n') line++;   // track line
            pos++;
            continue;
        }

        // --------- IDENTIFIER / KEYWORD ----------
        if (isalpha(current)) {
            string word;

            while (pos < code.size() && isalnum(code[pos])) {
                word += code[pos];
                pos++;
            }

            if (word == "int")
                tokens.push_back({KW_INT, word, line});
            else if (word == "char")
                tokens.push_back({KW_CHAR, word, line});
            else if (word == "float")
                tokens.push_back({KW_FLOAT, word, line});
            else if (word == "double")
                tokens.push_back({KW_DOUBLE, word, line});
            else if (word == "if")
                tokens.push_back({KW_IF, word, line});
            else if (word == "else")
                tokens.push_back({KW_ELSE, word, line});
            else if (word == "while")
                tokens.push_back({KW_WHILE, word, line});
            else if (word == "printf")
                tokens.push_back({KW_PRINTF, word, line});
            else
                tokens.push_back({IDENTIFIER, word, line});

            continue;
        }

        // --------- NUMBER (INT + FLOAT) ----------
        if (isdigit(current)) {
            string num;
            bool hasDot = false;

            while (pos < code.size() &&
                  (isdigit(code[pos]) || code[pos] == '.')) {

                if (code[pos] == '.') {
                    if (hasDot) break;
                    hasDot = true;
                }

                num += code[pos];
                pos++;
            }

            tokens.push_back({NUMBER, num, line});
            continue;
        }

        // --------- STRING ----------
        if (current == '"') {
            pos++;
            string str;

            while (pos < code.size() && code[pos] != '"') {
                str += code[pos];
                pos++;
            }

            pos++; // closing "
            tokens.push_back({STRING, str, line});
            continue;
        }

        // --------- RELATIONAL ----------
        if (current == '>' || current == '<' || current == '=' || current == '!') {
            string op;
            op += current;
            pos++;

            if (pos < code.size() && code[pos] == '=') {
                op += code[pos];
                pos++;
            }

            if (op == "=")
                tokens.push_back({ASSIGN, op, line});
            else
                tokens.push_back({REL_OP, op, line});

            continue;
        }

        // --------- ARITHMETIC ----------
        if (current == '+') {
            tokens.push_back({PLUS, "+", line});
            pos++;
            continue;
        }

        if (current == '-') {
            tokens.push_back({MINUS, "-", line});
            pos++;
            continue;
        }

        if (current == '*') {
            tokens.push_back({MUL, "*", line});
            pos++;
            continue;
        }

        if (current == '/') {
            tokens.push_back({DIV, "/", line});
            pos++;
            continue;
        }

        // --------- SYMBOLS ----------
        if (current == ';') {
            tokens.push_back({SEMICOLON, ";", line});
            pos++;
            continue;
        }

        if (current == '(') {
            tokens.push_back({LPAREN, "(", line});
            pos++;
            continue;
        }

        if (current == ')') {
            tokens.push_back({RPAREN, ")", line});
            pos++;
            continue;
        }

        if (current == '{') {
            tokens.push_back({LBRACE, "{", line});
            pos++;
            continue;
        }

        if (current == '}') {
            tokens.push_back({RBRACE, "}", line});
            pos++;
            continue;
        }

        if (current == ',') {
            tokens.push_back({COMMA, ",", line});
            pos++;
            continue;
        }

        // --------- UNKNOWN ----------
        pos++;
    }

    tokens.push_back({END_OF_FILE, "", line});
    return tokens;
}