# Context-Free Grammar Rules

This defines the complete syntax accepted by our compiler frontend:

```text
<Program>      ::= <Statement> <Program> | EMPTY

<Statement>    ::= <Declaration> | <Assignment> | <IfElse> | <While> | <Print>

<Declaration>  ::= "int" IDENTIFIER ";"

<Assignment>   ::= IDENTIFIER "=" <Expression> ";"

<Expression>   ::= <Value> | <Value> "+" <Value>

<IfElse>       ::= "if" "(" IDENTIFIER ")" <Assignment> "else" <Assignment>

<While>        ::= "while" "(" IDENTIFIER ")" <Assignment>

<Print>        ::= "print" IDENTIFIER ";"

<Value>        ::= NUMBER | IDENTIFIER
```
