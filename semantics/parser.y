%{
#include <cstring>
#include "lexer.hpp"
#include "ast.hpp"


extern int yylineno;
extern char *yytext;
extern bool optimOption;

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
  TypeBts *type;
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
                                            printf("- \033[1;35mAST\033[0m:\n");
                                            std::cout << "=================================================" << std::endl; 
                                            std::cout << "Remove the comment in the parser.y line 103 to display it." << std::endl; 
                                            std::cout << *$1 << std::endl; 
                                            std::cout << "=================================================" << std::endl; 
                                            printf("- \033[1;33mParsing\033[0m: \033[1;32mPASSED\n\033[0m");
                                            parsingTree->sem();
                                            printf("- \033[1;33mSemantic analysis\033[0m: \033[1;32mPASSED\033[0m\n");
                                            parsingTree->llvm_compile_and_dump();
                                            delete $1; }
;

func-def: 
    header local-def_list block         { $$ = new FuncDef($1, $2, $3, yylineno); }
;

local-def_list: 
    /*nothing*/                         { $$ = new LocalDefList(); }
|   local-def_list local-def            { $1->append($2); $$ = $1; }
;

header: 
    "fun" T_id '(' ')' ':' ret-type                 { $$ = new Header($2, new FparDefList(), $6, yylineno); }
|   "fun" T_id '(' fpar-def_list ')' ':' ret-type   { $$ = new Header($2, $4, $7, yylineno); }
;

fpar-def_list: 
    fpar-def                            { $$ = new FparDefList(); $$->append($1);}
|   fpar-def_list ';' fpar-def          { $1->append($3); $$ = $1; }
;

fpar-def:
    T_id id_list ':' fpar-type          { $2->putinfront($1); $$ = new FparDef(yylineno, $2, $4); }
|   "ref" T_id id_list ':' fpar-type    { $3->putinfront($2); $$ = new FparDef(yylineno, $3, $5, true); }
;

id_list: 
    /*nothing*/                         { $$ = new IdList(); }      
|   id_list ',' T_id                    { $1->append($3); $$ = $1; }
;

data-type: 
    "int"                               { $$ = TYPE_int; }                
|   "char"                              { $$ = TYPE_char; }
;

type:
    data-type int-const_list            { $$ = new TypeBts($1, $2);}
;

int-const_list: 
    /*nothing*/                         { $$ = new ConstList(); }   
|   int-const_list '[' T_const ']'      { if ($3 == 0) {$1->append(-1);} else {$1->append($3);}; $$ = $1; }
;

ret-type: 
    data-type                           { $$ = $1; }
|   "nothing"                           { $$ = TYPE_nothing; }
;

fpar-type: 
    data-type                                   { $$ = new FparType($1); }
|   data-type '[' ']' int-const_list            { $4->putinfront(0); $$ = new FparType($1, $4); }
|   data-type '[' T_const ']' int-const_list    { if ($3 == 0) {$5->putinfront(-1);} else {$5->putinfront($3);}; $$ = new FparType($1, $5); }
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
    "var" T_id id_list ':' type ';'     { $3->putinfront($2); $$ = new VarDef($3, $5, yylineno); }
;

stmt: 
    ';'                                 { $$ = new BlankStmt(); }
|   l-value "<-" expr ';'               { $$ = new Assignment($1, $3, yylineno); }
|   block                               { $$ = $1; }
|   func-call ';'                       { $1->makeStmt(); $$ = $1; }
|   "if" cond "then" stmt               { $$ = new If($2, $4, yylineno); }
|   "if" cond "then" stmt "else" stmt   { $$ = new If($2, $4, yylineno, $6); }
|   "while" cond "do" stmt              { $$ = new While($2, $4, yylineno); }
|   "return" ';'                        { $$ = new Return(yylineno); }
|   "return" expr ';'                   { $$ = new Return(yylineno, $2); }
;

block:
    '{' stmt_list '}'                   { $$ = $2; }
;

stmt_list: 
    /*nothing*/                         { $$ = new Block(); }
|   stmt_list stmt                      { $1->append($2); $$ = $1; }
;

func-call: 
    T_id '(' ')'                        { $$ = new FuncCall($1, yylineno); }
|   T_id '(' expr expr_list ')'         { $4->putinfront($3); $$ = new FuncCall($1, yylineno, $4); }
;

expr_list: 
    /*nothing*/                         { $$ = new ExprList(yylineno); }
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
  printf("- Compilation \033[1;31mFAILED\033[0m.\n");
  exit(42);
}

int main(int argc, char *argv[]) {
    if(argc==2) optimOption = true;
    else optimOption = false;
    int result = yyparse();
    return result;
}