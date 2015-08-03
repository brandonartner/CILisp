/*
* Name: Brandon Artner
* Lab: Lab 8 Task 10
* Date: 03/26/2015
*/

#include "l8t10.h"

int main(void)
{
    yyparse();
    return 0;
}

void yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}

// find out which function it is
int resolveFunc(char *func)
{
   char *funcs[] = { "neg", "abs", "exp", "sqrt", "add", "sub", "mult", "div", "remainder", "log", "pow", "max", "min", "exp2", "cbrt", "hypot", "rand", "read", "print", "equal", "smaller", "larger",""};
   
   int i = 0;
   while (funcs[i][0] !='\0')
   {
      if (!strcmp(funcs[i], func))
         return i;
         
      i++;
   }
   yyerror("invalid function"); // paranoic -- should never happen
   return -1;
}

// create a node for a number
AST_NODE *number(double value)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE) + sizeof(NUMBER_AST_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = NUM_TYPE;    
    //printf("the type is: %d\n", p->type);

    p->data.number.value = value;

    return p;
}

AST_NODE *symbol(char *name)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed size and the variable part (union)
    nodeSize = sizeof(AST_NODE) + sizeof(SYMBOL_AST_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = SYMB_TYPE;
       // printf("the type is: %d\n", p->type);

    p->data.symbol.name = name;
    printf("mainc: symbol-%s\n", p->data.symbol.name);

    return p;
}


AST_NODE *s_expr_list(struct ast_node *s_exprList, struct ast_node *newS_expr)
{
    newS_expr->exprTable = s_exprList;

    return newS_expr;
}

AST_NODE *args_list(struct ast_node *argList, struct ast_node *newArg)
{
  newArg->data.symbol.next = argList;

  return newArg;
}

AST_NODE *let_elem_func(char *name, struct ast_node *args, struct ast_node *value)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE) + sizeof(USER_FUNC_AST_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    // Sets up the function 
    p->data.userFunc.name = name;
    p->data.userFunc.args = args;
    p->data.userFunc.value = value;

    return p;

}

AST_NODE *let_elem(int varTypeNum, char *newName, struct ast_node *newValue)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE) + sizeof(LET_ELEM_AST_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = LET_ELEM_TYPE;
    //printf("the type is: %d\n", p->type);
    switch(varTypeNum){
      case 0:
            p->varType = UNDEC_VAR;
            break;
      case 1:
            p->varType = INT_VAR;
            break;
      case 2:
            p->varType = REAL_VAR;
            break;
      default:
            break;
    }
    p->data.letElem.name = newName;
    p->data.letElem.value = newValue;
    p->data.letElem.next = NULL;

    return p;
}

AST_NODE *let(AST_NODE *symbList, AST_NODE *s_expr)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p = s_expr;
    p->varTable = combineTables(s_expr->varTable,symbList);

    return p;
}

AST_NODE *let_list(AST_NODE *listStart, AST_NODE *newNode)
{
    newNode->data.letElem.next = listStart;

    if(resolveSymb(1,newNode->data.letElem.name,listStart) != NULL)
      printf("\nWARNING: Symbol %s has multiple definitions.\n", newNode->data.letElem.name);
    

    return newNode;
}

// Determines the current value of a symbol
struct ast_node* resolveSymb(int checkFlag, char *symbName, struct ast_node *symbList)
{
    struct ast_node *retVal = NULL;
    int isFound = 0;

    struct ast_node *currNode = symbList;

    while(currNode != NULL && !isFound)
    {
        if (strcmp(symbName,currNode->data.letElem.name) == 0)
        {
            retVal = currNode->data.letElem.value;
            //printf("The value of %s = %f",symbName,eval(retVal));
            isFound++;
        }
        else
          currNode = currNode->data.letElem.next;
    }
    if(retVal != NULL)
      retVal->varType = currNode->varType;

    if (!isFound)
        printf("\nSymbol: %s not defined\n", symbName);

    return retVal;
}

struct ast_node* combineTables(struct ast_node* childTable, struct ast_node* parentTable)
{
    if(parentTable == NULL){
      printf("\nparent tab is null\n");
      return childTable;
    }
    if(childTable == NULL){
      printf("\nchild tab is null\n");
      return parentTable;
    }

    struct ast_node *currNode = childTable;

    while(currNode->data.letElem.next != NULL){
      printf("\ntraversing list%s\n", currNode->data.letElem.name);
      currNode = currNode->data.letElem.next;
    }

    currNode->data.letElem.next = parentTable;

    return childTable;
}

// create a node for a function
AST_NODE *function(char *funcName, AST_NODE *ops)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE) + sizeof(FUNCTION_AST_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = FUNC_TYPE;
    p->data.function.name = funcName;
    p->data.function.op1 = ops;
    p->data.function.op2 = ops->exprTable;

    return p;
}

AST_NODE *cond(struct ast_node *newCondition, struct ast_node *newThen, struct ast_node *newElse)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE) + sizeof(COND_AST_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = COND_TYPE;
    p->data.cond.condition = newCondition;
    p->data.cond.ifTrue = newThen;
    p->data.cond.ifFalse = newElse;
}

// free a node
void freeNode(AST_NODE *p)
{
    if (!p)
       return;
       
    if (p->type == FUNC_TYPE)
    {
        free(p->data.function.name);
        //if(p->varTable != NULL)
        //freeNode(p->varTable);
        freeNode(p->data.function.op1);
        freeNode(p->data.function.op2);
    }
    else if (p->type == SYMB_TYPE)
    {
        free(p->data.symbol.name);
        //if(p->varTable != NULL)
        //freeNode(p->varTable);
    }
    else if (p->type == LET_ELEM_TYPE)
    {
        free(p->data.letElem.name);
        //if(p->varTable != NULL)
        freeNode(p->varTable);
        freeNode(p->data.letElem.value);
        freeNode(p->data.letElem.next);
    }

    if(!p && p->varTable){
      freeNode(p->varTable);
      free (p);
    }
}
/*
void freeVarTable(AST_NODE *p)
{
    if(!p)
      return;

    if (p->data.letElem.next != NULL)
    {
      freeVarTable(p->data.letElem.next);
    }

    free(p);
}*/

double eval(AST_NODE *p)
{
  if (!p)
    return 0;
  
  // Check what type of node p is
  if(p->type == FUNC_TYPE)
  {

    if(p->data.function.op1 != NULL)
      p->data.function.op1->varTable = combineTables(p->data.function.op1->varTable, p->varTable);
    if(p->data.function.op2 != NULL)
      p->data.function.op2->varTable = combineTables(p->data.function.op2->varTable, p->varTable);
    return calc(p->data.function.name, eval(p->data.function.op1), eval(p->data.function.op2));

  }
  else if(p->type == NUM_TYPE)
  {
    return p->data.number.value;
  }
  else if(p->type == SYMB_TYPE)
  {
    struct ast_node *temp = resolveSymb(0,p->data.symbol.name,p->varTable);
    if(!p)
      return 0;
    temp->varTable = combineTables(temp->varTable,p->varTable);

    // Type Checking
    // If the variable is of type int round it, if undeclared return an error, and if real do nothing 
    if(temp->varType == INT_VAR)
      return round(eval(temp));
    else if(temp->varType == UNDEC_VAR){
      printf("\nERROR: undeclared variable %s used\n",p->data.letElem.name);
      return eval(temp);
    }
    else
      return eval(temp);
  }
  else if(p->type == COND_TYPE)
  {
    p->data.cond.condition->varTable = combineTables(p->data.cond.condition->varTable,p->varTable); 
    if(eval(p->data.cond.condition)){
      p->data.cond.ifTrue->varTable = combineTables(p->data.cond.ifTrue->varTable,p->varTable);
      return eval(p->data.cond.ifTrue);
    }
    else{
      p->data.cond.ifFalse->varTable = combineTables(p->data.cond.ifFalse->varTable,p->varTable);
      return eval(p->data.cond.ifFalse);
    }
  }
  else
  {
   // printf("The final type is: %d\n", p->type);
    exit(0);
  }
}

double calc(char *func, double op1, double op2)
{

    double retVal;

    switch(resolveFunc(func)){
        case 0:retVal = -op1; break;
        case 1:retVal = fabs(op1); break;
        case 2:retVal = exp(op1); break;
        case 3:retVal = sqrt(op1); break;
        case 4:retVal = op1 + op2; break;
        case 5:retVal = op1 - op2; break;
        case 6:retVal = op1 * op2; break;
        case 7:retVal = op1 / op2; break;
        case 8:retVal = fmod(op1,op2); break;
        case 9:
          if(op1 == 2)  
            retVal = log2(op2);
          else if(op1 == 10)
            retVal = log10(op2);
          else
            printf("Log only supports base 2 and 10.\n");
        break;
        case 10:retVal = pow(op1,op2); break;
        case 11:retVal =  fmax(op1,op2);break;
        case 12:retVal =  fmin(op1,op2);break;
        case 13:retVal = exp2(op1); break;
        case 14:retVal = cbrt(op1); break;
        case 15:retVal = hypot(op1,op2); break;
        case 16:retVal = rand();break;
        case 17:scanf("%lf",&retVal);break;
        case 18:printf("%f\n", op1);
        case 22:retVal = op1; break;
        case 19:retVal = (op1 == op2);break;
        case 20:retVal = (op1 < op2);break;
        case 21:retVal = (op1 > op2);break;
        default:
          break;
    }

    return retVal;

}  


void translate(AST_NODE *p)
{
  //printf("and its translateing\n");
  if (!p)
    return;

  char *symbols[4] = {"+","-","*","/"};
  //printf("1\n");
  // Check what type of node p is
  if(p->type == FUNC_TYPE){
    //printf("2\n");
    int funcNum = resolveFunc(p->data.function.name);
    switch(funcNum){
      case 0:case 1:case 2:case 3:case 12:case 13:case 14: // {func}(op)
        printf("%s( ", p->data.function.name); translate(p->data.function.op1); printf(" )");
        break;
      // Arithmetic functions 
      case 4:case 5:case 6:case 7:
        printf("( "); translate(p->data.function.op1);
        printf(" %s ",symbols[funcNum-ARITHMETIC_SYMB_IND]);
        translate(p->data.function.op2); printf(" )");
        break;
      case 8:case 9:case 10:case 11:case 15: // {func}(op1,op2)
        printf("%s( ", p->data.function.name); translate(p->data.function.op1); printf(", "); translate(p->data.function.op2); printf(" )");
        break;
      default:
        break;

    }
  }
  else if(p->type == SYMB_TYPE){
    printf(" %s ", p->data.symbol.name);
  }
  else if(p->type == NUM_TYPE){ // Default/terminal case 
    printf(" %f ",p->data.number.value);
  }
}
