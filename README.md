How to compile  
g++ main.cpp lexer/Lexer.cpp parser/Parser.cpp cfg/CFG.cpp -o compiler
How to run 
./compiler
Generate dot file
dot -Tpng cfg.dot -o cfg.png
