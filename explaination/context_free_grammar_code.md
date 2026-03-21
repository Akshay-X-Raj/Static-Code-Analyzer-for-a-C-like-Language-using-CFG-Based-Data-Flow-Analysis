# Context-Free Grammar (CFG) Implementation

This document explains the explicit C++ code written to define our Context-Free Grammar. 

While the `Parser.cpp` handles the *action* of parsing the language based on the grammar, the exact rules of the grammar are formally stored and printed using the `ContextFreeGrammar` class. This is extremely helpful for humans to see exactly what syntax the language supports at a glance!

## The `ContextFreeGrammar` Class

The code is split into two files:
1. `header/ContextFreeGrammar.h` (The Declarations)
2. `ContextFreeGrammar.cpp` (The Implementations)

### How the Data Structure Works
Inside the class, we use a C++ standard `map` to link a grammar rule (left-hand side) to its possible variations (right-hand side).

```cpp
map<string, vector<string>> rules;
```
- `string`: This is the **Non-Terminal** rule name (e.g., `"<Statement>"` or `"<IfElse>"`).
- `vector<string>`: This is a list of all the exact matching **Production Rules** that the non-terminal can turn into.

### Initializing the Rules (`ContextFreeGrammar()`)

When the `ContextFreeGrammar` object is created in `main.cpp`, its constructor immediately populates the `rules` map with the grammar:

```cpp
ContextFreeGrammar::ContextFreeGrammar() {
    rules["<Program>"] = {"<Statement> <Program>", "EMPTY"};
    rules["<Statement>"] = {"<Declaration>", "<Assignment>", "<IfElse>"};
    rules["<Declaration>"] = {"int IDENTIFIER ;"};
    rules["<Assignment>"] = {"IDENTIFIER = <Value> ;"};
    rules["<IfElse>"] = {"if ( IDENTIFIER ) <Assignment> else <Assignment>"};
    rules["<Value>"] = {"NUMBER", "IDENTIFIER"};
}
```
This mimics exactly how a classic compiler textbook defines language rules! 
For instance, the map tells us that a `<Statement>` could be a `<Declaration>` **OR** an `<Assignment>` **OR** an `<IfElse>` block.

### Printing the Rules (`printGrammar()`)

To make it beginner-friendly and visually appealing, the `printGrammar()` function loops over the map:

```cpp
void ContextFreeGrammar::printGrammar() {
    cout << "\n--- CONTEXT-FREE GRAMMAR RULES ---\n";
    map<string, vector<string>>::iterator it;
    
    for (it = rules.begin(); it != rules.end(); it++) {
        // ... prints the left-hand key
        cout << it->first << " -> ";
        
        // ... loops over the vector of string expansions
        for (int i = 0; i < it->second.size(); i++) {
            cout << it->second[i];
            if (i < it->second.size() - 1) {
                cout << " | "; // Separates different choices with the OR operator
            }
        }
    }
}
```
This takes the map and prints it beautifully formatted into the terminal using standard index loops!

### Execution in `main.cpp`
When you execute the program, the very first thing it does is instantiate the grammar and print it:
```cpp
ContextFreeGrammar grammar;
grammar.printGrammar();
```
This guarantees that before parsing any tokens from `test.c`, the terminal explicitly tells you what valid grammar it is looking for.
