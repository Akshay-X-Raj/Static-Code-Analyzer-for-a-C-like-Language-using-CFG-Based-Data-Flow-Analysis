# Software Requirements Specification (SRS)
## Static Code Analyzer using CFG-Based Data-Flow Analysis

## 1. Introduction
### 1.1 Purpose
The purpose of this document is to define the software requirements for the Static Code Analyzer. This system is a C++ command-line tool designed to parse a simplified C-like language, generate its Control Flow Graph (CFG), and apply Data-Flow Analysis to detect common logical and semantic errors natively without executing the code.

### 1.2 Scope
The system will feature a complete compiler-like pipelined architecture:
- A **Compiler Frontend** module responsible for Lexical Analysis, Parsing (generating an AST), and syntax verification.
- A **Compiler Backend** module responsible for maintaining the Symbol Table, constructing the Control Flow Graph (CFG), and running Data Flow passes to yield terminal warnings.

---

## 2. Overall Description
### 2.1 Product Perspective
The tool acts as a standalone static analysis engine executed via the terminal, emulating modern professional static analyzers like SonarQube or Clang. It processes `.c` input files and outputs diagnostic messages directly into the console.

### 2.2 User Classes and Characteristics
- **Developers / Students**: Users trying to verify the structure and logical safety of their C-like code paths.

### 2.3 Operating Environment
- **Platform**: Platform-independent (Windows/Linux/macOS) execution via standard terminal.
- **Requirements**: Built with any standard modern C++ compiler (`g++`, `clang`).
- **Dependencies**: Graphviz (`dot`) for visualizing the generated `.dot` files into graphical `.png` graphs.

---

## 3. System Features

### 3.1 Lexical & Syntactic Analysis (Compiler Frontend)
**Description**: The application reads raw text, tokenizes it, and builds a hierarchical AST validation check.
**Functional Requirements**:
- REQ-1.1: The Lexer shall strip all whitespaces and categorize raw strings into predefined tokens (Keywords, Operators, Identifiers).
- REQ-1.2: The Parser shall construct a verified Abstract Syntax Tree based on the accepted CFG grammar rules.

### 3.2 Symbol Table Maintenance
**Description**: Managing variable lifecycles.
**Functional Requirements**:
- REQ-2.1: The analyzer must identify all valid variable declarations and enforce scope-level checks.
- REQ-2.2: It should easily block usage operations on explicitly undeclared variables.

### 3.3 CFG Generation (Compiler Backend)
**Description**: Constructing and visualizing basic branching blocks to trace flow.
**Functional Requirements**:
- REQ-3.1: The engine shall trace AST nodes and convert them to sequential and branching block nodes.
- REQ-3.2: It must export this memory structure as a standard `cfg.dot` mapping.

### 3.4 Data-Flow Analysis & Console Reporting
**Description**: Applying mathematical data-flow sets (Live variables, Reaching definitions) to detect issues.
**Functional Requirements**:
- REQ-4.1: The system shall parse paths to detect **uninitialized variables**.
- REQ-4.2: The system shall detect **unused variables**.
- REQ-4.3: The system shall warn about **unreachable code** blocks.
- REQ-4.4: All diagnostic warnings MUST be streamed clearly as text-based reports directly to the active system console terminal.

---

## 4. Non-Functional Requirements
### 4.1 Performance Requirements
- Text parsing and CFG generation must perform iteratively in milliseconds for scripts less than 1,000 lines.

### 4.2 Usability
- No graphical interface installations are required (beyond the optional Graphviz binary). Operation focuses purely on seamless CLI efficiency.
