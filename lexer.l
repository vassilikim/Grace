%option noyywrap

%{
#include <string.h>

#define T_eof     0000
#define T_and     1000
#define T_char    1001
#define T_div     1002
#define T_do      1003
#define T_else    1004
#define T_fun     1005
#define T_if      1006
#define T_int     1007
#define T_not     1008
#define T_nothing 1009
#define T_or      1010
#define T_ref     1011
#define T_return  1012
#define T_then    1013
#define T_var     1014
#define T_while   1015
#define T_mod     1016

#define T_id        2000
#define T_const     2001
#define T_constchar 2002
#define T_string    2003

#define T_leq     3000
#define T_meq     3001
#define T_assign  3002

int line = 1;
int last = 0;

void illegal_char(char character);
void check_last();
%}

L [a-zA-Z]
D [0-9]
W [ \t\r]


%%
"and"     { return T_and;}
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

\$\n                                {line++;}
\$[^\$\n].*\n                       {line++;}
\$                                  {check_last(); last = 1;}
\$[^\$\n].*                         {check_last(); last = 1;}
\$\$([^\$]|(\n)|(\$[^\$]))*\$\$     {
                                        for(int i =0; i<strlen(yytext);i++){
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

int main (int argc, char *argv[]) {
    yyin = fopen(argv[1], "r");
    int token;
    do {
        token = yylex();
        printf("token=%d, lexeme=\"%s\"\n",
        token, yytext);
    } while (token != T_eof);
    fclose(yyin);
    return 0;
}
