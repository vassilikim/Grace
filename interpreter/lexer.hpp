#ifndef __LEXER_HPP__
#define __LEXER_HPP__

int yylex();
void yyerror(const char *msg);
void illegal_char(char character);
void check_last();

#endif