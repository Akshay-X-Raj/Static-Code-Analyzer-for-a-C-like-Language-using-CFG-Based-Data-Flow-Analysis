#include "Parser.h"
#include <iostream>
#include "../symbolTable/SymbolTable.h"

using namespace std;

extern SymbolTable symTable;

// ---------- TYPE DETECTION (NEW) ----------
string getExprType(ASTNode* node) {

    // NUMBER
    if (node->type == "NUM") {
        if (node->value.find('.') != string::npos)
            return "float";
        return "int";
    }

    // VARIABLE
    if (node->type == "ID") {
        return symTable.getType(node->value);
    }

    // OPERATION
    if (node->type == "OP") {
        string left = getExprType(node->children[0]);
        string right = getExprType(node->children[1]);

        if (left == "double" || right == "double") return "double";
        if (left == "float" || right == "float") return "float";
        return "int";
    }

    return "int";
}

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

    Token type = get();  // int / float / etc

    ASTNode* declNode = new ASTNode("DeclList");

    while (true) {

        // check before consuming
        if (peek().type != IDENTIFIER) {
            syntaxError("Expected identifier", peek().line);
        }

        Token id = get();

        // add to symbol table
        symTable.add(id.value, type.value);

        // create node
        ASTNode* varNode = new ASTNode("Decl", id.value);
        varNode->valueType = type.value;
        declNode->children.push_back(varNode);

        // check next token
        if (peek().type == COMMA) {
            get(); // consume ','

            if (peek().type != IDENTIFIER) {
                syntaxError("Expected identifier after ','", peek().line);
            }

            continue;
        }

        else if (peek().type == SEMICOLON) {
            get(); // consume ';'
            break;
        }

        else {
            syntaxError("Missing ';' after declaration", id.line);
        }
    }

    return declNode;
}
// ---------- ASSIGN (FIXED) ----------
ASTNode* Parser::parseAssign() {

    Token id = get(); // variable

    match(ASSIGN);

    ASTNode* expr = parseExpr();

    Token semi = get();
    if (semi.type != SEMICOLON)
        syntaxError("Missing ';' after assignment", semi.line);

    // --------- CORRECT TYPE DETECTION ---------
    string exprType = getExprType(expr);

    // --------- TYPE CHECK ---------
    symTable.checkType(id.value, exprType);

    ASTNode* node = new ASTNode("Assign");
    node->children.push_back(new ASTNode("ID", id.value));
    node->children.push_back(expr);

    return node;
}

// ---------- PRINT ----------
ASTNode* Parser::parsePrint() {

    get(); // printf

    if (!match(LPAREN)) {
    syntaxError("Missing '(' before printf", peek().line);
    }

    Token format = get(); // "%d"

    if (!match(COMMA)) {
    syntaxError("Missing ',' in printf", peek().line);
    }

    Token var = get(); // variable

    if (!match(RPAREN)) {
    syntaxError("Missing ')' in printf", peek().line);
    }

    Token semi = get();
    if (semi.type != SEMICOLON)
        syntaxError("Missing ';' after printf", semi.line);

    string varType = symTable.getType(var.value);

    if (format.value == "%d" && varType != "int") {
        cout << "Semantic Error: printf %d expects int but got " << varType << endl;
        exit(1);
    }

    if ((format.value == "%f" || format.value == "%lf") && varType == "int") {
        cout << "Semantic Error: printf expects float/double but got int\n";
        exit(1);
    }

    ASTNode* node = new ASTNode("Print", format.value);
    node->children.push_back(new ASTNode("ID", var.value));

    return node;
}

// ---------- IF ----------
ASTNode* Parser::parseIf() {

    get();

    if (!match(LPAREN)) {
        int line = (pos > 0) ? tokens[pos - 1].line : 0;
        syntaxError("Missing '(' after if", line);
    }

    ASTNode* cond = parseCondition();

    if (!match(RPAREN)) {
        int line = (pos > 0) ? tokens[pos - 1].line : 0;
        syntaxError("Missing ')'", line);
    }

    ASTNode* thenStmt = parseStmt();

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