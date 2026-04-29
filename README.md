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
int y;
int z;
int p;

x = 0;
y = 10;

while (x < 5) {

    if (x < 3) {
        y = y + x;
    } else {
        y = y - x;
    }

    if (y > 5) {
        z = y + x;
    } else {
        z = y - x;
    }

    x = x + 1;
}

printf("%d", y);

_______________________________________________________________________
Example 2: 

int x;
int y;

x = 0;
y = 0;

while (x < 5) {

    if (x < 3) {
        y = y + x;
    } else {
        y = y - x;
    }

    x = x + 1;
}
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
b=a+10;
printf("%d", b)
________________________________________________________________
