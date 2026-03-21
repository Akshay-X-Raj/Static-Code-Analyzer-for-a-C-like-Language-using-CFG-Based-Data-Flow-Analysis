#include "ParserDriver.h"
#include <iostream>

ParserDriver::ParserDriver(vector<Token> t) : tokens(t) {}

Token ParserDriver::peek() {
    if (pos < tokens.size()) return tokens[pos];
    Token eofToken; eofToken.type = END_OF_FILE; eofToken.value = "";
    return eofToken;
}

Token ParserDriver::get() {
    Token c = peek();
    if (pos < tokens.size()) pos++;
    return c;
}

bool ParserDriver::match(TokenType type) {
    if (peek().type == type) {
        pos++;
        return true;
    }
    return false;
}

Token ParserDriver::consume(TokenType type, const string& err) {
    if (peek().type == type) return get();
    cerr << "[Parse Error] " << err << " (found " << peek().value << ")" << endl;
    // In a real compiler, we would throw an exception or panic mode recover.
    return Token{END_OF_FILE, ""};
}

bool ParserDriver::isTypeToken(TokenType t) {
    return t == KW_INT || t == KW_BOOL || t == KW_CHAR || t == KW_FLOAT;
}

unique_ptr<ProgramNode> ParserDriver::parseProgram() {
    auto prog = make_unique<ProgramNode>();
    while (peek().type != END_OF_FILE) {
        prog->functions.push_back(parseFunction());
    }
    return prog;
}

unique_ptr<FunctionNode> ParserDriver::parseFunction() {
    Token type = get(); // type token
    if (!isTypeToken(type.type)) {
        cerr << "[Parse Error] Expected function return type" << endl;
    }
    Token name = consume(ID, "Expected function name");
    consume(LPAREN, "Expected '(' after function name");
    consume(RPAREN, "Expected ')' after function arguments (args not supported in mini-grammar)");
    auto body = parseBlock();
    return make_unique<FunctionNode>(type, name, std::move(body));
}

unique_ptr<BlockStmt> ParserDriver::parseBlock() {
    consume(LBRACE, "Expected '{' to start block");
    vector<unique_ptr<Stmt>> stmts;
    while (peek().type != RBRACE && peek().type != END_OF_FILE) {
        stmts.push_back(parseStatement());
    }
    consume(RBRACE, "Expected '}' to end block");
    return make_unique<BlockStmt>(std::move(stmts));
}

unique_ptr<Stmt> ParserDriver::parseStatement() {
    TokenType t = peek().type;
    if (t == LBRACE) return parseBlock();
    if (t == KW_IF) return parseIfStatement();
    if (t == KW_WHILE) return parseWhileStatement();
    if (t == KW_FOR) return parseForStatement();
    if (t == KW_RETURN) return parseReturnStatement();
    if (isTypeToken(t)) return parseDeclaration();
    return parseAssignment();
}

unique_ptr<Stmt> ParserDriver::parseDeclaration() {
    Token type = get();
    Token name = consume(ID, "Expected variable name in declaration");
    consume(SEMI, "Expected ';' after declaration");
    return make_unique<DeclarationStmt>(type, name);
}

unique_ptr<Stmt> ParserDriver::parseAssignment() {
    Token name = consume(ID, "Expected identifier for assignment");
    consume(ASSIGN, "Expected '=' in assignment");
    auto expr = parseExpression();
    consume(SEMI, "Expected ';' after assignment");
    return make_unique<AssignmentStmt>(name, std::move(expr));
}

unique_ptr<Stmt> ParserDriver::parseIfStatement() {
    consume(KW_IF, "Expected 'if'");
    consume(LPAREN, "Expected '(' after 'if'");
    auto cond = parseExpression();
    consume(RPAREN, "Expected ')' after if condition");
    auto thenB = parseStatement();
    unique_ptr<Stmt> elseB = nullptr;
    if (match(KW_ELSE)) {
        elseB = parseStatement();
    }
    return make_unique<IfStmt>(std::move(cond), std::move(thenB), std::move(elseB));
}

unique_ptr<Stmt> ParserDriver::parseWhileStatement() {
    consume(KW_WHILE, "Expected 'while'");
    consume(LPAREN, "Expected '(' after 'while'");
    auto cond = parseExpression();
    consume(RPAREN, "Expected ')' after while condition");
    auto body = parseStatement();
    return make_unique<WhileStmt>(std::move(cond), std::move(body));
}

unique_ptr<Stmt> ParserDriver::parseForStatement() {
    consume(KW_FOR, "Expected 'for'");
    consume(LPAREN, "Expected '(' after 'for'");
    
    unique_ptr<Stmt> init = nullptr;
    if (peek().type != SEMI) {
        if (isTypeToken(peek().type)) init = parseDeclaration();
        else init = parseAssignment(); // assignment consumes SEMI, wait, our assignment grammar says ID '=' expression ';'
        // For statement specifies `forInit? ';'` but Assignment/Decl already eat ';'
    } else {
        consume(SEMI, "Expected ';'");
    }

    unique_ptr<Expr> cond = nullptr;
    if (peek().type != SEMI) {
        cond = parseExpression();
    }
    consume(SEMI, "Expected ';' after for condition");

    unique_ptr<Stmt> update = nullptr;
    if (peek().type != RPAREN) {
        // "assignment without semicolon" usually, but grammar says "assignment"
        // Let's parse assignment but since we wrote parseAssignment to eat ';', we need to be careful.
        // The user grammar says `forUpdate : assignment ;`, `assignment : ID '=' expression ';' ;`
        // Meaning for(int i=0; i<10; i = i + 1;) which is weird C syntax.
        // I will just use parseAssignment and accept the `;` inside the loop header.
        update = parseAssignment();
    }
    
    // In standard C, the update has no semicolon. But we follow strict provided grammar:
    // `forUpdate : assignment ;` where assignment has `;`
    // Actually the provided grammar says `for(forInit? ; expression? ; forUpdate?)`
    // wait, if assignment eats `;`, then `forInit` eating `;` aligns with `for(forInit expression? ;`...
    // Let's just consume RPAREN
    consume(RPAREN, "Expected ')' after for clauses");
    auto body = parseStatement();
    return make_unique<ForStmt>(std::move(init), std::move(cond), std::move(update), std::move(body));
}

unique_ptr<Stmt> ParserDriver::parseReturnStatement() {
    consume(KW_RETURN, "Expected 'return'");
    auto expr = parseExpression();
    consume(SEMI, "Expected ';' after return value");
    return make_unique<ReturnStmt>(std::move(expr));
}

unique_ptr<Expr> ParserDriver::parseExpression() {
    return parseLogicalOrExpr();
}

unique_ptr<Expr> ParserDriver::parseLogicalOrExpr() {
    auto expr = parseLogicalAndExpr();
    while (peek().type == LOGICAL_OR) {
        Token op = get();
        auto right = parseLogicalAndExpr();
        expr = make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

unique_ptr<Expr> ParserDriver::parseLogicalAndExpr() {
    auto expr = parseEqualityExpr();
    while (peek().type == LOGICAL_AND) {
        Token op = get();
        auto right = parseEqualityExpr();
        expr = make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

unique_ptr<Expr> ParserDriver::parseEqualityExpr() {
    auto expr = parseRelationalExpr();
    while (peek().type == EQ || peek().type == NEQ) {
        Token op = get();
        auto right = parseRelationalExpr();
        expr = make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

unique_ptr<Expr> ParserDriver::parseRelationalExpr() {
    auto expr = parseAdditiveExpr();
    while (peek().type == LT || peek().type == GT || peek().type == LTE || peek().type == GTE) {
        Token op = get();
        auto right = parseAdditiveExpr();
        expr = make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

unique_ptr<Expr> ParserDriver::parseAdditiveExpr() {
    auto expr = parseMultiplicativeExpr();
    while (peek().type == PLUS || peek().type == MINUS) {
        Token op = get();
        auto right = parseMultiplicativeExpr();
        expr = make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

unique_ptr<Expr> ParserDriver::parseMultiplicativeExpr() {
    auto expr = parseUnaryExpr();
    while (peek().type == MUL || peek().type == DIV) {
        Token op = get();
        auto right = parseUnaryExpr();
        expr = make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

unique_ptr<Expr> ParserDriver::parseUnaryExpr() {
    if (peek().type == NOT || peek().type == MINUS) {
        Token op = get();
        auto right = parseUnaryExpr();
        return make_unique<UnaryExpr>(op, std::move(right));
    }
    return parsePrimary();
}

unique_ptr<Expr> ParserDriver::parsePrimary() {
    Token t = get();
    if (t.type == INT_LITERAL || t.type == FLOAT_LITERAL || t.type == CHAR_LITERAL || 
        t.type == KW_TRUE || t.type == KW_FALSE || t.type == ID) {
        return make_unique<PrimaryExpr>(t);
    }
    if (t.type == LPAREN) {
        auto expr = parseExpression();
        consume(RPAREN, "Expected ')' after expression");
        return expr;
    }
    cerr << "[Parse Error] Expected primary expression, found " << t.value << endl;
    return nullptr;
}
