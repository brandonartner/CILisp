/*
* Name: Brandon Artner
* Lab: Lab 8 Task 10
* Date: 03/26/2015
*/

/*

s-expressions calculator

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

*/

%{
#include "CILisp.h"
%}

%union
{
   double dval;
   char *sval;
   struct ast_node *astNode;
};

%token <sval> FUNC SYMBOL
%token <dval> NUMBER INTEGER REAL
%token LPAREN RPAREN EOL QUIT LET COND

%type <astNode> s_expr let_list let_elem s_expr_list arg_list

%%

program:/* empty */ { 
                       printf("> ");
                    }
        | program s_expr EOL {
                              printf("yacc: program expr\n"); 
                              //translate($2);
                              printf(" = %f ",eval($2));
                              freeNode($2);
                              printf("\n> "); 
                           }
        ;

s_expr:
        NUMBER { 
                  printf("yacc: NUMBER %lf\n", $1); 
                  $$ = number($1); 
               }
        | SYMBOL {
                  printf("yacc: SYMBOL %s\n", $1);
                  $$ = symbol($1);
                 }
        | LPAREN FUNC RPAREN {
                                printf("yacc: LPAREN FUNC RPAREN\n"); 
                                $$ = function($2,0);
                                printf("%s",$2);
                            }
        | LPAREN FUNC s_expr_list RPAREN {
                                      printf("yacc: LPAREN FUNC expr_list RPAREN\n"); 
                                     $$ = function($2, $3);  
                                     printf("%s", $2);
                                    }
        | LPAREN SYMBOL s_expr_list RPAREN {
                                           printf("LPAREN FUNC expr expr RPAREN\n"); 
                                          // $$ = calc($2, $3, $4); 
                                          $$ = function($2, $3);                                      //UNFINISHED
                                       }
        | LPAREN LPAREN LET let_list RPAREN s_expr RPAREN{
                                                          printf("yacc: LET list/s-expr\n");
                                                          $$ = let($4, $6);
                                                         }
        | LPAREN COND s_expr s_expr s_expr RPAREN{
                                                  printf("yacc: COND s_expr s_expr s_expr\n");
                                                  $$ = cond($3,$4,$5);
                                                 } 
        | QUIT { 
                  //printf("QUIT\n"); 
                  exit(0);
               }
        
        | error { 
                        printf("error\n"); 
                        //printf("> ");
                    }

        ;

s_expr_list: /* empty */{
                          printf(">");
                        }
            | s_expr_list s_expr {
                                  $$ = s_expr_list($1,$2);                                                                //UNFINISHED
                                }
            ;

arg_list: 
          SYMBOL arg_list {
                          $$ = NULL;                                                                        //UNFINISHED
                          }
          | /*Empty*/ {
                          printf(">");
                        }
          ;
let_list:
          let_elem {
                    printf("yacc: let_elem\n");
                    $$ = $1;

                   }
        | let_list let_elem {
                              printf("yacc: let_list let_elem\n");
                              $$ = let_list($1,$2);

                            }
        ;

let_elem:
          LPAREN SYMBOL s_expr RPAREN {
                                      printf("yacc: type SYMBOL s_expr\n");
                                      $$ = let_elem(0,$2,$3);
                                      }
        | LPAREN INTEGER SYMBOL s_expr RPAREN {
                                      printf("yacc: type SYMBOL s_expr\n");
                                      $$ = let_elem($2,$3,$4);
                                      }
        | LPAREN REAL SYMBOL s_expr RPAREN {
                                      printf("yacc: type SYMBOL s_expr\n");
                                      $$ = let_elem($2,$3,$4);
                                      }
        | LPAREN SYMBOL LPAREN arg_list RPAREN s_expr RPAREN {
                                                              $$ = let_elem_func($2,$4,$6);                                    //UNFINISHED
                                                              }
        ;
%%

