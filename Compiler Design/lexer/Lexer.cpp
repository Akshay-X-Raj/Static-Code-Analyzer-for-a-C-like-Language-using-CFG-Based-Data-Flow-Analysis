#include "Lexer.h"
#include <cctype>

Lexer::Lexer(string input) {
    source = input;
    pos = 0;
}

char Lexer::peek() {
    if (pos < source.size())
        return source[pos];
    return '\0';
}

char Lexer::get() {
    if (pos < source.size())
        return source[pos++];
    return '\0';
}

void Lexer::skipWhitespace() {
    while (isspace(peek())) {
        get();
    }
}

// 🔥 Updated keywords
bool Lexer::isKeyword(string word) {
    return (word == "int" || word == "float" || word == "char" ||
            word == "double" || word == "if" || word == "while" ||
            word == "return");
}

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;

    while (pos < source.size()) {
        skipWhitespace();

        char ch = peek();

        //  IDENTIFIER or KEYWORD (with _ support)
        if (isalpha(ch) || ch == '_') {
            string word = "";

            while (isalnum(peek()) || peek() == '_') {
                word += get();
            }

            if (isKeyword(word))
                tokens.push_back(Token(KEYWORD, word));
            else
                tokens.push_back(Token(IDENTIFIER, word));
        }

        // 🔥 NUMBER
        else if (isdigit(ch)) {
            string num = "";
            while (isdigit(peek())) {
                num += get();
            }
            tokens.push_back(Token(NUMBER, num));
        }

        // 🔥 OPERATORS (extended)
            else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '_') {
            string op = "";
            op += get();
            tokens.push_back(Token(OPERATOR, op));
        }

        // 🔥 SYMBOLS
        else if (ch == ';' || ch == '(' || ch == ')') {
            string sym = "";
            sym += get();
            tokens.push_back(Token(SYMBOL, sym));
        }

        else {
            get(); // ignore unknown characters
        }
    }

    tokens.push_back(Token(END, "EOF"));
    return tokens;
}