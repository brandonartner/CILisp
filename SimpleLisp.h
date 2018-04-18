/*
* Name: Brandon Artner
* Lab: Lab 8 Task 10
* Date: 03/26/2015
*/

#ifndef __l8t4_h_
#define __l8t4_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "y.tab.h"

int yyparse(void);
int yylex(void);
void yyerror(char *);

typedef enum { NUM_TYPE, SYMB_TYPE, FUNC_TYPE, LET_ELEM_TYPE, COND_TYPE, USER_FUNC_TYPE } AST_NODE_TYPE;
typedef enum { UNDEC_VAR, INT_VAR, REAL_VAR } VAR_TYPE;

typedef struct
{
    double value;
} NUMBER_AST_NODE;

typedef struct
{
   char *name;
   struct ast_node *op1;
   struct ast_node *op2;
} FUNCTION_AST_NODE;

typedef struct 
{
   char *name;
   struct ast_node *value;
   struct ast_node *next;
} LET_ELEM_AST_NODE;

typedef struct 
{
   char *name;
   struct ast_node *next;
}SYMBOL_AST_NODE;

typedef struct 
{
   struct ast_node *condition;
   struct ast_node *ifTrue;
   struct ast_node *ifFalse;
}COND_AST_NODE;

typedef struct 
{
   char *name;
   struct ast_node *args;
   struct ast_node *value;
}USER_FUNC_AST_NODE;

typedef struct ast_node
{
   AST_NODE_TYPE type;
   VAR_TYPE varType;
   union
   {
      NUMBER_AST_NODE number;
      SYMBOL_AST_NODE symbol;
      FUNCTION_AST_NODE function;
      LET_ELEM_AST_NODE letElem;
      COND_AST_NODE cond;
      USER_FUNC_AST_NODE userFunc;
   } data;
   struct ast_node *varTable;
   struct ast_node *exprTable;
} AST_NODE;

AST_NODE *number(double value);
AST_NODE *symbol(char *name);
AST_NODE *let(AST_NODE *symbList, AST_NODE *s_expr);
AST_NODE *let_elem(int varTypeNum, char *name,struct ast_node *value);
AST_NODE *let_list(AST_NODE *first, AST_NODE *last);
AST_NODE *function(char *funcName, AST_NODE *ops);
AST_NODE *cond(struct ast_node *newCondition, struct ast_node *newThen, struct ast_node *newElse);
AST_NODE *s_expr_list(struct ast_node *s_exprList, struct ast_node *newS_expr);
AST_NODE *args_list(struct ast_node *argList, struct ast_node *newArg);
AST_NODE *let_elem_func(char *name, struct ast_node *args, struct ast_node *value);
void freeNode(AST_NODE *p);
//void freeVarTable(AST_NODE *p);
struct ast_node* combineTables(struct ast_node* childTable, struct ast_node* parentTable);

double eval(AST_NODE *ast);
double calc(char *func, double op1, double op2);
void translate(AST_NODE *p);
struct ast_node* resolveSymb(int checkFlag, char *symbName, struct ast_node *symbList);

#define ARITHMETIC_SYMB_IND 4

#endif
