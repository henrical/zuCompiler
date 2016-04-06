%option c++ yyclass="zu_scanner" outfile="zu_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
/* $Id: zu_scanner.l,v 1.1 2016/02/19 19:29:48 david Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "zu_scanner.h"
#include "zu_parser.tab.h"

// don't change this
#define yyerror LexerError

%}
/* Um unico caracter hexadecimal, alfabetico e alfanumerico  */
HEXNUM                      [a-fA-F0-9]
ALPHA                       [a-zA-Z]
ALPHANUM                    [0-9a-zA-z]

WHITESPACE                  [\s\t\n]+
IDENTIFIER                  [A-Za-z][A-Za-z0-9_]*

/* Usado para identificar o inicio de uma string  */
STRING_DELIMITER            \"



/* Um backslash (\)  */
BACKSLASH                   \\

/* Quando aparece numa string, a string termina nesse ponto.  */
ASCII_NULL                  \\0



%x X_STRING X_STRING_SPECIAL
%%

"--".*                                  ; /* ignore comments */

">="                                    return tGE; 
"<="                                    return tLE; 
"=="                                    return tEQ; 
"!="                                    return tNE; 

[A-Za-z][A-Za-z0-9_]*                   yylval.s = new std::string(yytext); return tIDENTIFIER;


{STRING_DELIMITER}                      yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>{STRING_DELIMITER}            yy_pop_state(); return tSTRING;
<X_STRING>{BACKSLASH}                   yy_push_state(X_STRING_SPECIAL);
<X_STRING>.                             *yylval.s += yytext;

<X_STRING_SPECIAL>n                     yy_pop_state(); *yylval.s += std::string(1, 10);
<X_STRING_SPECIAL>t                     yy_pop_state(); *yylval.s += std::string(1, 9);
<X_STRING_SPECIAL>r                     yy_pop_state(); *yylval.s += std::string(1, 13);

[0-9]+                                  yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;

[-()<>=+*/%;{}.]                        return *yytext;

{WHITESPACE}                            ; /* ignore whitespace */

.                                       yyerror("Unknown character");

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int zu_scanner::yywrap() { return 1; }
