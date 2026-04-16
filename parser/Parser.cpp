#include "Parser.h"
#include <iostream>
#include "../symbolTable/SymbolTable.h"

using namespace std;

extern SymbolTable symTable;

// ---------- CONSTRUCTOR ----------
Parser::Parser(vector<Token> t) {
    tokens = t;
    pos = 0;
}

// ---------- BASIC ----------
Token Parser::peek() {
    if (pos < tokens.size()) return tokens[pos];
    return {END_OF_FILE, "", 0};
}

Token Parser::get() {
    if (pos < tokens.size()) return tokens[pos++];
    return {END_OF_FILE, "", 0};
}
bool Parser::match(TokenType type) {
    if (peek().type == type) {
        get();
        return true;
    }
    return false;
}
// ---------- SYNTAX ERROR ----------
void syntaxError(string msg, int line) {
    cout << "Syntax Error at line " << line << ": " << msg << endl;
    exit(1);
}

// ---------- PROGRAM ----------
ASTNode* Parser::parseProgram() {
    ASTNode* root = new ASTNode("Program");

    while (peek().type != END_OF_FILE) {
        root->children.push_back(parseStmt());
    }

    return root;
}

// ---------- STATEMENT ----------
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

    if (peek().type == KW_WHILE)
        return parseWhile();

    if (peek().type == LBRACE)
        return parseBlock();

    syntaxError("Unknown statement near '" + peek().value + "'", peek().line);
    return nullptr;
}

// ---------- BLOCK ----------
ASTNode* Parser::parseBlock() {
    get(); // {

    ASTNode* block = new ASTNode("Block");

    while (peek().type != RBRACE && peek().type != END_OF_FILE) {
        block->children.push_back(parseStmt());
    }

    if (peek().type != RBRACE)
        syntaxError("Missing '}'", peek().line);

    get(); // }
    return block;
}

// ---------- DECL ----------
ASTNode* Parser::parseDecl() {
    Token type = get();
    Token id = get();

    if (id.type != IDENTIFIER) {
        syntaxError("Expected identifier", id.line);
    }

    symTable.add(id.value, type.value);

    if (peek().type != SEMICOLON) {
        syntaxError("Missing ';' after declaration", id.line);
    }

    get();

    ASTNode* node = new ASTNode("Decl", id.value);
    node->valueType = type.value;   

    return node;
}

// ---------- ASSIGN ----------
ASTNode* Parser::parseAssign() {

    Token id = get(); // variable

    match(ASSIGN);

    ASTNode* expr = parseExpr();

    Token semi = get();
    if (semi.type != SEMICOLON)
        syntaxError("Missing ';' after assignment", semi.line);

    // --------- TYPE DETECTION (FIX) ---------
    string exprType = "int";

    if (expr->type == "NUM") {
        if (expr->value.find('.') != string::npos)
            exprType = "float";
    }

    if (expr->type == "ID") {
        exprType = symTable.getType(expr->value);
    }

    if (expr->type == "OP") {
        // check right side too
        string leftType = "int";
        string rightType = "int";

        if (expr->children[0]->type == "NUM" &&
            expr->children[0]->value.find('.') != string::npos)
            leftType = "float";

        if (expr->children[1]->type == "NUM" &&
            expr->children[1]->value.find('.') != string::npos)
            rightType = "float";

        if (leftType == "float" || rightType == "float")
            exprType = "float";
    }

    // --------- FINAL TYPE CHECK ---------
    symTable.checkType(id.value, exprType);

    ASTNode* node = new ASTNode("Assign");
    node->children.push_back(new ASTNode("ID", id.value));
    node->children.push_back(expr);

    return node;
}
// ---------- PRINT ----------
ASTNode* Parser::parsePrint() {

    get(); // printf

    match(LPAREN);

    Token format = get(); // "%d"

    match(COMMA);

    Token var = get(); // variable

    match(RPAREN);

    Token semi = get();
    if (semi.type != SEMICOLON)
        syntaxError("Missing ';' after printf", semi.line);

    // --------- TYPE CHECK FIX ---------
    string varType = symTable.getType(var.value);

    if (format.value == "%d" && varType != "int") {
        cout << "Semantic Error: printf %d expects int but got " << varType << endl;
        exit(1);
    }

    if ((format.value == "%f" || format.value == "%lf") && varType == "int") {
        cout << "Semantic Error: printf expects float/double but got int\n";
        exit(1);
    }

    // --------- AST ----------
    ASTNode* node = new ASTNode("Print", format.value);
    node->children.push_back(new ASTNode("ID", var.value));

    return node;
}

// ---------- IF ----------
ASTNode* Parser::parseIf() {

    get(); // consume 'if'

    // CHECK '('
    if (!match(LPAREN)) {
        int line = (pos > 0) ? tokens[pos - 1].line : 0;
        syntaxError("Missing '(' after if", line);
    }

    // condition
    ASTNode* cond = parseCondition();

    // CHECK ')'
    if (!match(RPAREN)) {
        int line = (pos > 0) ? tokens[pos - 1].line : 0;
        syntaxError("Missing ')'", line);
    }

    // then statement
    ASTNode* thenStmt = parseStmt();

    // optional else
    ASTNode* elseStmt = nullptr;
    if (peek().type == KW_ELSE) {
        get();
        elseStmt = parseStmt();
    }

    ASTNode* node = new ASTNode("If");
    node->children.push_back(cond);
    node->children.push_back(thenStmt);

    if (elseStmt)
        node->children.push_back(elseStmt);

    return node;
}

// ---------- WHILE ----------
ASTNode* Parser::parseWhile() {
    get();

    if (peek().type != LPAREN)
        syntaxError("Expected '(' after while", peek().line);

    get();
    ASTNode* cond = parseCondition();

    if (peek().type != RPAREN)
        syntaxError("Expected ')'", peek().line);

    get();

    ASTNode* body = parseStmt();

    ASTNode* node = new ASTNode("While");
    node->children = {cond, body};

    return node;
}

// ---------- CONDITION ----------
ASTNode* Parser::parseCondition() {

    ASTNode* left = parseExpr();

    Token op = get();

    //  used REL_OP instead of LT, GT, etc
    if (op.type != REL_OP) {
        syntaxError("Invalid comparison operator", op.line);
    }

    ASTNode* right = parseExpr();

    ASTNode* node = new ASTNode("OP", op.value);
    node->children = {left, right};

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

    if (peek().type == NUMBER) {
        Token num = get();
        return new ASTNode("NUM", num.value);
    }

    if (peek().type == IDENTIFIER) {
        Token id = get();
        return new ASTNode("ID", id.value);
    }

    if (peek().type == LPAREN) {
        get();
        ASTNode* node = parseExpr();

        if (peek().type != RPAREN)
            syntaxError("Expected ')'", peek().line);

        get();
        return node;
    }

    syntaxError("Invalid expression", peek().line);
    return nullptr;
}