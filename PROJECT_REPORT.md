# Project Report: Static Code Analyzer using CFG-Based Data-Flow Analysis

## 1. Proposal Description

### Motivation
Our project focuses on the design and implementation of a static code analyzer for a simplified C-like programming language using Control Flow Graph (CFG)-based data-flow analysis. Our system does this by analysing the source code and finding out all the possible types of error during the compile time of the program without executing it. This static analysis process follows a structured process that includes lexical analysis, parsing, abstract syntax tree generation, symbol table construction, control flow graph generation, and finally data flow analysis.

At first, the source code is analysed and then it is broken down into tokens by the lexical analyser. Then the syntax is verified and an Abstract Syntax Tree is generated to describe the program structure in a simplified tree form. During all these processes, a symbol table is maintained to store information regarding the source and type of variables. A Control Flow Graph is then constructed to describe all the various possible execution paths that can be taken by the source code. The data-flow analysis on this allows the source code analyzer to determine how the variables have been used along different paths.

As a result, our project looks at the code and how it is being executed at different paths and then keeps all the records of how the variables are being used along with it. Due to this, the ability to find errors such as uninitialized variables, unreachable code, and unused variables becomes possible. This project is initialised because we want to know how modern static analysis tools like Clang Static Analyzer or SonarQube analyze code without executing it.

---

## 2. System Architecture & Modules

The project is divided into eight different modules which work dependently step-wise, thus analyzing the code. These naturally form the **frontend** and **backend** of our compiler-like structure.

### Compiler Frontend Setup
1. **Source Code**: This is the starting point of the system, where a user inputs source code in a C-like language. This code is not executed; it is analyzed structurally to understand its behaviour.
2. **Lexer**: The lexer reads the source code character by character and converts it into tokens. This removes whitespaces and newlines and categorizes each meaningful unit into keywords, identifiers, operators, symbols, etc. *(Eg: `int x=5;` is broken into `int`, `x`, `=`, `5`, and `;`)*.
3. **Parser**: It takes the stream of tokens generated during the lexer phase and checks if the grammar follows the programming rules of the C-like language. It checks if the program is written in a syntactically correct form.
4. **AST**: The Abstract Syntax Tree represents the hierarchical structure of the code.

### Compiler Backend Analysis
5. **Symbol Table**: Stores information about Identifiers (variables), like names, datatypes, scope, etc. Helps detect semantic errors like undeclared variables.
6. **CFG**: The Control Flow Graph shows all possible execution paths of the program. It models execution dividing into multiple blocks with single entry and exit points. It is visualized using GRAPHVIZ in a `.dot` file.
7. **Data Flow Analysis**: It uses the CFG to study how data moves through the program. The system can detect where variables are declared, updated, and used, which helps in the removal of logical errors.
8. **Warnings/Report**: This is the final output phase. The project sends a **warning message on the console**, helping developers identify issues early.

---

## 3. Project Goals and Milestones

The main objective of this project is to design and implement a static code analyzer for a C-like language using Control Flow Graph (CFG)-based data-flow analysis techniques. Our project should be able to analyze the source code without executing it and identify all the possible types of errors such as uninitialized variables, unreachable codes, and unused variables.

Other than this, the project detects common programming problems such as:
1. Use of uninitialized variables
2. Unreachable code
3. Unused variables

### Milestones
- **1: Planning and Language Design**: Analyse features of the C-like language and design the system architecture.
- **2: Lexer Development**: Implement the lexical analyzer which converts source code into tokens.
- **3: Parser and AST Construction**: Implement the parser and generate the AST to represent the source code.
- **4: Symbol Table and CFG Creation**: Implement the symbol table and generate the CFG.
- **5: Data-Flow Analysis Implementation**: Implement the data-flow analysis on the CFG to detect errors.
- **6: Testing and Final Report**: Test the system with sample programs and generate analysis reports to the terminal.

---

## 4. Project Outcome / Deliverables

### Primary Deliverables:

- **Functional Static Code Analyzer**: The primary outcome is a working Static Code Analyzer written in C++ which reads source code without executing it.
- **Complete Compiler-like pipelined structure**: The structure includes: `Source Code → Lexer → Parser → Abstract syntax tree → Symbol table → Control flow Graph → Data flow analysis → Warnings/Reports`.
- **Complete construction and visualization of CFG**: This project successfully constructs a Control Flow Graph. The CFG is exported in `.dot` format using **GRAPHVIZ**, enabling graphical visualization.
- **Error detection using Data-Flow analysis**: Implements data-flow analysis over the built CFG. Detects uninitialized variables, redundant assignments, unused variables, and unreachable code segments.

---

## 5. Assumptions

1. **Input Language Scope**: It is built exclusively for a C-like language with basic control flow or functions. It will not handle functions including pointers, structs/classes, or the full C standard library.
2. **External Dependency (Graphviz)**: It is assumed that a **Graphviz** installation will be available to process the generated DOT files.
3. **Development Environment**: The core system is assumed to be compiled and run in a standard environment with a modern **C++ compiler** (e.g., GCC/G++ or Clang).
4. **Error and Warning Reporting**: Diagnostics are natively limited to **text-based reports output to the console**.
