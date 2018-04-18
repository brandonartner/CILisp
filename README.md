# Simple Lisp
Compiler for a simplified version of Lisp. Uses Bison for parsing and Flex for scanning. 

Instructions for Ubuntu:
1. Install bison and flex
```
sudo apt-get install flex bison
```
2. Run the build file
```
./build.sh
```
3. Excecute l8t10
```
./SimpleLisp
```

## Language Grammar:

program ::= program s-expr  
s-expr ::= number | symbol | ( func s-expr_list ) | ( symbol s-expr_list ) | ( ( let let_list ) s_expr ) | ( cond s_expr s_expr s_expr )  
s_expr_list ::= | s_expr_list s_expr  
let_list ::= let_elem | let_list let_elem  
let_elem ::= (symbol s_expr) ( type symbol s_expr ) | ( symbol ( arg_list ) s-expr )  
type ::= integer | real  
arg_list ::= symbol arg_list |  
symbol ::= letter+  
letter ::= [a-zA-Z]  
number ::= [ + | - ] digit+ [ . digit+ ]   
digit ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9  

## Functions:

"neg"|"abs"|"exp"|"sqrt"|"add"|"sub"|"mult"|"div"|"remainder"|"log"|"pow"|"max"|"min"|"exp2"|"cbrt"|"hypot"|"print"|"equal"|"smaller"|"larger"  

