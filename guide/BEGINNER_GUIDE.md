# Beginner's Guide to Static Code Analyzer

## Overview

This project analyzes C-like code to find bugs WITHOUT running it. Think of it as a smart checker that reads your code and finds problems like:
- Variables used before being initialized
- Variables declared but never used
- Code that can never be reached
- Variables used without being declared

## The Program Flow (5 Simple Steps)

```
Your Code (test.c)
    ↓
1️⃣ LEXER - Break code into tokens
    ↓
2️⃣ PARSER - Build Abstract Syntax Tree (AST)
    ↓
3️⃣ SEMANTIC ANALYZER - Check that code follows rules
    ↓
4️⃣ CFG BUILDER - Create Control Flow Graph
    ↓
5️⃣ DATA FLOW ANALYSIS - Find bugs
    ↓
Reports and Warnings
```

---

## Understanding Each Step

### Step 1: LEXER (src/parser/Lexer.h)
**What it does:** Converts raw text into tokens

**Example:**
```
Input:  "int x = 5;"
Output: [KW_INT], [ID("x")], [ASSIGN], [INT_LITERAL("5")], [SEMI]
```

**Key concept:** A token is the smallest meaningful piece of code
- Keywords: `int`, `bool`, `if`, `while`, etc.
- Identifiers: Variable names like `x`, `count`, `result`
- Operators: `+`, `-`, `==`, `&&`
- Literals: Numbers like `5`, `3.14`, characters like `'a'`

---

### Step 2: PARSER (src/parser/ParserDriver.h)
**What it does:** Takes tokens and builds a tree structure

**Example:**
```
Input tokens: [KW_INT] [ID("x")] [ASSIGN] [INT_LITERAL("5")] [SEMI]

Output AST:
    DeclarationStmt
        ├── type: int
        ├── name: x
```

**Key concept:** The AST is like an outline of your code's structure
- It respects grammar rules (you can't have `int ;` with no variable name)
- It organizes code into blocks, functions, statements, and expressions

---

### Step 3: SEMANTIC ANALYZER (src/semantic/SemanticAnalyzer.h)
**What it does:** Checks if the code makes sense

**Examples of what it checks:**
```
❌ int x; cout << x;        // ERROR: x not initialized
✅ int x = 5; cout << x;    // OK: x was initialized

❌ cout << y;               // ERROR: y never declared
✅ int y; cout << y;        // OK: y was declared

❌ int z;                    // WARNING: z declared but never used
✅ int z = 10; cout << z;   // OK: z is used
```

**Key concept:** Maintains a Symbol Table
- Tracks which variables are declared
- Tracks which variables are initialized
- Checks if variables are used correctly

---

### Step 4: CFG BUILDER (src/cfg/CFGBuilder.h)
**What it does:** Shows how the program flows from one block to another

**Example:**
```c
if (x > 5) {
    y = 10;
} else {
    y = 20;
}
z = y + 1;
```

**Becomes CFG blocks:**
```
Block 1: Check if x > 5
  ├─→ Block 2 (true): y = 10
  │      └─→ Block 4
  └─→ Block 3 (false): y = 20
         └─→ Block 4
         
Block 4: z = y + 1
```

**Key concept:** Each block is a sequence of statements with no branching
- Used to trace possible execution paths
- Helps detect unreachable code

---

### Step 5: DATA FLOW ANALYSIS (src/dataflow/)
**What it does:** Traces data through the program to find bugs

**Example Analysis: Uninitialized Variables**
```c
int x;
if (condition) {
    x = 5;
}
cout << x;  // ⚠️ WARNING: x might not be initialized!
```

**The analyzer creates sets:**
- `DEFINED`: Variables that definitely have values
- `USED`: Variables that are read
- Checks if USED variables are in DEFINED

---

## Key Data Structures

### Token (src/parser/Token.h)
```cpp
struct Token {
    TokenType type;   // What is this? (keyword, operator, etc.)
    string value;     // The actual text ("int", "5", "x")
};
```

### AST Nodes (src/ast/)
```cpp
// Expression examples
BinaryExpr   // a + b, x > 5, flag && test
UnaryExpr    // -x, !flag
PrimaryExpr  // 42, "hello", x

// Statement examples
DeclarationStmt   // int x;
AssignmentStmt    // x = 5;
IfStmt            // if (...) {...} else {...}
WhileStmt         // while (...) {...}
ForStmt           // for (...;...;...) {...}
BlockStmt         // { ... }
```

### Symbol (src/semantic/SymbolTable.h)
```cpp
struct Symbol {
    string name;           // Variable name
    bool initialized;      // Has it been assigned a value?
};
```

### BasicBlock (src/cfg/BasicBlock.h)
```cpp
class BasicBlock {
    int id;                  // Unique ID
    vector<string> statements;  // Statements in this block
    vector<int> next;        // IDs of blocks that can follow
};
```

---

## Complete Example Walkthrough

### Input Code (test.c)
```c
int main() {
    int x;
    x = 10;
    if (x > 5) {
        int y = x + 1;
        int z;
    }
    return x;
}
```

### Step 1: Lexer Output
```
KW_INT, ID(main), LPAREN, RPAREN, LBRACE,
  KW_INT, ID(x), SEMI,
  ID(x), ASSIGN, INT_LITERAL(10), SEMI,
  KW_IF, LPAREN, ID(x), GT, INT_LITERAL(5), RPAREN, LBRACE,
    KW_INT, ID(y), ASSIGN, ID(x), PLUS, INT_LITERAL(1), SEMI,
    KW_INT, ID(z), SEMI,
  RBRACE,
  KW_RETURN, ID(x), SEMI,
RBRACE, END_OF_FILE
```

### Step 2: Parser Output (AST)
```
ProgramNode
└── FunctionNode(type: int, name: main)
    └── BlockStmt
        ├── DeclarationStmt(type: int, name: x)
        ├── AssignmentStmt(name: x, value: 10)
        ├── IfStmt
        │   ├── condition: BinaryExpr(left: x, op: >, right: 5)
        │   └── thenBranch: BlockStmt
        │       ├── DeclarationStmt(type: int, name: y, value: x + 1)
        │       └── DeclarationStmt(type: int, name: z)
        └── ReturnStmt(value: x)
```

### Step 3: Semantic Analysis
```
✅ x declared at global scope
✅ x initialized before use (x = 10)
✅ x used in condition (x > 5)
⚠️  z declared but never used
✅ x returned from main
```

### Step 4: CFG Output
```
Block 1:
  x = 10
  → Check if x > 5
  
Block 2 (if true):
  y = x + 1
  z = ?
  → Block 3
  
Block 3:
  return x
```

### Step 5: Data Flow Output
```
At Block 1: x is DEFINED
At Block 2: x is DEFINED, y is DEFINED, z is NOT DEFINED
At Block 3: x is DEFINED
```

---

## File Organization

```
src/
├── parser/        ← Tokenizing and parsing
│   ├── Lexer.h        (converts text to tokens)
│   ├── ParserDriver.h (converts tokens to AST)
│   └── Token.h        (defines token types)
│
├── ast/           ← Abstract Syntax Tree
│   ├── Stmt.h         (statements: if, while, for, etc.)
│   ├── Expr.h         (expressions: a+b, x>5, etc.)
│   └── ASTNode.h      (base class)
│
├── semantic/      ← Semantic analysis
│   ├── SemanticAnalyzer.h  (checks code validity)
│   ├── Scope.h             (manages variable scopes)
│   └── SymbolTable.h       (stores variable information)
│
├── cfg/           ← Control Flow Graph
│   ├── CFGBuilder.h   (creates CFG)
│   ├── CFG.h          (CFG data structure)
│   └── BasicBlock.h   (nodes in CFG)
│
├── dataflow/      ← Data Flow Analysis
│   ├── DataFlowAnalysis.h  (base analysis class)
│   ├── WorklistSolver.h    (solver algorithm)
│   └── analyses/
│       ├── UnusedVars.cpp
│       ├── UninitializedVars.cpp
│       └── UnreachableCode.cpp
│
└── utils/         ← Utilities
    ├── Logger.h       (print messages)
    └── Helpers.h      (helper functions)
```

---

## How to Extend

### Add a New Warning Type

1. **Add analysis in `src/dataflow/analyses/`**
   ```cpp
   // NewAnalysis.cpp
   class NewAnalysis : public DataFlowAnalysis {
       void analyze() override {
           // Your logic here
       }
   };
   ```

2. **Call it from main.cpp**
   ```cpp
   NewAnalysis analysis;
   analyzer.run(&analysis);
   ```

### Add a New Language Feature

1. **Add token type in `src/parser/Token.h`**
   ```cpp
   enum TokenType {
       // ... existing tokens ...
       KW_SWITCH,  // New keyword
   };
   ```

2. **Handle in `src/parser/Lexer.cpp`**
   ```cpp
   if (word == "switch") t.type = KW_SWITCH;
   ```

3. **Add parser rule in `src/parser/ParserDriver.cpp`**
   ```cpp
   unique_ptr<Stmt> ParserDriver::parseSwitchStatement() {
       // Your parsing logic
   }
   ```

4. **Add AST node in `src/ast/Stmt.h`**
   ```cpp
   class SwitchStmt : public Stmt {
       // ...
   };
   ```

---

## Common Questions

**Q: Why use an AST instead of directly analyzing tokens?**
A: An AST organizes code into a tree structure that's easier to navigate and analyze. It respects grammar rules.

**Q: What's the difference between semantic analysis and data flow analysis?**
A: Semantic analysis checks *syntax* rules. Data flow analysis checks *logic* bugs.

**Q: Why do we need a CFG?**
A: It shows all possible execution paths. This helps detect unreachable code and trace how values flow through branches.

**Q: How is this different from a compiler?**
A: A compiler translates code to machine instructions. This analyzer only checks for bugs without executing anything.

---

## Running the Analyzer

```bash
# Compile
mkdir build
cd build
cmake ..
make

# Run
./static_analyzer

# View CFG
dot -Tpng cfg.dot -o cfg.png
```

---

## Next Steps

1. Read the actual grammar in `grammar/MiniC.g4`
2. Look at `tests/samples/test.c` for example code
3. Trace through `main.cpp` to see how everything connects
4. Try adding your own warning detection!
