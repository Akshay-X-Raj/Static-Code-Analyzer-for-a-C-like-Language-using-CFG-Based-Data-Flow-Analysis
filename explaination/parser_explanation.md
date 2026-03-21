# Syntax Analysis (The Parser)

If the Lexer is the component that functionally reads "words" (tokens), then the **Parser** is the component that reads "sentences" (grammar). 

The Parser's job is to take the long flat list of Tokens that the Lexer generated, mathematically verify that they follow the exact structural rules of your **Context-Free Grammar**, and execute the logic. In this project, it specifically builds out your Control Flow Graph (CFG) and populates your Symbol Table to track variables!

## Core Architecture (`Parser.h`)

```cpp
class Parser {
    vector<Token> tokens; // The complete list of tokens from the Lexer
    int pos = 0;          // Our current reading index in the token list
};
```
Just like the Lexer tracked its position in a giant string of raw characters, the Parser tracks its position in a giant vector of complete `Token` structures. 

It also utilizes two extremely simple array helper functions:
- **`peek()`**: Safely looks at the current token without moving `pos` forward.
- **`get()`**: Grabs the current token and forcefully advances `pos` by 1 to move to the next token.

## How It Evaluates Grammar (`Parser.cpp`)

Your parser is designed as a **Recursive Descent Parser**. This means that for every single formal rule defined in your Context-Free Grammar, there is an exact, 1-to-1 C++ function that matches it!

### 1. Variables and The Symbol Table
Let's look at `parseDeclaration()`. This handles grammar syntax like `int x;`.

```cpp
// RULE: <Declaration> ::= "int" IDENTIFIER ";"
void Parser::parseDeclaration() {
    get(); // Consumes the 'INT' token
    Token ident = get(); // Consumes and saves the exact IDENTIFIER token (like 'x')

    // Add 'x' to our global tracking map!
    if (symbolTable.count(ident.value) == 0) {
        Symbol s;
        s.name = ident.value;
        s.initialized = false;
        symbolTable[ident.value] = s;
    }

    get(); // Consumes the ';' token
}
```
If the parser's routing logic sees an `int` token, it immediately jumps into this function. It linearly consumes the identifier and the semicolon, and crucially registers the new variable in the global `symbolTable` so the system knows the variable exists!

### 2. Assignments
The `parseAssignment()` function handles syntax lines like `x = 5;`. 
It identically consumes the `ident` token (`x`), the `=` token, and the `value` token (`5`), and the `;` token. Then it dynamically looks up `x` in the `symbolTable` and correctly updates its flag to `s.initialized = true` since it now legally holds a mathematical value!

### 3. The Control Flow Graph (If-Else)
The `parseIfElse()` function is the most complex function because it handles entire routing branches of code, and it is responsible for physically building the CFG graph!

1. It consumes `if` and `(`.
2. It grabs the condition token and immediately creates a mathematical `createBlock()` holding the branch test (`if (x)`).
3. It seamlessly grabs the inner code for the **True Branch** and creates a block for it.
4. It consumes the `else` token.
5. It grabs the inner code for the **False Branch** and creates a block for it.
6. Finally, it creates an `END` block and uses the `cfg[...].next.push_back(...)` vector logic to physically draw the edge arrows linking the test block down to the correct branches, and the branches back into the final convergence node!

## Summary
The Parser loops through `parseProgram() -> parseStatement()`, constantly redirecting traffic to the appropriate grammatical sub-function based on whether `peek()` reveals the next token is an `int`, an `if`, or an `Identifier`. 
It simultaneously enforces perfect grammar correctness while beautifully extracting the code into functional memory tables and flowchart structures!
