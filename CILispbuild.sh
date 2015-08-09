#!/bin/sh
bison -y -d CILParser.y
flex CILScanner.l
gcc -std=c99 -D_GNU_SOURCE=1 -D_POSIX_SOURCE=1 CILisp.c lex.yy.c y.tab.c -o CILisp -g -lm
