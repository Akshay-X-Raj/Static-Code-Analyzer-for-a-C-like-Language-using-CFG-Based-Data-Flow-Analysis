# Semantic Analysis (The Symbol Table)

While the Lexer breaks code into words and the Parser ensures those words form valid grammatical sentences, the **Semantic Analysis** phase checks if those sentences actually make logical sense. 

One of the most crucial tools for semantic analysis is the **Symbol Table**.

## What is a Symbol Table?
A Symbol Table is a dictionary (or map) used by a compiler/interpreter to keep track of every single variable, function, or class that a programmer creates in their code.

In your project, the symbol table specifically tracks:
1. The **names** of variables (e.g., `x`, `y`).
2. Whether the variable has been **initialized** (given a value yet).

## Understanding `SymbolTable.h` and `SymbolTable.cpp`

The code for the Symbol Table is intentionally very lightweight and globally accessible.

### 1. The `Symbol` Structure (`SymbolTable.h`)
```cpp
struct Symbol {
    string name;
    bool initialized = false;
};
```
This defines what we need to know about a "symbol" (variable). Whenever a new variable is declared in the code, we create one of these `Symbol` structures to represent it. By default, `initialized` is strictly set to `false`.

### 2. The Global Map (`SymbolTable.h` & `SymbolTable.cpp`)
```cpp
// In SymbolTable.h
extern map<string, Symbol> symbolTable;

// In SymbolTable.cpp
map<string, Symbol> symbolTable;
```
We use the C++ standard `map` to build the actual table. 
- The **Key** (`string`) is the literal name of the variable (like `"x"`).
- The **Value** (`Symbol`) is the structure holding the data about that variable.

Using `extern` in the header file allows the Parser (and `main.cpp`) to look at and modify the exact same `symbolTable` memory space without creating conflicting duplicate tables.

## How the Parser Uses the Symbol Table

The table is dynamically built and updated during the Syntax Analysis (Parsing) phase.

### Adding New Symbols (Declarations)
When the Parser reads a line like `int x;`, its `parseDeclaration()` function does the following:

```cpp
if (symbolTable.count(ident.value) == 0) {
    Symbol s;
    s.name = ident.value;    // e.g., "x"
    s.initialized = false;   // It has no value yet!
    symbolTable[ident.value] = s;
}
```
It checks if `x` already exists in the map (`count == 0`). If it doesn't, it creates a new `Symbol` and adds it to the dictionary. If a programmer tried to declare `int x;` twice, the compiler could check this table and throw a "Variable already declared!" error.

### Updating Symbols (Assignments)
When the Parser skips down and reads a line like `x = 5;`, its `parseAssignment()` function triggers:

```cpp
if (symbolTable.count(ident.value) > 0) {
    symbolTable[ident.value].initialized = true;
}
```
It looks up `x` in the dictionary. If it finds it, it updates the `initialized` flag to `true` because `x` now securely holds the number `5`!

## Viewing the Output (`main.cpp`)
After the entire parsing process finishes, `main.cpp` loops over the entire dictionary to print the final semantic state of all variables found in the program:

```cpp
cout << "\nSymbol Table:\n";
map<string, Symbol>::iterator it;
for (it = symbolTable.begin(); it != symbolTable.end(); it++) {
    string status;
    if (it->second.initialized == true) {
        status = "Yes";
    } else {
        status = "No";
    }
    cout << it->first << " | Initialized: " << status << "\n";
}
```

If you look at your terminal output after running `la.exe` on `test.c`, you will see something like:
```text
Symbol Table:
x | Initialized: No
y | Initialized: Yes
```
This output proves the Semantic phase works! The compiler perfectly tracked that `y` was assigned a value inside the `if-else` block, while `x` was merely declared but never populated!
