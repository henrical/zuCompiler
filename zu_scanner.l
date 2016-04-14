%option c++ yyclass="zu_scanner" outfile="zu_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
/* $Id: zu_scanner.l,v 1.6 2016/04/12 19:47:25 ist175838 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include "math.h"

#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "zu_scanner.h"
#include "zu_parser.tab.h"

// don't change this
#define yyerror LexerError

//Estas funçoes servem para debug.
//Token e o tipo do que foi lido, lexema e o que foi lido
// "texto" -> token=string, lexema=texto
void print(std::string msg, int lineno)
{
    std::cout << "Line " << lineno << ": " << msg << std::endl;
}

void print(std::string msg)
{
    std::cout << msg << std::endl;
}

void printLexeme(int lexeme, int lineno)
{
    std::cout << "::: Read lexeme [" << lexeme << "] at line " <<lineno << std::endl;
}

void printLexeme(std::string token, int lexeme, int lineno)
{
    std::cout << "::: Read token \"" << token << "\", lexeme [" << lexeme << "] at line " <<lineno << std::endl;
}

void printLexeme(std::string token, double lexeme, int lineno)
{
    std::cout << "::: Read token \"" << token << "\", lexeme [" << lexeme << "] at line " <<lineno << std::endl;
}

void printLexeme(std::string lexeme, int lineno)
{
    std::cout << "::: Read lexeme [" << lexeme << "] at line " <<lineno << std::endl;
}

void printLexeme(std::string token, std::string lexeme, int lineno)
{
    std::cout << "::: Read token \"" << token << "\", lexeme [" << lexeme << "] at line " <<lineno << std::endl;
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
/*[-()<>=+*%/;{}.]*/ 
/* Um unico caracter hexadecimal, alfabetico e alfanumerico  */
HEXNUM                      [a-fA-F0-9]
ALPHA                       [a-zA-Z]
ALPHANUM                    [0-9a-zA-z]

WHITESPACE                  [ \t\n]+
/* whitespace opcional (pode estar ou nao estar) */
OPT_WHITESPACE              [ \t\n]*

IDENTIFIER                  [A-Za-z][A-Za-z0-9_]*

/* Usado para identificar o inicio ou fim de uma string  */
STRING_LIM                  \"

/* Um backslash (\)  */
BACKSLASH                   \\

/* Quando aparece numa string, a string termina nesse ponto.  */
ASCII_NULL                  \\0

LINE_COMMENT                \/\/.*
COMMENT                     \/\*((\*+[^/*])|([^*]))*\**\*\/

/* ========================================== */

/* X_STRING: estado para quando entramos numa string.
   X_EOF_STRING: estado para quando estamos numa string e encontramos '\0'. Dai ate ao final da string ignora-se os caracteres.
   X_STRING_SPECIAL: estado para quando estamos numa string e encontramos '\', que e o inicio de um caracter especial.
*/
%x X_STRING X_STRING_SPECIAL X_EOF_STRING
%%

{LINE_COMMENT}                                  ; /*print("skipping line comment", yylineno);*/
{COMMENT}                                       ; /*print("skipping multi-line comment", yylineno);*/

"!!"                                            return tNLPRINT;
"!!!"                                           return tRETURN;

"<>"                                            /*printLexeme("break", yytext, yylineno);*/return tBREAK;
"><"                                            /*printLexeme("continue", yytext, yylineno);*/return tCONTINUE;

">="                                           /* printLexeme("greater|equal than", yytext, yylineno);*/return tGE; 
"<="                                            /*printLexeme("less|equal than", yytext, yylineno);*/return tLE; 


"=="                                            /*printLexeme("equal to", yytext, yylineno);*/return tEQ; 
"!="                                            /*printLexeme("different than", yytext, yylineno);*/return tNE; 

"#"                                             return *yytext;; 
"$"                                             /*printLexeme("string", "$", yylineno);*/ return tSTRING; 
"%"                                             /*printLexeme("float", "%", yylineno);*/  return tDOUBLE; 

"<"                                             return *yytext;  
">"                                             return *yytext; 

[A-Za-z][A-Za-z0-9_]*                           yylval.s = new std::string(yytext); /*printLexeme("identifier", *yylval.s, yylineno);*/ return tIDENTIFIER;


{STRING_LIM}                                        yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>{STRING_LIM}{OPT_WHITESPACE}{STRING_LIM}      ;
<X_STRING>{STRING_LIM}{OPT_WHITESPACE}{COMMENT}{OPT_WHITESPACE}{STRING_LIM}   ;/* esta regra serve para ignorar comentarios entre strings */  
<X_STRING>{STRING_LIM}                              yy_pop_state(); /*printLexeme("l_string", *yylval.s, yylineno);*/ return tLSTRING;

<X_STRING>{BACKSLASH}0                              yy_push_state(X_EOF_STRING); /* encontrar '\0' faz ignorar o resto da string */

<X_STRING>{BACKSLASH}                               yy_push_state(X_STRING_SPECIAL); /* encontrar '\' faz ir para estado de caracter especial */

<X_STRING>.                                         *yylval.s += yytext;
<X_STRING>\n                                        yyerror("Error: Newline inside string.");

<X_EOF_STRING>{STRING_LIM}                      { yyless(yyleng - 1); /*isto devolve uma aspa ao input para o estado X_STRING ler*/
                                                  yy_pop_state(); }
<X_EOF_STRING>.                                 ; /* caracteres depois de '\0' sao ignorados */


<X_STRING_SPECIAL>n                             yy_pop_state(); *yylval.s += std::string(1, 10);/* 10 e o cod. ascii do \n */
<X_STRING_SPECIAL>t                             yy_pop_state(); *yylval.s += std::string(1, 9);/* 9 e o cod. ascii do \t */
<X_STRING_SPECIAL>r                             yy_pop_state(); *yylval.s += std::string(1, 13);/* 13 e o cod. ascii do \r */
<X_STRING_SPECIAL>\"                            yy_pop_state(); *yylval.s += std::string(1, 34);/* 34 e o cod. ascii da aspa */
<X_STRING_SPECIAL>\\                            yy_pop_state(); *yylval.s += std::string(1, 92);/* 92 e o cod. ascii do backslash */
<X_STRING_SPECIAL>{HEXNUM}{HEXNUM}              yy_pop_state(); *yylval.s += std::string(1, hex2dec(yytext)); //hex2dec converte hexadecimal para decimal
<X_STRING_SPECIAL>{HEXNUM}                      yy_pop_state(); *yylval.s += std::string(1, hex2dec(yytext));


[0-9]+[.][0-9]+                                 yylval.d = std::stod(yytext, nullptr); /*printLexeme("l_double", yylval.d, yylineno);*/  return tLDOUBLE;

[0-9]+                                          yylval.i = strtol(yytext, nullptr, 10); /*printLexeme("l_integer", yylval.i, yylineno);*/ return tLINTEGER;

[-+?!*()=;,{}]                                  return *yytext;

{WHITESPACE}                                    ; /* ignore whitespace */

.                                               yyerror("Unknown character");

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int zu_scanner::yywrap() { return 1; }
