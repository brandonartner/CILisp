#!/bin/sh
bison -y -d parser.y
flex scanner.l
gcc -std=c99 -D_GNU_SOURCE=1 -D_POSIX_SOURCE=1 SimpleLisp.c lex.yy.c y.tab.c -o SimpleLisp -g -lm
