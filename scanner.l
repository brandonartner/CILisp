/*
* Name: Brandon Artner
* Lab: Lab 8 Task 10
* Date: 03/26/2015
*/

%option noyywrap

%{
    #include "SimpleLisp.h"
%}

digit [0-9]
number (-)?{digit}+(\.{digit}+)?
letter [a-zA-Z]
symbol {letter}+
func "neg"|"abs"|"exp"|"sqrt"|"add"|"sub"|"mult"|"div"|"remainder"|"log"|"pow"|"max"|"min"|"exp2"|"cbrt"|"hypot"|"rand"|"read"|"print"|"equal"|"smaller"|"larger"

%%

{number} {
                yylval.dval = strtod(yytext, NULL);
                printf("lex: NUMBER dval = %lf\n", yylval.dval);
                return NUMBER;
            }


"let"       { 
                printf("lex: LET\n");
                return LET; 
            }
"cond"      {
                printf("lex: cond");
                return COND;
            }

"int"       {
                printf("lex: type int\n");
                yylval.dval = 1;
                return INTEGER;
            }

"integer"   {
                printf("lex: type int\n");
                yylval.dval = 1;
                return INTEGER;
            }

"real"      {
                printf("lex: type real\n");
                yylval.dval = 2;
                return REAL;
            }

"q"|"quit"      { return QUIT; }

{func}      {
                yylval.sval = (char *) malloc(strlen(yytext)*sizeof(char));
                strcpy(yylval.sval, yytext);
                printf("lex: FUNC sval = %s\n", yylval.sval);
                return FUNC;
            }

{symbol} {
                yylval.sval = (char *) malloc(strlen(yytext)*sizeof(char));
                strcpy(yylval.sval, yytext);
                printf("lex: SYMBOL sval = %s\n", yylval.sval);
                return SYMBOL;

            }

"("         { 
                printf("lex: LPAREN\n"); 
                return LPAREN;
            }
")"         { 
                printf("lex: RPAREN\n"); 
                return RPAREN;
            }
"\n"        {
                printf("lex: RPAREN\n"); 
                return EOL;
            }

[ \t]       ; /* skip whitespace */

.           // yyerror("lex: invalid character");

%%
