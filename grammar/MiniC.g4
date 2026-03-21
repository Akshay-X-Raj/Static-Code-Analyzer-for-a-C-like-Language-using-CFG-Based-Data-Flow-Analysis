grammar MiniC;

// ======================
// PROGRAM STRUCTURE
// ======================

program
    : function+ EOF
    ;

function
    : type ID '(' ')' block
    ;

// ======================
// STATEMENTS
// ======================

block
    : '{' statement* '}'
    ;

statement
    : declaration
    | assignment
    | ifStatement
    | whileStatement
    | forStatement
    | returnStatement
    | block
    ;

// ======================
// DECLARATIONS
// ======================

declaration
    : type ID ';'
    ;

// ======================
// ASSIGNMENT
// ======================

assignment
    : ID '=' expression ';'
    ;

// ======================
// CONTROL FLOW
// ======================

ifStatement
    : 'if' '(' expression ')' statement ('else' statement)?
    ;

whileStatement
    : 'while' '(' expression ')' statement
    ;

forStatement
    : 'for' '(' forInit? ';' expression? ';' forUpdate? ')' statement
    ;

forInit
    : declaration
    | assignment
    ;

forUpdate
    : assignment
    ;

returnStatement
    : 'return' expression ';'
    ;

// ======================
// EXPRESSIONS (PRECEDENCE SAFE)
// ======================

expression
    : logicalOrExpr
    ;

logicalOrExpr
    : logicalAndExpr ('||' logicalAndExpr)*
    ;

logicalAndExpr
    : equalityExpr ('&&' equalityExpr)*
    ;

equalityExpr
    : relationalExpr (('==' | '!=') relationalExpr)*
    ;

relationalExpr
    : additiveExpr (('<' | '>' | '<=' | '>=') additiveExpr)*
    ;

additiveExpr
    : multiplicativeExpr (('+' | '-') multiplicativeExpr)*
    ;

multiplicativeExpr
    : unaryExpr (('*' | '/') unaryExpr)*
    ;

unaryExpr
    : '!' unaryExpr
    | '-' unaryExpr
    | primary
    ;

primary
    : INT
    | FLOAT
    | CHAR
    | BOOL
    | ID
    | '(' expression ')'
    ;

// ======================
// TYPES
// ======================

type
    : 'int'
    | 'bool'
    | 'char'
    | 'float'
    ;

// ======================
// LEXER RULES
// ======================

BOOL
    : 'true'
    | 'false'
    ;

CHAR
    : '\'' . '\''
    ;

FLOAT
    : [0-9]+ '.' [0-9]+
    ;

INT
    : [0-9]+
    ;

ID
    : [a-zA-Z_][a-zA-Z0-9_]*
    ;

// ----------------------
// COMMENTS
// ----------------------

COMMENT
    : '//' ~[\r\n]* -> skip
    ;

COMMENT_MULTI
    : '/*' .*? '*/' -> skip
    ;

// ----------------------
// WHITESPACE
// ----------------------

WS
    : [ \t\r\n]+ -> skip
    ;
