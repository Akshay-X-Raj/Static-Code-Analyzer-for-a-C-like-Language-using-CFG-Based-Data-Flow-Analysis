# Static-Code-Analyzer-for-a-C-like-Language-using-CFG-Based-Data-Flow-Analysis

## Project Structure

- **`src/`**: Contains the compiler logic, divided into modular passes:
  - `parser/`: Generates semantic `Token`s and recursively parses them into an AST.
  - `ast/`: The Abstract Syntax Tree defining expressions and statements.
  - `semantic/`: Checks variable declarations, undefined uses, and scope bounds.
  - `cfg/`: Translates the AST flow into basic blocks representing runtime execution.
  - `dataflow/`: Skeleton handlers for Data Flow analyses.
  - `viz/`: Provides the `cfg.dot` export logic to visualize branches and loops.
  - `utils/`: Additional logging and boilerplate helpers.
- **`grammar/`**: Holds `MiniC.g4`, the formal syntax blueprint mapping the language implementation.
- **`tests/samples/test.c`**: The input dummy source code that the compiler parses.

## How to Compile & Run

### 1. Requirements
- You must have a C++ compiler installed (like **g++** / MinGW).
- (Optional) Download **Graphviz** to convert the outputted `cfg.dot` into a visual diagram.

### 2. Quick Start

**Step 1: Compile the project**
Copy and paste this command into your terminal:
```powershell
g++ -I src/parser -I src/ast -I src/semantic -I src/cfg -I src/dataflow -I src/viz -I src/utils src/main.cpp src/parser/Lexer.cpp src/parser/ParserDriver.cpp src/ast/ASTBuilder.cpp src/semantic/SymbolTable.cpp src/semantic/Scope.cpp src/semantic/SemanticAnalyzer.cpp src/cfg/CFGBuilder.cpp src/dataflow/WorklistSolver.cpp src/viz/CFGPrinter.cpp src/dataflow/analyses/DeadCode.cpp src/dataflow/analyses/UnusedVars.cpp src/dataflow/analyses/UnreachableCode.cpp src/dataflow/analyses/UninitializedVars.cpp -o analyzer.exe
```

**Step 2: Run the analyzer**
```powershell
.\analyzer.exe
```

That's it! The program will analyze the test code and show results.

---

### 3. Detailed Compilation

**Simple Command (Recommended):**
Open your terminal inside the project directory and run:

```bash
mkdir -p build
g++ -I src/parser -I src/ast -I src/semantic -I src/cfg -I src/dataflow -I src/viz -I src/utils $(find src -name "*.cpp") -o build/static_analyzer.exe
```

**For Windows PowerShell (Alternative):**
```powershell
New-Item -ItemType Directory -Force -Path "build"
$src = Get-ChildItem -Path "src" -Recurse -Filter "*.cpp" | ForEach-Object { "`"$($_.FullName)`"" }
$srcList = $src -join " "
Invoke-Expression "g++ -I src/parser -I src/ast -I src/semantic -I src/cfg -I src/dataflow -I src/viz -I src/utils $srcList -o build/static_analyzer.exe"
```

### 4. Running the Analyzer

Simply execute the compiled program:

```bash
# On Linux/Mac
./analyzer

# On Windows
analyzer.exe
```

Or from PowerShell:
```powershell
.\analyzer.exe
```

**Output Phases:**
1. Identifies strings and logical operators by breaking `test.c` into raw syntax tokens (Lexing).
2. Validates your loops, assignments, and expressions against the strict grammar precedence by generating an AST (Parsing).
3. Warns if you try to use undeclared variables (Semantic Analysis).
4. Translates the nested AST directly into a sequential basic block branch chart.
5. Automatically generates a `cfg.dot` file tracking the exact runtime flow!

### 5. Visualizing the CFG (Optional)
If you have Graphviz installed on your machine (`winget install graphviz`), you can convert the textual `cfg.dot` into a flowchart image:

```powershell
dot -Tpng cfg.dot -o cfg_visual.png
```

*(Note: If `dot` is not recognized after a fresh installation, you can directly run the absolute file location to bypass restarting the terminal, e.g.: `& "C:\Program Files\Graphviz\bin\dot.exe" -Tpng cfg.dot -o cfg_visual.png`)*
