%{
// $Id: zu_parser.y,v 1.4 2016/04/09 16:50:10 ist175838 Exp $
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE       compiler->scanner()->lineno()
#define yylex()    compiler->scanner()->scan()
#define yyerror(s) compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                  i;	/* integer value */
  float                f;       /* float value */
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  zu::lvalue_node      *lvalue;
};

/* tLSTRING: string literal. 
*/
%token <i> tLINTEGER 
%token <s> tIDENTIFIER tLSTRING  
%token <f> tLFLOAT
%token tWHILE tIF tPRINT tREAD tBEGIN tEND tSTRING tFLOAT tINTEGER

%nonassoc tIFX
%nonassoc tELSE

%right '='
%left tGE tLE tEQ tNE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY

%type <node> stmt 
%type <sequence> list
%type <expression> expr
%type <lvalue> lval
//FIXME: completar lista <node>:regra

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%


list : stmt	                        { $$ = new cdk::sequence_node(LINE, $1); }
    | list stmt                         { $$ = new cdk::sequence_node(LINE, $2, $1); }
    ;

stmt : variable ';'                     {}
     | function                         {}
   /*: expr ';'                         { $$ = new zu::evaluation_node(LINE, $1); }
     | tPRINT expr ';'                  { $$ = new zu::print_node(LINE, $2); }
     | tREAD lval ';'                   { $$ = new zu::read_node(LINE, $2); }
     | tIF '(' expr ')' stmt tELSE stmt { $$ = new zu::if_else_node(LINE, $3, $5, $7); }
     | '{' list '}'                     { $$ = $2; }
   */;
   
/* id_func!() -> funçao global 
   id_func?() -> funcao definida noutro modulo
   
   !id_func() -> retorna void
*/
function : tINTEGER tIDENTIFIER  '(' ')'                {/*FIXME*/}
         | tINTEGER tIDENTIFIER  '(' arguments ')'      {/*FIXME*/}
         | tINTEGER tIDENTIFIER '!' '(' ')'             {/*FIXME*/}
         | tINTEGER tIDENTIFIER '!' '(' arguments ')'   {/*FIXME*/}
         | tINTEGER tIDENTIFIER '?' '(' ')'             {/*FIXME*/}
         | tINTEGER tIDENTIFIER '?' '(' arguments ')'   {/*FIXME*/}
         
         | tFLOAT tIDENTIFIER '(' ')'                   {/*FIXME */}
         | tFLOAT tIDENTIFIER '(' arguments ')'         {/*FIXME */}
         | tFLOAT tIDENTIFIER '!' '(' ')'               {/*FIXME */}
         | tFLOAT tIDENTIFIER '!' '(' arguments')'      {/*FIXME */}
         | tFLOAT tIDENTIFIER '?' '(' ')'               {/*FIXME */}
         | tFLOAT tIDENTIFIER '?' '(' arguments ')'     {/*FIXME */}
         
         | tSTRING tIDENTIFIER  '(' ')'                 {/*FIXME*/}
         | tSTRING tIDENTIFIER  '(' arguments ')'       {/*FIXME*/}
         | tSTRING tIDENTIFIER '!' '(' ')'              {/*FIXME*/}
         | tSTRING tIDENTIFIER '!' '(' arguments ')'    {/*FIXME*/}
         | tSTRING tIDENTIFIER '?' '(' ')'              {/*FIXME*/}
         | tSTRING tIDENTIFIER '?' '(' arguments ')'    {/*FIXME*/}
         
         | '!' tIDENTIFIER '(' ')'                      {/* funçao que retorna void:FIXME*/}
         | '!' tIDENTIFIER '(' arguments ')'            {/* funçao que retorna void:FIXME*/}
         | '!' tIDENTIFIER '!' '(' ')'                  {/* funçao que retorna void:FIXME*/}
         | '!' tIDENTIFIER '!' '(' arguments ')'        {/* funçao que retorna void:FIXME*/}
         | '!' tIDENTIFIER '!' '(' ')'                  {/* funçao que retorna void:FIXME*/}
         | '!' tIDENTIFIER '?' '(' arguments ')'        {/* funçao que retorna void:FIXME*/}
         ;
   
/* Os argumentos de uma funçao: uma ou mais variaveis. */
arguments: variable                                     {}
         | arguments ',' variable                       {}
         ;
         
/* falta implementar [= expressao]  */
variable : type tIDENTIFIER
         | type tIDENTIFIER '!'
         | type tIDENTIFIER '?'
         | 
         ; 
      
/* tipos primitivos */
type : tINTEGER                                         {}
     | tFLOAT                                           {}
     | tSTRING                                          {}
     | '<' type '>'
     ;
      
expr : tLINTEGER                       { $$ = new cdk::integer_node(LINE, $1); }
     | tLSTRING                        { $$ = new cdk::string_node(LINE, $1); }
     | tLFLOAT                         { $$ = new cdk::double_node(LINE, $1);}
     | '-' expr %prec tUNARY           { $$ = new cdk::neg_node(LINE, $2); }
     | expr '+' expr	               { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr	               { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr	               { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr	               { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr	               { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '<' expr	               { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr	               { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	               { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr                   { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	               { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr	               { $$ = new cdk::eq_node(LINE, $1, $3); }
     | '(' expr ')'                    { $$ = $2; }
     | lval                            { $$ = new zu::rvalue_node(LINE, $1); }  //FIXME
     | lval '=' expr                   { $$ = new zu::assignment_node(LINE, $1, $3); }
     ;

lval : tIDENTIFIER             { $$ = new zu::lvalue_node(LINE, $1); }
     ;
     
epsilon :
        ;
%%
