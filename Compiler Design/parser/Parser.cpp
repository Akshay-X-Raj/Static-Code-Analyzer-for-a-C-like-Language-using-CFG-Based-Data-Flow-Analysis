#include "Parser.h"
#include "../ast/ASTNode.h"

Parser::Parser(vector<Token> t) {
    tokens = t;
    pos = 0;
}

Token Parser::peek() {
    return tokens[pos];
}

Token Parser::get() {
    return tokens[pos++];
}

// 🔥 Parse assignment
ASTNode* parseAssign(Parser* p) {
    p->get(); // datatype

    Token var = p->get();
    p->get(); // =

    Token t1 = p->get();
    Token op = p->get();
    Token t2 = p->get();

    ASTNode* opNode = new ASTNode(op.value);
    opNode->left = new ASTNode(t1.value);
    opNode->right = new ASTNode(t2.value);

    ASTNode* root = new ASTNode("=");
    root->left = new ASTNode(var.value);
    root->right = opNode;

    return root;
}

// 🔥 Parse single statement
ASTNode* Parser::parseStatement() {

    Token first = peek();

    // 🔥 IF with body
    if (first.value == "if") {
        get(); // if

        ASTNode* node = new ASTNode("if");

        // next statement is body
        node->body = parseStatement();

        return node;
    }

    // 🔥 WHILE with body
    if (first.value == "while") {
        get(); // while

        ASTNode* node = new ASTNode("while");

        node->body = parseStatement();

        return node;
    }

    // 🔥 assignment
    return parseAssign(this);
}

// 🔥 Parse full program
vector<ASTNode*> Parser::parseProgram() {
    vector<ASTNode*> program;

    while (peek().type != END) {
        program.push_back(parseStatement());

        if (peek().value == ";") get();
    }

    return program;
}