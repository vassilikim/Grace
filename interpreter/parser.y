%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include "lexer.hpp"
#include "ast.hpp"
%}

%token T_and    "and"  
%token T_char   "char"
%token T_div    "div"
%token T_do     "do"
%token T_else   "else"
%token T_fun    "fun"
%token T_if     "if"
%token T_int    "int"
%token T_not     "not"
%token T_nothing "nothing"
%token T_or      "or"
%token T_ref     "ref"
%token T_return  "return"
%token T_then    "then"
%token T_var     "var"
%token T_while   "while"
%token T_mod     "mod"

%token<str> T_id 
%token<num> T_const
%token<var> T_constchar
%token<str> T_string

%token T_leq
%token T_meq 
%token T_assign

%left<condop> "or"
%left<condop> "and"
%precedence "not"
%nonassoc<condop> '=' '#' '<' '>' T_leq T_meq 
%left<binop> '+' '-'
%left<binop> '*' "div" "mod"
%precedence SIGN

%expect 1

%union {
  FuncDef *funcdef;
  LocalDefList *localdeflist;
  Header *header;
  FparDefList *fpardeflist;
  FparDef *fpardef;
  IdList *idlist;
  char *datatype;
  Type *type;
  ConstList *constlist;
  char *rettype;
  FparType *fpartype;
  Func *func;
  Stmt *stmt;
  Block *block;
  FuncCall *funccall;
  ExprList *exprlist;
  Expr *expr;
  char var;
  int num;
  char *str;
  char *binop;
  char *condop;
}

%type<funcdef> program func-def
%type<localdeflist> local-def_list
%type<header> header
%type<fpardeflist> fpar-def_list
%type<fpardef> fpar-def
%type<idlist> id_list
%type<datatype> data-type
%type<type> type
%type<constlist> int-const_list
%type<rettype> ret-type
%type<fpartype> fpar-type
%type<func> local-def func-decl var-def
%type<stmt> stmt
%type<block> stmt_list block
%type<funccall> func-call
%type<exprlist> expr_list
%type<expr> l-value expr cond

%%

program: 
func-def                          { std::cout << "AST: " << *$1 << std::endl; }
;

func-def: 
header local-def_list block       { $$ = new FuncDef($1, $2, $3); }
;

local-def_list: 
                                  { $$ = new LocalDefList(); }
| local-def_list local-def        { $1->append($2); $$ = $1; }
;

header: 
"fun" T_id '(' fpar-def fpar-def_list ')' ':' ret-type    { $5->append($4); $$ = new Header($2, $5, $8); }
;

fpar-def_list: 
                                    { $$ = new FparDefList(); }
| fpar-def_list ';' fpar-def        { $1->append($3); $$ = $1; }
;

fpar-def:
                                    { $$ = new FparDef(); }
| T_id id_list ':' fpar-type        { $2->append($1); $$ = new FparDef($2, $4); }
| "ref" T_id id_list ':' fpar-type  { $3->append($2); $$ = new FparDef($3, $5); }
;

id_list: 
                                    { $$ = new IdList(); }      
| id_list ',' T_id                  { $1->append($3); $$ = $1; }
;

data-type: 
"int"                               { char* c = new char[4]; strcpy(c, "int"); $$ = c; }                
| "char"                            { char* s = new char[5]; strcpy(s, "char"); $$ = s; }
;

type: 
data-type int-const_list             { $$ = new Type($1, $2);}
;

int-const_list: 
                                    { $$ = new ConstList(); }   
| int-const_list '[' T_const ']'    { $1->append($3); $$ = $1; }
;

ret-type: 
data-type                           { $$ = $1; }
| "nothing"                         { char* n= new char[8]; strcpy(n, "nothing"); $$ = n; }
;

fpar-type: 
data-type                                     { $$ = new FparType($1); }
| data-type '[' ']' int-const_list            { $4->append(0); $$ = new FparType($1, $4); }
| data-type '[' T_const ']' int-const_list    { $5->append($3); $$ = new FparType($1, $5); }
;

local-def: 
func-def                            { $$ = $1;}
| func-decl                         { $$ = $1;}
| var-def                           { $$ = $1;}
;

func-decl: 
header ';'                          { $$ = new FuncDecl($1); }
;

var-def: 
"var" T_id id_list ':' type ';'     { $3->append($2); $$ = new VarDef($3, $5); }
;

stmt: 
';'                                 { $$ = new BlankStmt(); }
| l-value T_assign expr ';'         { $$ = new Assignment($1, $3); }
| block                             { $$ = $1; }
| func-call ';'                     { $$ = $1; }
| "if" cond "then" stmt             { $$ = new If($2, $4); }
| "if" cond "then" stmt "else" stmt { $$ = new If($2, $4, $6); }
| "while" cond "do" stmt            { $$ = new While($2, $4); }
| "return" ';'                      { $$ = new Return(); }
| "return" expr ';'                 { $$ = new Return($2); }
;

block: 
'{' stmt_list '}'                   { $$ = $2; }
;

stmt_list: 
                                    { $$ = new Block(); }
| stmt_list stmt                    { $1->append($2); $$ = $1; }
;

func-call: 
T_id '(' ')'                        { $$ = new FuncCall($1); }
| T_id '(' expr expr_list ')'       { $4->append($3); $$ = new FuncCall($1, $4); }
;

expr_list: 
                                    { $$ = new ExprList(); }
| expr_list ',' expr                { $1->append($3); $$ = $1; }
;

l-value: 
T_id                                { $$ = new Id($1); }
| T_string                          { $$ = new String($1); }
| l-value '[' expr ']'              { $$ = new Array($1, $3); }
;

expr: 
T_const                             { $$ = new Const($1); }
| T_constchar                       { $$ = new Const($1); }
| l-value                           { $$ = $1; }
| '(' expr ')'                      { $$ = $2; }
| func-call                         { $$ = $1; }
| '+' expr %prec SIGN               { $$ = new Plus($2); }
| '-' expr %prec SIGN               { $$ = new Minus($2); }
| expr '+' expr                     { $$ = new BinOp($1, $2, $3); }
| expr '-' expr                     { $$ = new BinOp($1, $2, $3); }
| expr '*' expr                     { $$ = new BinOp($1, $2, $3); }
| expr "div" expr                   { $$ = new BinOp($1, $2, $3); }
| expr "mod" expr                   { $$ = new BinOp($1, $2, $3); }
;

cond: 
'(' cond ')'                        { $$ = $2; }
| "not" cond                        { $$ = new Not($2); }
| cond "and" cond                   { $$ = new CondOp($1, $2, $3); }
| cond "or" cond                    { $$ = new CondOp($1, $2, $3); }
| expr '=' expr                     { $$ = new CondOp($1, $2, $3); }
| expr '#' expr                     { $$ = new CondOp($1, $2, $3); }
| expr '<' expr                     { $$ = new CondOp($1, $2, $3); }
| expr '>' expr                     { $$ = new CondOp($1, $2, $3); }
| expr T_leq expr                   { $$ = new CondOp($1, $2, $3); }
| expr T_meq expr                   { $$ = new CondOp($1, $2, $3); }
;


%%

void yyerror(const char *msg){
  fprintf(stderr, "Syntax Error: %s\n", msg);
  exit(42);
}

int main() {
  int result = yyparse();
  if (result == 0) printf("Successful parsing\n");
  return result;
}