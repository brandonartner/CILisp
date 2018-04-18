/*
* Name: Brandon Artner
* Lab: Lab 8 Task 8
* Date: 03/26/2015
*/

/*

s-expressions calculator

program ::= program s-expr

s-expr ::= number | symbol | ( func s-expr ) | ( func s-expr s-expr ) | ( ( let let_list ) s_expr ) | ( cond s_expr s_expr s_expr )

let_list ::= let_elem | let_list let_elem

let_elem ::= ( type symbol s_expr )

type ::= integer | real

symbol ::= letter+

letter ::= [a-zA-Z]

number ::= [ + | - ] digit+ [ . digit+ ] 

digit ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

*/

%{
#include "SimpleLisp.h"
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

%type <astNode> s_expr let_list let_elem 

%%

program:/* empty */ { 
                       printf("> ");
                    }
        | program s_expr EOL {
                              //printf("yacc: program expr\n"); 
                              //translate($2);
                              printf(" = %f ",eval($2));
                              freeNode($2);
                              printf("\n> "); 
                           }
        ;

s_expr:
        NUMBER { 
                  //printf("yacc: NUMBER %lf\n", $1); 
                  $$ = number($1); 
               }
        | SYMBOL {
                  //printf("yacc: SYMBOL %s\n", $1);
                  $$ = symbol($1);
                 }
        | LPAREN FUNC s_expr RPAREN {
                                     //printf("yacc: LPAREN FUNC expr RPAREN\n"); 
                                     $$ = function($2, $3, 0);  
                                     //printf("%s", $2);
                                    }
        | LPAREN FUNC s_expr s_expr RPAREN {
                                          //printf("LPAREN FUNC expr expr RPAREN\n"); 
                                          // $$ = calc($2, $3, $4); 
                                          $$ = function($2, $3, $4);
                                       }
        | LPAREN LPAREN LET let_list RPAREN s_expr RPAREN{
                                                          //printf("yacc: LET list/s-expr\n");
                                                          $$ = let($4, $6);
                                                         }
        | LPAREN COND s_expr s_expr s_expr RPAREN{
                                                  //printf("yacc: COND s_expr s_expr s_expr\n");
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

let_list:
          let_elem {
                    //printf("yacc: let_elem\n");
                    $$ = $1;

                   }
        | let_list let_elem {
                              //printf("yacc: let_list let_elem\n");
                              $$ = let_list($1,$2);

                            }
        ;

let_elem:
          LPAREN SYMBOL s_expr RPAREN {
                                      //printf("yacc: type SYMBOL s_expr\n");
                                      $$ = let_elem(0,$2,$3);
                                      }
        | LPAREN INTEGER SYMBOL s_expr RPAREN {
                                      //printf("yacc: type SYMBOL s_expr\n");
                                      $$ = let_elem($2,$3,$4);
                                      }
        | LPAREN REAL SYMBOL s_expr RPAREN {
                                      //printf("yacc: type SYMBOL s_expr\n");
                                      $$ = let_elem($2,$3,$4);
                                      }
        ;
%%

