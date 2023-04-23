%option noyywrap
%option nounput
%option yylineno

%{
#include <string.h>
#include "lexer.hpp"
#include "ast.hpp"
#include "parser.hpp"


#define T_eof     0000

int line = 1;
int last = 0;

void illegal_char(char character);
void check_last();
%}

L [a-zA-Z]
D [0-9]
W [ \t\r]


%%
"and"     {yylval.condop = new char[yyleng + 1]; strcpy(yylval.condop, "and"); return T_and;}
"char"    {return T_char;}
"div"     {yylval.binop = new char[yyleng + 1]; strcpy(yylval.binop, "div"); return T_div;}
"do"      {return T_do;}
"else"    {return T_else;}
"fun"     {return T_fun;}
"if"      {return T_if;}
"int"     {return T_int;}
"mod"     {yylval.binop = new char[yyleng + 1]; strcpy(yylval.binop, "mod"); return T_mod;}
"not"     {return T_not;}
"nothing" { return T_nothing;}
"or"      {yylval.condop = new char[yyleng + 1]; strcpy(yylval.condop, "or"); return T_or;}
"ref"     {return T_ref;}
"return"  {return T_return;}
"then"    {return T_then;}
"var"     {return T_var;}
"while"   {return T_while;}

{L}({L}|{D}|_)* {yylval.str = new char[yyleng + 1]; strcpy(yylval.str, yytext); return T_id;}
{D}+            {yylval.num = atoi(yytext); return T_const;}

(\'[^\\\'\"]\')|(\'(\\n|\\t|\\r|\\0|\\\\|\\\'|\\\")\')|(\'\\x{D}{D}\') {yylval.constchar = new char[yyleng + 1]; strcpy(yylval.constchar, yytext); return T_constchar;}

\"(([^\\\'\"\n])|(\\n|\\t|\\r|\\0|\\\\|\\\'|\\\")|(\\x{D}{D}))*\" {yylval.str = new char[yyleng + 1]; strcpy(yylval.str, yytext); return T_string;}

[\+\-\*]                {yylval.binop = new char[yyleng + 1]; strcpy(yylval.binop, yytext); return yytext[0];}
[\=\#\>\<]              {yylval.condop = new char[yyleng + 1]; strcpy(yylval.condop, yytext); return yytext[0];}
">="                    {yylval.condop = new char[yyleng + 1]; strcpy(yylval.condop, yytext); return T_meq;}
"<="                    {yylval.condop = new char[yyleng + 1]; strcpy(yylval.condop, yytext); return T_leq;}

[\(\)\[\]\{\}\,\;\:]    {return yytext[0];}
"<-"                    {return T_assign;}

\n    {line++;}
{W}+  {/*nothing*/}

\$\n                                {check_last(); line++;}
\$[^\$\n].*\n                       {check_last(); line++;}
\$                                  {check_last(); last = 1;}
\$[^\$\n].*                         {check_last(); last = 1;}
\$\$([^\$]|(\n)|(\$[^\$]))*\$\$     {
                                        for(int i =0; i<int(strlen(yytext));i++){
                                            if(yytext[i] == '\n') line++;   
                                        }
                                    }                            

. {illegal_char( yytext[0] );}


%%
void illegal_char(char character) {
    printf("\033[1;31mlexical error:\n\t\033[0m");
    if (character == '$'){
        printf("Expected \"");
        printf("\033[1;35m%s\033[0m", "$$");
        printf("\" at the end of the comment -- line: ");
    }
    else{
        printf("Illegal character: \'");
        printf("\033[1;35m%c\033[0m", character);
        printf("\' -- line: ");
    }
    printf("\033[1;36m%d\n\033[0m", yylineno);
    exit(1);
}

void check_last() {
    if(last == 1) {
        illegal_char('$');
    }
}