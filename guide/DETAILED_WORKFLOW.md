# Detailed Project Workflow and Explanation

This document provides a comprehensive, step-by-step explanation of exactly what your **Static Code Analyzer** project is doing under the hood when you run `.\build\static_analyzer.exe`.

---

## High-Level Overview
Your project is a **Compiler Frontend and Analysis Backend**. When you feed it a C program (like `test.c`), it does **not** run the program. Instead, it reads the text like a book, understands the grammar, builds a mathematical model of how the code executes, and points out logical flaws (like unreachable code) or syntax mistakes.

Here is the exact step-by-step pipeline of what happens inside your C++ code.

---

## Step 1: Lexical Analysis (Lexing)
**What it does:**
The Lexer reads the raw text of your `.c` file character by character. It strips away all spaces, newlines, and comments. It groups the remaining characters into meaningful "words" called **Tokens**.
* **Example Input:** `int y = 5;`
* **Lexer Output:** `[KEYWORD: int] [IDENTIFIER: y] [OPERATOR: =] [LITERAL: 5] [SYMBOL: ;]`

**As seen in your terminal:**
> `Lexing completed.`

## Step 2: Syntax Analysis (Parsing)
**What it does:**
The Parser takes the stream of tokens from the Lexer and checks if they follow the grammatical rules of the C language. It groups the tokens together to form expressions and statements, building a tree-like structure called an **Abstract Syntax Tree (AST)**. 
If you forget a semicolon or type something out of order, the parser catches it here because the grammar rules are broken.

**As seen in your terminal:**
> `[Parse Error] Expected ';' after declaration (found =)`
> `[Parse Error] Expected identifier for assignment (found =)`
> `Parsing completed.`

*Explanation of your error:* Your test code likely has a malformed line (maybe something like `int = 5;` or a missing semicolon on the previous line). The parser detected that the sequence of tokens did not match valid C grammar.

## Step 3: Semantic Analysis & Symbol Table
**What it does:**
While the Parser checks *grammar*, the Semantic Analyzer checks *meaning*. It uses a **Symbol Table** to keep track of every variable you declare, its data type, and the scope it belongs to (e.g., inside a specific function or `if` block).
When you try to use a variable, the Semantic Analyzer looks it up in the Symbol Table. If it's not there, it throws an error.

**As seen in your terminal:**
> `[Semantic Error] Use of undeclared variable:`
> `Semantic constraints evaluated.`

*Explanation of your error:* You tried to assign a value to a variable that hasn't been declared with `int` or `float` earlier in its scope.

## Step 4: Control Flow Graph (CFG) Construction
**What it does:**
Now that the AST is built and semantically verified, the analyzer transforms it into a **Control Flow Graph (CFG)**. The CFG breaks your code down into "Basic Blocks". A basic block is a straight-line sequence of code with no branches (no `if` statements, no `loops`). 
When a branch occurs (like `if (y > 0)`), the CFG splits into two paths: one path if the condition is true, and another if it is false.

**As seen in your terminal:**
```text
CFG STRUCTURE:
Block 1: Func main
   -> Block 3
Block 3: int x
   -> Block 4
Block 4: int y
   -> Block 5
Block 6: if (y > 0)
   -> Block 8  (True Path)
   -> Block 7  (False Path)
...
Block 9: return 0
   -> Block 2  (End of Function)
   -> Block 10 (Unreachable path created in memory!)
```
*(Notice how Block 9 `return 0` tries to jump to Block 2 which is `EndFunc`, but since there is code written *after* the return statement, the CFG also mapped a Block 10. We can clearly see structurally that Block 10 can never be reached naturally, which is how Dead Code detection works!)*

## Step 5: Data-Flow Analysis
**What it does:**
This is the core mathematical engine of your analyzer. It traverses the CFG blocks created in Step 4 to track the lifecycle of variables across all possible execution paths.
* **Unused Variables:** It checks if a variable is put into the Symbol Table but never appears on the right side of an equation or in a print statement across any path. (e.g., `int x;` in Block 3 is never used).
* **Unreachable Code:** It checks if any block in the CFG has no incoming arrows from previous blocks (like code written after a `return` statement).
* **Uninitialized Variables:** It checks paths to ensure a variable is assigned a value *before* it is used in an equation.

## Step 6: DOT File Exportation
**What it does:**
Finally, the analyzer takes the CFG structure mapped in memory and translates it into the DOT language format. It writes this out to a file called `cfg.dot`.

**As seen in your terminal:**
> `DOT file exported -> cfg.dot`

This DOT file is just raw text defining nodes and arrows. When you run the Graphviz tool (`dot -Tpng cfg.dot -o cfg_visual.png`), Graphviz reads those text instructions and draws the actual flowchart image you see in the PNG.

---

### In Summary
Your project successfully emulates exactly how a real-world compiler (like GCC or Clang) processes code before translating it into machine code. It proves you understand how to tokenize raw text, computationally evaluate grammatical rules, memory-map execution paths, and algorithmically detect logical errors!
