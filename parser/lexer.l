%option noyywrap

%{
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include "lexer.hpp"
#include "parser.hpp"


int line = 1;
int last = 0;
%}

L [a-zA-Z]
D [0-9]
W [ \t\r]


%%
"and"     {return T_and;}
"char"    {return T_char;}
"div"     {return T_div;}
"do"      {return T_do;}
"else"    {return T_else;}
"fun"     {return T_fun;}
"if"      {return T_if;}
"int"     {return T_int;}
"mod"     {return T_mod;}
"not"     {return T_not;}
"nothing" { return T_nothing;}
"or"      {return T_or;}
"ref"     {return T_ref;}
"return"  {return T_return;}
"then"    {return T_then;}
"var"     {return T_var;}
"while"   {return T_while;}

{L}({L}|{D}|_)* {return T_id;}
{D}+            {return T_const;}

(\'[^\\\'\"]\')|(\'(\\n|\\t|\\r|\\0|\\\\|\\\'|\\\")\')|(\'\\x{D}{D}\') {return T_constchar;}

\"(([^\\\'\"\n])|(\\n|\\t|\\r|\\0|\\\\|\\\'|\\\")|(\\x{D}{D}))*\" {return T_string;}

[\+\-\*\=\#\>\<]    {return yytext[0];}
">="                {return T_meq;}
"<="                {return T_leq;}

[\(\)\[\]\{\}\,\;\:]    {return yytext[0];}
"<-"                    {return T_assign;}

\n    {line++;}
{W}+  {/*nothing*/}

\$\n                                {check_last();line++;}
\$[^\$\n].*\n                       {check_last();line++;}
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
    if (character == '$')
        printf("expected \"$$\" at the end of the comment -- line: %d", line);
    else printf("illegal character: %c -- line: %d", character, line);
    exit(1);
}

void check_last() {
    if(last == 1) {
        illegal_char('$');
    }
}