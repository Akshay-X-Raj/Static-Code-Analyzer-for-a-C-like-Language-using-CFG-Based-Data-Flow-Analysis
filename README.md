How to compile 

g++ main.cpp lexer/Lexer.cpp parser/Parser.cpp cfg/CFG.cpp -o compiler
_______________________________________________________________________
How to run:

./compiler
_______________________________________________________________________

How to generate dot file

dot -Tpng cfg.dot -o cfg.png

_______________________________________________________________________

EXAMPLE 1:

int x;
float y;

x = 5;
y = x + 2.5;

if (y > 5.0) {
    printf("%f", y);
} else {
    printf("%d", x);
}

_______________________________________________________________________

Example 2:

    int x;
    int y;
    int z;

    x = 0;
    y = 0;

    while (x < 5) {
        y = y + x;
        x = x + 1;
    }

    printf("%d", y);

_______________________________________________________________________
    
for semantic analysis
int a;
a=5;
b=a+5;

_______________________________________________________________________

for syntax analysis
int a;
int b;
a=5;
b=a+5
_______________________________________________________________________
