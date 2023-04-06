%{
#include <cstdio>
#include "lexer.hpp"
%}

%token T_and     "and"
%token T_char    "char"
%token T_div     "div"
%token T_do      "do"
%token T_else    "else"
%token T_fun     "fun"
%token T_if      "if"
%token T_int     "int"
%token T_not     "not"
%token T_nothing "nothing"
%token T_or      "or"
%token T_ref     "ref"
%token T_return  "return"
%token T_then    "then"
%token T_var     "var"
%token T_while   "while"
%token T_mod     "mod"

%token T_id        
%token T_const     
%token T_constchar 
%token T_string    

%token T_leq     "<="
%token T_meq     ">="
%token T_assign  "<-"

%left "or"
%left "and"
%precedence "not"
%nonassoc  '=' '#' '<' '>' "<=" ">="
%left '+' '-'
%left '*' "div" "mod"
%precedence SIGN

%expect 1

%%

program:
    func-def    
;

func-def:
    header local-def_list block
;

local-def_list:
    /*nothing*/
|   local-def_list local-def
;

header:
    "fun" T_id '(' ')' ':' ret-type
|   "fun" T_id '(' fpar-def-list ')' ':' ret-type
;

fpar-def-list:
    fpar-def
|   fpar-def ';' fpar-def-list
;

fpar-def:
    T_id id_list ':' fpar-type
|   "ref" T_id id_list ':' fpar-type
;

id_list:
    /*nothing*/
|   id_list ',' T_id
;

data-type:
    "int"
|   "char"
;

type:
    data-type int-const_list
;

int-const_list:
    /*nothong*/
|   int-const_list '['T_const']'

ret-type:
    data-type
|   "nothing"
;

fpar-type: 
    data-type
|   data-type '[' ']' int-const_list 
|   data-type '[' T_const ']' int-const_list
;

local-def:
    func-def
|   func-decl
|   var-def
;

func-decl:
    header ';'
;

var-def:
    "var" T_id id_list ':' type ';'
;

stmt:
    ';'
|   l-value "<-" expr ';'
|   block
|   func-call ';'
|   "if" cond "then" stmt 
|   "if" cond "then" stmt "else" stmt 
|   "while" cond "do" stmt 
|   "return" ';'
|   "return" expr ';'
;

block:
    '{' stmt_list '}'
;

stmt_list:
    /*nothing*/
|   stmt_list stmt
;


func-call: 
T_id '(' ')'
| T_id '(' expr expr_list ')'
;

expr_list: 

| expr_list ',' expr
;

l-value:
    T_id
|   T_string
|   l-value '[' expr ']'
;

expr:
    T_const
|   T_constchar
|   l-value
|   '(' expr ')'
|   func-call
|   '+' expr %prec SIGN
|   '-' expr %prec SIGN
|   expr '+' expr
|   expr '-' expr
|   expr '*' expr
|   expr "div" expr
|   expr "mod" expr
;

cond:
    '('cond')'
|   "not" cond
|   cond "and" cond
|   cond "or" cond
|   expr '=' expr
|   expr '#' expr
|   expr '<' expr
|   expr '>' expr
|   expr "<=" expr
|   expr ">=" expr
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
