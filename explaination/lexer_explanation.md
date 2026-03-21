# Lexical Analysis (The Lexer)

The Lexer (also known as a scanner or tokenizer) is the very first phase of your compiler/interpreter! 

Before the Parser can understand grammar rules or build a Control Flow Graph, the raw text of the code (like `int x = 5;`) must be broken down into recognizable, meaningful chunks called **Tokens**. 

## Core Purpose
The `Lexer` reads the `test.c` file character by character and systematically groups those characters together. 

For example, if it sees `i`, `n`, `t`, it bundles them into a single `INT` token. If it sees `5`, it bundles it into a `NUMBER` token. It also completely ignores things that don't matter to the grammar, like spaces, tabs, and newlines!

## Understanding `Lexer.h` and `Lexer.cpp`

### 1. Token Definitions (from `Token.h`)
Before the Lexer can work, we must define what a "Token" is:
```cpp
enum TokenType { INT, IF, ELSE, IDENTIFIER, NUMBER, ASSIGN, SEMICOLON, LPAREN, RPAREN, END };

struct Token {
    TokenType type;
    string value;
};
```
Every token gets a `type` (so the Parser knows what it is, like `ASSIGN` for `=`) and a `value` (the exact raw text, like `"5"` or `"x"`).

### 2. State Management (`Lexer.h`)
```cpp
class Lexer {
    string code;    // The entire raw text of the file
    int pos = 0;    // Our current reading index
};
```
The Lexer class is extremely simple. It holds the entire source code string in memory and uses `pos` as a bookmark to remember exactly which character it is currently looking at.

### 3. Reading Helper Functions
- **`peek()`**: safely looks at the character at the current `pos` without moving the bookmark forward.
- **`get()`**: grabs the character and then advances `pos` by 1 to move on to the next one.
- **`skipWhitespace()`**: Continuously calls `get()` to silently fast-forward past spaces until it hits real code.

### 4. The Heart of the File: `nextToken()`
When `main.cpp` loops and calls `lexer.nextToken()`, the Lexer performs the following flowchart of checks:

1. **Skips whitespace**: Jumps past any formatting spaces so they don't corrupt the rules.
2. **Checks for End of File**: If it hits the end marker `\0`, it returns an `END` token to signal completion.
3. **Keywords & Identifiers**:
   If the character is a letter (`isalpha()`), it goes into a loop reading characters until the word ends. Then it checks:
   - Is the finished word exactly `"int"`, `"if"`, or `"else"`? If so, return a keyword token.
   - Otherwise, it must be a custom variable name. Return an `IDENTIFIER` token.
4. **Numbers**:
   If the character is a digit (`isdigit()`), it goes into a loop reading numbers and returns a `NUMBER` token.
5. **Symbols**:
   If it's an `=`, `;`, `(`, or `)`, it consumes exactly one character and returns the matching mathematical symbol token!

## Summary
The Lexer takes messy human text and produces a clean, mathematically organized sequence of `Token` structures. Your `main.cpp` loops over `nextToken()` and saves all these generated tokens into a massive `vector<Token>`. Finally, it hands this clean list of tokens directly over to the **Parser**, which handles all the Context-Free Grammar syntax logic!
