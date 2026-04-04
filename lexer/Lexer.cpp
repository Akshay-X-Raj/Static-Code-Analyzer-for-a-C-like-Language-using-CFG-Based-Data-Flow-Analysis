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

    while (pos < code.size()) {

        char current = code[pos];

        // --------- 1. Skip spaces ---------
        if (isspace(current)) {
            pos++;
            continue;
        }

        // --------- 2. Keywords / Identifiers ---------
        if (isalpha(current)) {
            string word;

            while (pos < code.size() && isalnum(code[pos])) {
                word += code[pos];
                pos++;
            }

            // -------- KEYWORDS --------
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
            else
                tokens.push_back({IDENTIFIER, word});

            continue;
        }

        // --------- 3. Numbers ---------
        if (isdigit(current)) {
            string num;

            while (pos < code.size() && isdigit(code[pos])) {
                num += code[pos];
                pos++;
            }

            tokens.push_back({NUMBER, num});
            continue;
        }

        // --------- 4. Strings ---------
        if (current == '"') {
            pos++; // skip opening "

            string str;
            while (pos < code.size() && code[pos] != '"') {
                str += code[pos];
                pos++;
            }

            pos++; // skip closing "

            tokens.push_back({STRING, str});
            continue;
        }

        // --------- 5. Relational Operators FIRST ---------
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

        // --------- 6. Arithmetic Operators ---------
        if (current == '+') {
            tokens.push_back({PLUS, "+"});
            pos++;
            continue;
        }

        if (current == '-') {
            tokens.push_back({MINUS, "-"});
            pos++;
            continue;
        }

        if (current == '*') {
            tokens.push_back({MUL, "*"});
            pos++;
            continue;
        }

        if (current == '/') {
            tokens.push_back({DIV, "/"});
            pos++;
            continue;
        }

        // --------- 7. Symbols ---------
        if (current == ';') {
            tokens.push_back({SEMICOLON, ";"});
            pos++;
            continue;
        }

        if (current == '(') {
            tokens.push_back({LPAREN, "("});
            pos++;
            continue;
        }

        if (current == ')') {
            tokens.push_back({RPAREN, ")"});
            pos++;
            continue;
        }

        if (current == ',') {
            tokens.push_back({COMMA, ","});  
            pos++;
            continue;
        }

        // --------- 8. Unknown ---------
        pos++;
    }

    tokens.push_back({END_OF_FILE, ""});
    return tokens;
}