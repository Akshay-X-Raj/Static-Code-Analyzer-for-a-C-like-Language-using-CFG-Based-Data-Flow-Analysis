#include "Lexer.h"
#include <bits/stdc++.h>

using namespace std;

Lexer::Lexer(string input) {
    code = input;
    pos = 0;
}

vector<Token> Lexer::tokenize() {

    vector<Token> tokens;

    while (pos < code.size()) {

        char current = code[pos];

        if (isspace(current)) {
            pos++;
            continue;
        }

        // -------- Keywords / Identifiers --------
        if (isalpha(current)) {
            string word;

            while (pos < code.size() && isalnum(code[pos])) {
                word += code[pos];
                pos++;
            }

            if (word == "int")
                tokens.push_back({KW_INT, word});
            else if (word == "char")
                tokens.push_back({KW_CHAR, word});
            else if (word == "float")
                tokens.push_back({KW_FLOAT, word});
            else if (word == "double")
                tokens.push_back({KW_DOUBLE, word});
            else if (word == "if")
                tokens.push_back({KW_IF, word});
            else if (word == "else")
                tokens.push_back({KW_ELSE, word});
            else if (word == "printf")
                tokens.push_back({KW_PRINTF, word});
            else if (word == "return")   // ✅ FIX
                tokens.push_back({KW_RETURN, word});
            else
                tokens.push_back({IDENTIFIER, word});

            continue;
        }

        // -------- Numbers --------
        if (isdigit(current)) {
            string num;

            while (pos < code.size() && isdigit(code[pos])) {
                num += code[pos];
                pos++;
            }

            tokens.push_back({NUMBER, num});
            continue;
        }

        // -------- Strings --------
        if (current == '"') {
            pos++;

            string str;
            while (pos < code.size() && code[pos] != '"') {
                str += code[pos];
                pos++;
            }

            pos++;

            tokens.push_back({STRING, str});
            continue;
        }

        // -------- Operators --------
        if (current == '>' || current == '<' || current == '=' || current == '!') {
            string op;
            op += current;
            pos++;

            if (pos < code.size() && code[pos] == '=') {
                op += code[pos];
                pos++;
            }

            if (op == ">" || op == "<" || op == ">=" || op == "<=" ||
                op == "==" || op == "!=") {
                tokens.push_back({REL_OP, op});
            }
            else if (op == "=") {
                tokens.push_back({ASSIGN, "="});
            }

            continue;
        }

        if (current == '+') { tokens.push_back({PLUS, "+"}); pos++; continue; }
        if (current == '-') { tokens.push_back({MINUS, "-"}); pos++; continue; }
        if (current == '*') { tokens.push_back({MUL, "*"}); pos++; continue; }
        if (current == '/') { tokens.push_back({DIV, "/"}); pos++; continue; }

        if (current == ';') { tokens.push_back({SEMICOLON, ";"}); pos++; continue; }
        if (current == '(') { tokens.push_back({LPAREN, "("}); pos++; continue; }
        if (current == ')') { tokens.push_back({RPAREN, ")"}); pos++; continue; }
        if (current == ',') { tokens.push_back({COMMA, ","}); pos++; continue; }

        pos++;
    }

    tokens.push_back({END_OF_FILE, ""});
    return tokens;
}