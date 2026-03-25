/*
 * CONSOLIDATED PARSER IMPLEMENTATION
 * Combines: Lexer + ParserDriver + Token definitions
 * This single file handles all tokenization and parsing
 */

#pragma once
#include <string>
#include <vector>
#include <memory>
#include <cctype>
using namespace std;

//============================================================
// TOKEN DEFINITIONS (was: Token.h)
//============================================================

enum TokenType {
    // Keywords
    KW_INT, KW_BOOL, KW_CHAR, KW_FLOAT,
    KW_TRUE, KW_FALSE,
    KW_IF, KW_ELSE, KW_WHILE, KW_FOR, KW_RETURN,

    // Identifiers and Literals
    ID, INT_LITERAL, FLOAT_LITERAL, CHAR_LITERAL,

    // Operators
    ASSIGN, LOGICAL_OR, LOGICAL_AND, EQ, NEQ, LT, GT, LTE, GTE,
    PLUS, MINUS, MUL, DIV, NOT,

    // Punctuation
    SEMI, LPAREN, RPAREN, LBRACE, RBRACE,

    // End of File
    END_OF_FILE
};

struct Token {
    TokenType type;
    string value;
};

//============================================================
// LEXER (was: Lexer.h/Lexer.cpp)
//============================================================

class Lexer {
    string code;
    size_t pos = 0;

public:
    Lexer(string input) : code(input), pos(0) {}

    char peek() {
        return (pos < code.length()) ? code[pos] : '\0';
    }

    char peekNext() {
        return (pos + 1 < code.length()) ? code[pos + 1] : '\0';
    }

    char get() {
        return code[pos++];
    }

    void skipWhitespaceAndComments() {
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

    Token nextToken() {
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

            if      (word == "int")     t.type = KW_INT;
            else if (word == "bool")    t.type = KW_BOOL;
            else if (word == "char")    t.type = KW_CHAR;
            else if (word == "float")   t.type = KW_FLOAT;
            else if (word == "true")    t.type = KW_TRUE;
            else if (word == "false")   t.type = KW_FALSE;
            else if (word == "if")      t.type = KW_IF;
            else if (word == "else")    t.type = KW_ELSE;
            else if (word == "while")   t.type = KW_WHILE;
            else if (word == "for")     t.type = KW_FOR;
            else if (word == "return")  t.type = KW_RETURN;
            else                        t.type = ID;

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
};

//============================================================
// AST NODES (was: ASTNode.h, Stmt.h, Expr.h)
//============================================================

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class Expr : public ASTNode {
public:
    virtual ~Expr() = default;
};

class Stmt : public ASTNode {
public:
    virtual ~Stmt() = default;
};

// Expressions
class BinaryExpr : public Expr {
public:
    unique_ptr<Expr> left;
    Token op;
    unique_ptr<Expr> right;
    BinaryExpr(unique_ptr<Expr> l, Token o, unique_ptr<Expr> r)
        : left(move(l)), op(o), right(move(r)) {}
};

class UnaryExpr : public Expr {
public:
    Token op;
    unique_ptr<Expr> right;
    UnaryExpr(Token o, unique_ptr<Expr> r) : op(o), right(move(r)) {}
};

class PrimaryExpr : public Expr {
public:
    Token value;
    PrimaryExpr(Token v) : value(v) {}
};

// Statements
class BlockStmt : public Stmt {
public:
    vector<unique_ptr<Stmt>> statements;
    BlockStmt(vector<unique_ptr<Stmt>> stmts) : statements(move(stmts)) {}
};

class IfStmt : public Stmt {
public:
    unique_ptr<Expr> condition;
    unique_ptr<Stmt> thenBranch;
    unique_ptr<Stmt> elseBranch;
    IfStmt(unique_ptr<Expr> cond, unique_ptr<Stmt> thenB, unique_ptr<Stmt> elseB)
        : condition(move(cond)), thenBranch(move(thenB)), elseBranch(move(elseB)) {}
};

class WhileStmt : public Stmt {
public:
    unique_ptr<Expr> condition;
    unique_ptr<Stmt> body;
    WhileStmt(unique_ptr<Expr> cond, unique_ptr<Stmt> b)
        : condition(move(cond)), body(move(b)) {}
};

class ForStmt : public Stmt {
public:
    unique_ptr<Stmt> initializer;
    unique_ptr<Expr> condition;
    unique_ptr<Stmt> update;
    unique_ptr<Stmt> body;
    ForStmt(unique_ptr<Stmt> init, unique_ptr<Expr> cond, unique_ptr<Stmt> up, unique_ptr<Stmt> b)
        : initializer(move(init)), condition(move(cond)), update(move(up)), body(move(b)) {}
};

class ReturnStmt : public Stmt {
public:
    unique_ptr<Expr> value;
    ReturnStmt(unique_ptr<Expr> val) : value(move(val)) {}
};

class DeclarationStmt : public Stmt {
public:
    Token type;
    Token name;
    DeclarationStmt(Token t, Token n) : type(t), name(n) {}
};

class AssignmentStmt : public Stmt {
public:
    Token name;
    unique_ptr<Expr> value;
    AssignmentStmt(Token n, unique_ptr<Expr> val) : name(n), value(move(val)) {}
};

class FunctionNode : public ASTNode {
public:
    Token type;
    Token name;
    unique_ptr<BlockStmt> body;
    FunctionNode(Token t, Token n, unique_ptr<BlockStmt> b)
        : type(t), name(n), body(move(b)) {}
};

class ProgramNode : public ASTNode {
public:
    vector<unique_ptr<FunctionNode>> functions;
};

//============================================================
// PARSER (was: ParserDriver.h/ParserDriver.cpp)
//============================================================

class ParserDriver {
    vector<Token> tokens;
    size_t pos = 0;

public:
    ParserDriver(vector<Token> t) : tokens(t), pos(0) {}

    Token peek() {
        return (pos < tokens.size()) ? tokens[pos] : Token{END_OF_FILE, ""};
    }

    Token get() {
        return tokens[pos++];
    }

    bool match(TokenType type) {
        if (peek().type == type) {
            get();
            return true;
        }
        return false;
    }

    Token consume(TokenType type, const string& err) {
        if (peek().type != type) {
            cout << "[ERROR] " << err << "\n";
        }
        return get();
    }

    bool isTypeToken(TokenType t) {
        return t == KW_INT || t == KW_BOOL || t == KW_CHAR || t == KW_FLOAT;
    }

    unique_ptr<ProgramNode> parseProgram() {
        auto program = make_unique<ProgramNode>();
        while (peek().type != END_OF_FILE) {
            program->functions.push_back(parseFunction());
        }
        return program;
    }

private:
    unique_ptr<FunctionNode> parseFunction() {
        Token type = consume(KW_INT, "Expected type");
        Token name = consume(ID, "Expected function name");
        consume(LPAREN, "Expected (");
        consume(RPAREN, "Expected )");
        auto body = parseBlock();
        return make_unique<FunctionNode>(type, name, move(body));
    }

    unique_ptr<BlockStmt> parseBlock() {
        consume(LBRACE, "Expected {");
        vector<unique_ptr<Stmt>> stmts;
        while (peek().type != RBRACE && peek().type != END_OF_FILE) {
            stmts.push_back(parseStatement());
        }
        consume(RBRACE, "Expected }");
        return make_unique<BlockStmt>(move(stmts));
    }

    unique_ptr<Stmt> parseStatement() {
        if (isTypeToken(peek().type)) return parseDeclaration();
        if (peek().type == ID) return parseAssignment();
        if (peek().type == KW_IF) return parseIfStatement();
        if (peek().type == KW_WHILE) return parseWhileStatement();
        if (peek().type == KW_FOR) return parseForStatement();
        if (peek().type == KW_RETURN) return parseReturnStatement();
        if (peek().type == LBRACE) return parseBlock();
        get();
        return nullptr;
    }

    unique_ptr<Stmt> parseDeclaration() {
        Token type = get();
        Token name = get();
        consume(SEMI, "Expected ;");
        return make_unique<DeclarationStmt>(type, name);
    }

    unique_ptr<Stmt> parseAssignment() {
        Token name = get();
        consume(ASSIGN, "Expected =");
        auto value = parseExpression();
        consume(SEMI, "Expected ;");
        return make_unique<AssignmentStmt>(name, move(value));
    }

    unique_ptr<Stmt> parseIfStatement() {
        consume(KW_IF, "Expected if");
        consume(LPAREN, "Expected (");
        auto cond = parseExpression();
        consume(RPAREN, "Expected )");
        auto thenBranch = parseStatement();
        unique_ptr<Stmt> elseBranch = nullptr;
        if (match(KW_ELSE)) {
            elseBranch = parseStatement();
        }
        return make_unique<IfStmt>(move(cond), move(thenBranch), move(elseBranch));
    }

    unique_ptr<Stmt> parseWhileStatement() {
        consume(KW_WHILE, "Expected while");
        consume(LPAREN, "Expected (");
        auto cond = parseExpression();
        consume(RPAREN, "Expected )");
        auto body = parseStatement();
        return make_unique<WhileStmt>(move(cond), move(body));
    }

    unique_ptr<Stmt> parseForStatement() {
        consume(KW_FOR, "Expected for");
        consume(LPAREN, "Expected (");
        unique_ptr<Stmt> init = nullptr;
        if (isTypeToken(peek().type) || peek().type == ID) {
            init = parseStatement();
        } else {
            get(); // consume ;
        }
        auto cond = parseExpression();
        consume(SEMI, "Expected ;");
        unique_ptr<Stmt> update = nullptr;
        if (peek().type != RPAREN) {
            update = parseStatement();
        } else {
            get(); // consume )
        }
        auto body = parseStatement();
        return make_unique<ForStmt>(move(init), move(cond), move(update), move(body));
    }

    unique_ptr<Stmt> parseReturnStatement() {
        consume(KW_RETURN, "Expected return");
        auto value = parseExpression();
        consume(SEMI, "Expected ;");
        return make_unique<ReturnStmt>(move(value));
    }

    unique_ptr<Expr> parseExpression() {
        return parseLogicalOrExpr();
    }

    unique_ptr<Expr> parseLogicalOrExpr() {
        auto left = parseLogicalAndExpr();
        while (peek().type == LOGICAL_OR) {
            Token op = get();
            auto right = parseLogicalAndExpr();
            left = make_unique<BinaryExpr>(move(left), op, move(right));
        }
        return left;
    }

    unique_ptr<Expr> parseLogicalAndExpr() {
        auto left = parseEqualityExpr();
        while (peek().type == LOGICAL_AND) {
            Token op = get();
            auto right = parseEqualityExpr();
            left = make_unique<BinaryExpr>(move(left), op, move(right));
        }
        return left;
    }

    unique_ptr<Expr> parseEqualityExpr() {
        auto left = parseRelationalExpr();
        while (peek().type == EQ || peek().type == NEQ) {
            Token op = get();
            auto right = parseRelationalExpr();
            left = make_unique<BinaryExpr>(move(left), op, move(right));
        }
        return left;
    }

    unique_ptr<Expr> parseRelationalExpr() {
        auto left = parseAdditiveExpr();
        while (peek().type == LT || peek().type == GT || peek().type == LTE || peek().type == GTE) {
            Token op = get();
            auto right = parseAdditiveExpr();
            left = make_unique<BinaryExpr>(move(left), op, move(right));
        }
        return left;
    }

    unique_ptr<Expr> parseAdditiveExpr() {
        auto left = parseMultiplicativeExpr();
        while (peek().type == PLUS || peek().type == MINUS) {
            Token op = get();
            auto right = parseMultiplicativeExpr();
            left = make_unique<BinaryExpr>(move(left), op, move(right));
        }
        return left;
    }

    unique_ptr<Expr> parseMultiplicativeExpr() {
        auto left = parseUnaryExpr();
        while (peek().type == MUL || peek().type == DIV) {
            Token op = get();
            auto right = parseUnaryExpr();
            left = make_unique<BinaryExpr>(move(left), op, move(right));
        }
        return left;
    }

    unique_ptr<Expr> parseUnaryExpr() {
        if (peek().type == NOT || peek().type == MINUS) {
            Token op = get();
            auto right = parseUnaryExpr();
            return make_unique<UnaryExpr>(op, move(right));
        }
        return parsePrimary();
    }

    unique_ptr<Expr> parsePrimary() {
        if (peek().type == INT_LITERAL || peek().type == ID) {
            return make_unique<PrimaryExpr>(get());
        }
        if (peek().type == LPAREN) {
            get();
            auto expr = parseExpression();
            consume(RPAREN, "Expected )");
            return expr;
        }
        return make_unique<PrimaryExpr>(Token{ID, "unknown"});
    }
};
