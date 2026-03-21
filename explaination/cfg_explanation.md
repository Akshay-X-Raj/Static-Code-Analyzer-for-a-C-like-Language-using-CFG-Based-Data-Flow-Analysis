# Control Flow Graph (CFG) Explanation

## What is a CFG?
A **Control Flow Graph (CFG)** is a representation of all paths that might be traversed through a program during its execution. It represents the code as a directed graph where:
- Each **Node** (or Basic Block) represents a sequence of statements with a single entry point and a single exit point (meaning no branches inside the block).
- Each **Edge** represents a possible jump or flow of control from one block to another (e.g., from an `if` condition jumping to either the `true` or `false` branch).

## Why is it Used?
CFGs are essential in compilers and code analysis tools for:
1. **Optimization**: Understanding how code flows helps compilers identify unused code (dead code elimination), predict branches, and optimize loops.
2. **Analysis**: Security and static analysis tools use CFGs to track variable states over time and catch uninitialized variables or infinite loops.

## How it works in this project

In your project, the CFG logic resides inside `header/CFG.h` and `CFG.cpp`, and the graph is populated by the `Parser` during syntax analysis. 

### Data Structures 
```cpp
class BasicBlock {
public:
    int id;                           // Unique identifier for the block
    vector<string> statements;        // Lines of code belonging to this block
    vector<int> next;                 // List of IDs for branching blocks (where to go next)
};

extern vector<BasicBlock> cfg;        // The actual graph storing all blocks
```
- The `BasicBlock` holds an identifier (`id`), the code that makes up the block (`statements`), and a list of target IDs (`next`) that point to subsequent paths the code can take.

### CFG Generation (`createBlock`)
In `CFG.cpp`, the `createBlock` function appends a new block to our global `cfg` graph list and assigns it an increasing ID.

### Building the Graph (Inside `Parser.cpp`)
The main work of constructing edges happens in the `parseIfElse()` function. When the parser sees an `if-else` statement:
1. It creates a **Condition Block** evaluating the `if (condition)`.
2. It creates a **True Block** holding the code if the condition corresponds to reality.
3. It creates a **False Block** (the `else` portion).
4. It creates an **End Block** where both branches eventually converge.
5. Finally, it links them using the `next` property of the `BasicBlock`:
   - Condition Block -> links to True Block and False Block.
   - True Block -> links to End Block.
   - False Block -> links to End Block.

### Outputs
- `printCFG()`: Loops through the graph and prints the blocks and their connections to the terminal.
- `exportDOT()`: Generates a `cfg.dot` file containing DOT syntax. This output can be opened by visualization software (like Graphviz) to draw a neat picture of the program's control flow.

---

### Understanding the Details of `CFG.cpp`

The `CFG.cpp` file contains the actual working logic for creating and showing the graph. Here is a breakdown of its purpose and the code inside it:

**1. Global Variables**
```cpp
vector<BasicBlock> cfg;
int blockCounter = 0;
```
- `cfg`: This is the master list that stores all the blocks in our program.
- `blockCounter`: A simple counter that ensures every new block gets a unique, incrementally higher ID (Block 1, Block 2, ...).

**2. The `createBlock` Function**
```cpp
int createBlock(string stmt) {
    BasicBlock b;
    b.id = ++blockCounter;
    b.statements.push_back(stmt);
    cfg.push_back(b);
    return b.id;
}
```
- **Purpose**: A centralized helper function to easily create a new block. 
- **How it works**: It takes the code statement as a string (`stmt`), assigns the block a fresh ID, pushes the statement onto the block's statement list, adds the new block to our global `cfg` graph list, and returns the ID so the `Parser` can keep track of it and link to it later.

**3. The `printCFG` Function**
- **Purpose**: Primarily useful for debugging in the terminal. 
- **How it works**: It loops using simple `for` loops through all the blocks inside the `cfg` list. For every block, it prints its ID, the statement it holds, and arrows `->` indicating the target IDs of any blocks it jumps to next (its branches).

**4. The `exportDOT` Function**
- **Purpose**: Creates a visual diagram representation of the graph. 
- **How it works**: It opens an `ofstream` to write into a new file named `cfg.dot`. It translates our basic block structures into a format called the "DOT Language". It creates nodes `B1 [label="..."]` and draws edges `B1 -> B2`. 

To convert this generated `cfg.dot` file into a visual image using Graphviz, you can run the following command in your terminal:
```bash
dot -Tpng cfg.dot -o cfg_visual.png
```
This will render a beautiful visual flowchart image of your program's execution!

---

### Visual Example of a CFG

If you have a statement like this in your code:
```cpp
if (x)
  y = 1;
else
  y = 0;
```

Here is a visual representation (Control Flow Graph) of what that looks like behind the scenes:

![CFG Example Diagram](cfg_example.png)

1. **Block 1** is the **Condition Block**. It checks the `if` statement.
2. If true, flow goes to **Block 2** (True Block).
3. If false, flow goes to **Block 3** (False/Else Block).
4. Both branches eventually flow into **Block 4** (End Block) to continue the rest of the program!
