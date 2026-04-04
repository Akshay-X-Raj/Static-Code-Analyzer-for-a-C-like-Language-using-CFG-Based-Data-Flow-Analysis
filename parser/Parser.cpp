#include "Parser.h"
#include <iostream>
#include "../symbolTable/SymbolTable.h"

using namespace std;

extern SymbolTable symTable;

Parser::Parser(vector<Token> t) {
    tokens = t;
    pos = 0;
}

// ---------- HELPERS ----------
Token Parser::peek() {
    if (pos < tokens.size()) return tokens[pos];
    return {END_OF_FILE, ""};
}

Token Parser::get() {
    if (pos < tokens.size()) return tokens[pos++];
    return {END_OF_FILE, ""};
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        get();
        return true;
    }
    cout << "Parser Error near: " << peek().value << endl;
    exit(1);
}

// ---------- EXPRESSION TYPE CALCULATOR ----------
string getExprType(ASTNode* node) {

    if (!node) return "int";

    if (node->type == "NUM")
        return "int";

    if (node->type == "ID")
        return symTable.getType(node->value);

    if (node->type == "NEG")
        return getExprType(node->children[0]);

    if (node->type == "OP") {
        string left = getExprType(node->children[0]);
        string right = getExprType(node->children[1]);

        if (left == "double" || right == "double")
            return "double";

        if (left == "float" || right == "float")
            return "float";

        return "int";
    }

    return "int";
}

// ---------- PROGRAM ----------
ASTNode* Parser::parseProgram() {
    ASTNode* root = new ASTNode("Program");

    while (peek().type != END_OF_FILE) {
        root->children.push_back(parseStmt());
    }

    return root;
}

// ---------- STATEMENTS ----------
ASTNode* Parser::parseStmt() {
    if (peek().type == KW_INT || peek().type == KW_CHAR ||
        peek().type == KW_FLOAT || peek().type == KW_DOUBLE)
        return parseDecl();

    if (peek().type == IDENTIFIER)
        return parseAssign();

    if (peek().type == KW_PRINTF)
        return parsePrint();

    if (peek().type == KW_IF)
        return parseIf();

    cout << "Unknown statement: " << peek().value << endl;
    get();
    return nullptr;
}

// ---------- DECL ----------
ASTNode* Parser::parseDecl() {
    Token typeToken = get();
    string type = typeToken.value;

    Token id = get();

    if (id.type != IDENTIFIER) {
        cout << "Error: Expected identifier\n";
        exit(1);
    }

    symTable.add(id.value, type);

    match(SEMICOLON);

    return new ASTNode("Decl", id.value);
}

// ---------- ASSIGN ----------
ASTNode* Parser::parseAssign() {
    Token id = get();

    symTable.checkUse(id.value);

    match(ASSIGN);

    ASTNode* expr = parseExpr();

    // type of the expression
    string exprType = getExprType(expr);

    symTable.checkType(id.value, exprType);

    match(SEMICOLON);

    ASTNode* node = new ASTNode("Assign");
    node->children.push_back(new ASTNode("ID", id.value));
    node->children.push_back(expr);

    return node;
}

// ---------- PRINT ----------
ASTNode* Parser::parsePrint() {
    get(); // printf

    match(LPAREN);

    Token format = get();

    string var = "";

    if (peek().type == COMMA) {
        get();

        Token id = get();
        symTable.checkUse(id.value);
        var = id.value;
    }

    match(RPAREN);
    match(SEMICOLON);

    return new ASTNode("Print", format.value + ", " + var);
}

// ---------- IF ----------
ASTNode* Parser::parseIf() {
    get();

    match(LPAREN);

    ASTNode* cond = parseCondition();

    match(RPAREN);

    ASTNode* thenStmt = parseStmt();

    match(KW_ELSE);

    ASTNode* elseStmt = parseStmt();

    ASTNode* node = new ASTNode("If");
    node->children = {cond, thenStmt, elseStmt};

    return node;
}

// ---------- CONDITION ----------
ASTNode* Parser::parseCondition() {
    ASTNode* left = parseExpr();
    Token op = get();
    ASTNode* right = parseExpr();

    ASTNode* node = new ASTNode("Condition");
    node->children = {left, new ASTNode("OP", op.value), right};

    return node;
}

// ---------- EXPRESSIONS ----------
ASTNode* Parser::parseExpr() {
    ASTNode* left = parseTerm();

    while (peek().type == PLUS || peek().type == MINUS) {
        Token op = get();
        ASTNode* right = parseTerm();

        ASTNode* node = new ASTNode("OP", op.value);
        node->children = {left, right};

        left = node;
    }

    return left;
}

ASTNode* Parser::parseTerm() {
    ASTNode* left = parseFactor();

    while (peek().type == MUL || peek().type == DIV) {
        Token op = get();
        ASTNode* right = parseFactor();

        ASTNode* node = new ASTNode("OP", op.value);
        node->children = {left, right};

        left = node;
    }

    return left;
}

// ---------- FACTOR ----------
ASTNode* Parser::parseFactor() {

    if (peek().type == MINUS) {
        get();
        ASTNode* node = new ASTNode("NEG");
        node->children.push_back(parseFactor());
        return node;
    }

    if (peek().type == NUMBER) {
        Token num = get();
        return new ASTNode("NUM", num.value);
    }

    if (peek().type == IDENTIFIER) {
        Token id = get();
        symTable.checkUse(id.value);
        return new ASTNode("ID", id.value);
    }

    if (peek().type == LPAREN) {
        get();
        ASTNode* node = parseExpr();
        match(RPAREN);
        return node;
    }

    cout << "Invalid factor\n";
    exit(1);
}