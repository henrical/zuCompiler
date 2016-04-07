%option c++ yyclass="zu_scanner" outfile="zu_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
/* $Id: zu_scanner.l,v 1.2 2016/04/06 16:20:26 ist175838 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include "math.h"

#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "zu_scanner.h"
#include "zu_parser.tab.h"

// don't change this
#define yyerror LexerError

void print(std::string msg, int lineno)
{
    std::cout << "Line " << lineno << ": " << msg << std::endl;
}

void printLexeme(std::string lexeme, int lineno)
{
    std::cout << "::: Read lexeme [" << lexeme << "] at line " <<lineno << std::endl;
}

//Converte hexadecimal para decimal
int hex2dec(std::string hex)
{
    int result = 0;
    for (int i=0; i<hex.length(); i++) {
        if (hex[i]>=48 && hex[i]<=57)
        {
            result += (hex[i]-48)*pow(16,hex.length()-i-1);
        } else if (hex[i]>=65 && hex[i]<=70) {
            result += (hex[i]-55)*pow(16,hex.length( )-i-1);
        } else if (hex[i]>=97 && hex[i]<=102) {
            result += (hex[i]-87)*pow(16,hex.length()-i-1);
        }
    }
    return result;
}

%}
/* Um unico caracter hexadecimal, alfabetico e alfanumerico  */
HEXNUM                      [a-fA-F0-9]
ALPHA                       [a-zA-Z]
ALPHANUM                    [0-9a-zA-z]

WHITESPACE                  [ \t\n]+
IDENTIFIER                  [A-Za-z][A-Za-z0-9_]*

/* Usado para identificar o inicio de uma string  */
STRING_LIM                  \"

/* Um backslash (\)  */
BACKSLASH                   \\

/* Quando aparece numa string, a string termina nesse ponto.  */
ASCII_NULL                  \\0

LINE_COMMENT                \/\/.*
COMMENT                     \/\*((\*+[^/*])|([^*]))*\**\*\/


/* X_STRING - estamos dentro de uma string (" ") .
   X_STRING_SPECIAL - encontramos um \ e estamos a ler um caracter especial dentro de uma string (\n, \12, \").
*/
%x X_STRING X_STRING_SPECIAL 
%%

{LINE_COMMENT}                                  ; print("skipping line comment", yylineno);
{COMMENT}                                       ; print("skipping multi-line comment", yylineno);

">="                                            return tGE; 
"<="                                            return tLE; 
"=="                                            return tEQ; 
"!="                                            return tNE; 

[A-Za-z][A-Za-z0-9_]*                           yylval.s = new std::string(yytext); printLexeme(*yylval.s, yylineno); return tIDENTIFIER;


{STRING_LIM}                                    yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>{STRING_LIM}{WHITESPACE}{STRING_LIM}  ;
<X_STRING>{STRING_LIM}                          yy_pop_state(); printLexeme(*yylval.s, yylineno); return tSTRING;
<X_STRING>{BACKSLASH}                           yy_push_state(X_STRING_SPECIAL);
<X_STRING>.                                     *yylval.s += yytext;

<X_STRING_SPECIAL>n                             yy_pop_state(); *yylval.s += std::string(1, 10);/* 10 e o cod. ascii do \n */
<X_STRING_SPECIAL>t                             yy_pop_state(); *yylval.s += std::string(1, 9);/* 9 e o cod. ascii do \t */
<X_STRING_SPECIAL>r                             yy_pop_state(); *yylval.s += std::string(1, 13);/* 13 e o cod. ascii do \r */
<X_STRING_SPECIAL>\"                            yy_pop_state(); *yylval.s += std::string(1, 34);/* 34 e o cod. ascii da aspa */
<X_STRING_SPECIAL>\\                            yy_pop_state(); *yylval.s += std::string(1, 92);/* 92 e o cod. ascii do backslash */
<X_STRING_SPECIAL>{HEXNUM}{HEXNUM}              yy_pop_state(); *yylval.s += std::string(1, hex2dec(yytext)); //hex2dec converte hexadecimal para decimal
<X_STRING_SPECIAL>{HEXNUM}                      yy_pop_state(); *yylval.s += std::string(1, hex2dec(yytext));

[0-9]+                                          yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;

[-()<>=+*/%;{}.]                                return *yytext;

{WHITESPACE}                                    ; /* ignore whitespace */

.                                               yyerror("Unknown character");

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int zu_scanner::yywrap() { return 1; }
