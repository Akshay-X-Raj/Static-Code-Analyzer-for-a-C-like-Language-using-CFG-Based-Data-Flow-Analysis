# Static-Code-Analyzer-for-a-C-like-Language-using-CFG-Based-Data-Flow-Analysis

## Project Architecture & Pipeline

This project implements a complete compiler-like modular architecture:
- **`src/`**: Contains the core logic, divided mathematically into the Compiler Frontend and Backend:
  - **Compiler Frontend:**
    - `parser/`: Reads raw language strings and recursively parses generated `Token`s into an AST.
    - `ast/`: The Abstract Syntax Tree data structures mathematically mapping the file.
  - **Compiler Backend:**
    - `semantic/` (Symbol Table): Stores definitions and flags undeclared or unbound usage requests.
    - `cfg/`: Interprets the generated AST nodes and traces them into sequential and branching paths.
    - `dataflow/`: Iterates over the generated CFG to detect semantic issues (uninitialized values, dead code).
    - `viz/`: The visual export logic mapping memory into `cfg.dot` blocks.
- **`grammar/`**: Holds the formal syntax blueprint governing the language validation.
- **`tests/samples/test.c`**: Evaluated input code testing standard logic blocks.

## How to Compile & Run

### 1. Requirements
- You must have a modern C++ compiler (like **GCC / g++ / MinGW / Clang**) to compile the terminal executable.
- (Optional but Highly Recommended) Install **Graphviz** to convert textual `.dot` traces into flowchart `.png` images.
  - Windows: `winget install graphviz`

### 2. Quick Start

**Step 1: Compile the Analyzer**
Open your terminal in the root project folder:

*(For Linux / Mac)*:
```bash
mkdir -p build
g++ -I src/parser -I src/ast -I src/semantic -I src/cfg -I src/dataflow -I src/viz -I src/utils $(find src -name "*.cpp") -o build/static_analyzer
```

*(For Windows PowerShell)*:
```powershell
New-Item -ItemType Directory -Force -Path "build"
$src = Get-ChildItem -Path "src" -Recurse -Filter "*.cpp" | ForEach-Object { "`"$($_.FullName)`"" }
$srcList = $src -join " "
Invoke-Expression "g++ -I src/parser -I src/ast -I src/semantic -I src/cfg -I src/dataflow -I src/viz -I src/utils $srcList -o build/static_analyzer.exe"
```

**Step 2: Run the Application**
```bash
# On Linux/Mac:
./build/static_analyzer

# On Windows:
.\build\static_analyzer.exe
```

*(You can also use the default included `la.exe` or `analyzer.exe` if previously generated).*

## 3. Reading the Output

Diagnostics are limited cleanly and natively into **text-based reports output to the console**.
When you run the engine, it performs the Lexing, Parsing, and Analysis steps natively without executing your code.

- **Warnings**: The terminal will output text warning about unreachable logic, unused variables, and undeclared states safely.
- **Visual CFG (.dot)**: It will generate a file named `cfg.dot`. If you have installed Graphviz and added it to your system PATH, you can run the following to generate a visual image!

```powershell
dot -Tpng cfg.dot -o cfg_visual.png
```
*(After generating this file, simply open `cfg_visual.png` to trace every possible execution path visually!)*
