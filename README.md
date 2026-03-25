# Static Code Analyzer - Simplified Consolidated Version

A lightweight C++ static code analyzer that detects bugs without executing code.

## Quick Start

### 1. Compile
```bash
cd src
g++ -std=c++17 main.cpp -o analyzer
```

**Or on Windows (PowerShell):**
```powershell
cd src
g++ -std=c++17 main.cpp -o analyzer.exe
```

### 2. Run
```bash
./analyzer
# or on Windows:
.\analyzer.exe
```

### 3. Visualize CFG (Optional)
```bash
dot -Tpng cfg.dot -o cfg_visual.png
```

## Architecture

**Simplified 4-file design:**
- **Parser.h** - Lexer, Parser, AST nodes
- **Semantic.h** - Semantic analyzer, scope, symbol table  
- **CFG.h** - Control flow graph builder
- **DataFlow.h** - Data flow analysis engine
- **main.cpp** - Complete pipeline

## Features

✅ Lexical analysis (tokenization)
✅ Parsing (AST building)
✅ Semantic analysis (variable validation)
✅ Control flow graph generation
✅ Data flow analysis (find bugs)

## Warnings Detected

- Uninitialized variables
- Unused variables  
- Unreachable code
- Undeclared variables

## Files Structure

```
src/
├── Parser.h       (500 lines) - Lexer + Parser + AST
├── Semantic.h     (200 lines) - Semantic analysis
├── CFG.h          (300 lines) - Control flow graph
├── DataFlow.h     (200 lines) - Data flow analysis
└── main.cpp       (150 lines) - Pipeline orchestration

tests/samples/
└── test.c         - Example code to analyze
```

## Learning Resources

- **BEGINNER_GUIDE.md** - 5-step pipeline explanation with examples
- **CONSOLIDATED_GUIDE.md** - Architecture and design decisions

## Requirements

- C++ 17 or later
- Standard library only (STL)
- Optional: Graphviz (for CFG visualization)
