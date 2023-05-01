%{
#include <cstring>
#include "lexer.hpp"
#include "ast.hpp"
extern int yylineno;
extern char *yytext;

SymbolTable st;
%}
%locations

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
%token<constchar> T_constchar
%token<str> T_string

%token T_leq    "<="
%token T_meq    ">="
%token T_assign "<-"

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
  Datatype datatype;
  Type *type;
  ConstList *constlist;
  Datatype rettype;
  FparType *fpartype;
  Func *func;
  Stmt *stmt;
  Block *block;
  FuncCall *funccall;
  ExprList *exprlist;
  Expr *expr;
  char var;
  int num;
  char *constchar;
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
    func-def                            {   FuncDef *parsingTree = $1;
                                            // std::cout << "AST: " << *$1 << std::endl; 
                                            printf("\033[1;32mSucessful parsing.\n\033[0m");
                                            parsingTree->sem(); delete $1; }
;

func-def: 
    header local-def_list block         { $$ = new FuncDef($1, $2, $3); }
;

local-def_list: 
    /*nothing*/                         { $$ = new LocalDefList(); }
|   local-def_list local-def            { $1->append($2); $$ = $1; }
;

header: 
    "fun" T_id '(' ')' ':' ret-type                 { $$ = new Header($2, new FparDefList(), $6); }
|   "fun" T_id '(' fpar-def_list ')' ':' ret-type   { $$ = new Header($2, $4, $7); }
;

fpar-def_list: 
    fpar-def                            { $$ = new FparDefList(); $$->append($1);}
|   fpar-def_list ';' fpar-def          { $1->append($3); $$ = $1; }
;

fpar-def:
    T_id id_list ':' fpar-type          { Id* id = new Id($1, yylineno); $2->putinfront(id); $$ = new FparDef($2, $4); }
|   "ref" T_id id_list ':' fpar-type    { Id* id = new Id($2, yylineno); $3->putinfront(id); $$ = new RefFparDef($3, $5); }
;

id_list: 
    /*nothing*/                         { $$ = new IdList(); }      
|   id_list ',' T_id                    { Id* id = new Id($3, yylineno); $1->append(id); $$ = $1; }
;

data-type: 
    "int"                               { $$ = TYPE_int; }                
|   "char"                              { $$ = TYPE_char; }
;

type:
    data-type int-const_list            { $$ = new Type($1, $2);}
;

int-const_list: 
    /*nothing*/                         { $$ = new ConstList(yylineno); }   
|   int-const_list '[' T_const ']'      { Const* c = new Const($3); $1->append(c); $$ = $1; }
;

ret-type: 
    data-type                           { $$ = $1; }
|   "nothing"                           { $$ = TYPE_nothing; }
;

fpar-type: 
    data-type                                   { $$ = new FparType($1); }
|   data-type '[' ']' int-const_list            { Const* c = new Const(0); $4->putinfront(c); $$ = new FparType($1, $4); }
|   data-type '[' T_const ']' int-const_list    { Const* c = new Const($3); $5->putinfront(c); $$ = new FparType($1, $5); }
;

local-def: 
    func-def                            { $$ = $1;}
|   func-decl                           { $$ = $1;}
|   var-def                             { $$ = $1;}
;

func-decl: 
    header ';'                          { $$ = new FuncDecl($1); }
;

var-def: 
    "var" T_id id_list ':' type ';'     { Id* id = new Id($2, yylineno); $3->putinfront(id); $$ = new VarDef($3, $5); }
;

stmt: 
    ';'                                 { $$ = new BlankStmt(); }
|   l-value "<-" expr ';'               { $$ = new Assignment($1, $3); }
|   block                               { $$ = $1; }
|   func-call ';'                       { $$ = $1; }
|   "if" cond "then" stmt               { $$ = new If($2, $4, yylineno); }
|   "if" cond "then" stmt "else" stmt   { $$ = new If($2, $4, yylineno, $6); }
|   "while" cond "do" stmt              { $$ = new While($2, $4, yylineno); }
|   "return" ';'                        { $$ = new Return(); }
|   "return" expr ';'                   { $$ = new Return($2); }
;

block:
    '{' stmt_list '}'                   { $$ = $2; }
;

stmt_list: 
    /*nothing*/                         { $$ = new Block(); }
|   stmt_list stmt                      { $1->append($2); $$ = $1; }
;

func-call: 
    T_id '(' ')'                        { $$ = new FuncCall($1); }
|   T_id '(' expr expr_list ')'         { $4->putinfront($3); $$ = new FuncCall($1, $4); }
;

expr_list: 
    /*nothing*/                         { $$ = new ExprList(); }
|   expr_list ',' expr                  { $1->append($3); $$ = $1; }
;

l-value: 
    T_id                                { $$ = new Id($1, yylineno); }
|   T_string                            { $$ = new String($1); }
|   l-value '[' expr ']'                { $$ = new ArrayCall($1, $3, yylineno); }
;

expr: 
    T_const                             { $$ = new Const($1); }
|   T_constchar                         { $$ = new ConstChar($1); }
|   l-value                             { $$ = $1; }
|   '(' expr ')'                        { $$ = $2; }
|   func-call                           { $$ = $1; }
|   '+' expr %prec SIGN                 { $$ = new Plus($2, yylineno); }
|   '-' expr %prec SIGN                 { $$ = new Minus($2, yylineno); }
|   expr '+' expr                       { $$ = new BinOp($1, $2, $3, yylineno); }
|   expr '-' expr                       { $$ = new BinOp($1, $2, $3, yylineno); }
|   expr '*' expr                       { $$ = new BinOp($1, $2, $3, yylineno); }
|   expr "div" expr                     { $$ = new BinOp($1, $2, $3, yylineno); }
|   expr "mod" expr                     { $$ = new BinOp($1, $2, $3, yylineno); }
;

cond: 
    '(' cond ')'                        { $$ = $2; }
|   "not" cond                          { $$ = new Not($2, yylineno); }
|   cond "and" cond                     { $$ = new CondOp($1, $2, $3, yylineno); }
|   cond "or" cond                      { $$ = new CondOp($1, $2, $3, yylineno); }
|   expr '=' expr                       { $$ = new CondOp($1, $2, $3, yylineno); }
|   expr '#' expr                       { $$ = new CondOp($1, $2, $3, yylineno); }
|   expr '<' expr                       { $$ = new CondOp($1, $2, $3, yylineno); }
|   expr '>' expr                       { $$ = new CondOp($1, $2, $3, yylineno); }
|   expr "<=" expr                      { $$ = new CondOp($1, $2, $3, yylineno); }
|   expr ">=" expr                      { $$ = new CondOp($1, $2, $3, yylineno); }
;


%%

void yyerror(const char *msg){



  printf("\033[1;31m%s:\n\t\033[0m", msg);
  printf("Cannot parse token: ");
  printf("\033[1;35m%s\033[0m", yytext);
  printf(" -- line: ");
  printf("\033[1;36m%d\n\033[0m", yylineno);
  exit(42);
}

int main() {
  int result = yyparse();
  if (result == 0) printf("\033[1;32mSemantics checked.\n\033[0m");
  return result;
}
