%{
// $Id: zu_parser.y,v 1.5 2016/04/11 20:36:33 ist175838 Exp $
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
  double               d;       /* double value */
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  basic_type           *type;
  zu::lvalue_node      *lvalue;
  zu::function_call_node *fdef;
};

/* tLSTRING: string literal. 
*/
%token <i> tLINTEGER 
%token <s> tIDENTIFIER tLSTRING  
%token <d> tLDOUBLE
%token tWHILE tIF tPRINT tREAD tBEGIN tEND tSTRING tDOUBLE tINTEGER tGT tLT tIDENTIFIER

%nonassoc tIFX
%nonassoc tELSE

%right '='
%left tGE tLE tEQ tNE tGT tLT
%left '+' '-'
%left '&' '|'
%left '*' '/' '%'
%nonassoc tUNARY

%type <node> assign_variable
%type <sequence> stmt list declare_var_node
%type <expression> expr
%type <lvalue> lval declare_variable
%type <type> type

//FIXME: completar lista <node>:regra

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file : list                             { compiler->ast($1);}
     |                                  { compiler->ast(new cdk::nil_node(LINE));}
     ;

list : stmt	                        { $$ = new cdk::sequence_node(LINE, $1); }
     | list stmt                        { $$ = new cdk::sequence_node(LINE, $2, $1); }
     ;

stmt : assign_variable ';'              { }
     | declare_function                         {}
     ;
   
/* id_func!() -> funçao global 
   id_func?() -> funcao definida noutro modulo
   
   !id_func() -> retorna void
   falta aqui funçao() = literal
*/
/*declare_function : type tIDENTIFIER  '(' ')'        {$$ = new zu::function_declaration_node(LINE, $1, NULL, true, false, NULL ); }
         | type tIDENTIFIER  '(' arguments ')'      {$$ = new zu::function_declaration_node(LINE, $1, NULL, true, false, $4 ); }
         | type tIDENTIFIER '!' '(' ')'             {$$ = new zu::function_declaration_node(LINE, $1, NULL, false, false, NULL); }
         | type tIDENTIFIER '!' '(' arguments ')'   {$$ = new zu::function_declaration_node(LINE, $1, NULL, false, false, $5);}
         | type tIDENTIFIER '?' '(' ')'             {$$ = new zu::function_declaration_node(LINE, $1, NULL, true, true, NULL ); }
         | type tIDENTIFIER '?' '(' arguments ')'   {$$ = new zu::function_declaration_node(LINE, $1, NULL, true, true, $5 );}
         ;*/
/*         | tDOUBLE tIDENTIFIER '(' ')'                   { }
         | tDOUBLE tIDENTIFIER '(' arguments ')'         { }
         | tDOUBLE tIDENTIFIER '!' '(' ')'               { }
         | tDOUBLE tIDENTIFIER '!' '(' arguments')'      { }
         | tDOUBLE tIDENTIFIER '?' '(' ')'               { }
         | tDOUBLE tIDENTIFIER '?' '(' arguments ')'     { }
         
         | tSTRING tIDENTIFIER  '(' ')'                 { }
         | tSTRING tIDENTIFIER  '(' arguments ')'       { }
         | tSTRING tIDENTIFIER '!' '(' ')'              { }
         | tSTRING tIDENTIFIER '!' '(' arguments ')'    { }
         | tSTRING tIDENTIFIER '?' '(' ')'              { }
         | tSTRING tIDENTIFIER '?' '(' arguments ')'    { }
         
         | '!' tIDENTIFIER '(' ')'                      {}
         | '!' tIDENTIFIER '(' arguments ')'            {}
         | '!' tIDENTIFIER '!' '(' ')'                  {}
         | '!' tIDENTIFIER '!' '(' arguments ')'        {}
         | '!' tIDENTIFIER '!' '(' ')'                  {}
         | '!' tIDENTIFIER '?' '(' arguments ')'        {}
         ;*/
   
/* Os argumentos de uma funçao: uma ou mais variaveis. */
/*arguments: assign_variable                                     {$$ = new cdk::sequence_node(LINE, $1);}
         | arguments ',' assign_variable                       {$$ = new cdk::sequence_node(LINE, $3 ,$1);}
         ;*/


declare_variable : type tIDENTIFIER                     { /*$$ = new zu::declare_var_node(LINE,$1, $2, true, false, false, false);*/}
                 | type tIDENTIFIER '!'                 { /*$$ = new zu::declare_var_node(LINE,$1, $2, false, false, false, false);*/}             
                 | type tIDENTIFIER '?'                 { /*$$ = new zu::declare_var_node(LINE,$1, $2, true, true, false, false);*/}
                 ; 
      
/*assign_variable : declare_variable '=' expr             { $$ = new zu::assignment_node(LINE, $1, $3 ) }         
                | declare_variable                      { $$ = $1; }
                ;*/
                
/* tipos primitivos */
type : tINTEGER                                         { $$ = new basic_type(4, basic_type::TYPE_INT); }
     | tDOUBLE                                          { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
     | tSTRING                                          { $$ = new basic_type(4, basic_type::TYPE_STRING); }
     | '<' type '>'                                     { $$ = new basic_type(4, basic_type::TYPE_POINTER); }
     ;
      
/*Falta:
    indexaçao?
    
*/
expr : tLINTEGER                       { $$ = new cdk::integer_node(LINE, $1); }
     | tLSTRING                        { $$ = new cdk::string_node(LINE, $1); }
     | tLDOUBLE                        { $$ = new cdk::double_node(LINE, $1);}
     
     | '-' expr %prec tUNARY           { $$ = new zu::symmetry_node(LINE, $2); }
     | '+' expr %prec tUNARY           { $$ = new zu::identity_node(LINE, $2);}
     | expr '?' %prec tUNARY           { $$ = new zu::memory_address_node(LINE, $1);/* Not sure about this*/} 
     
     /*funciona so so inteiros, dado que nao existem booleanos. ~0 = 1 = false e ~1 = 0 = true */
     | '~' expr %prec tUNARY           { $$ = new cdk::neg_node(LINE, $2); } 
      
     | expr '|' expr                   { $$ = new zu::or_node(LINE, $1, $3); }
     | expr '&' expr                   { $$ = new zu::and_node(LINE, $1, $3); }
     
     | expr '+' expr	               { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr	               { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr	               { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr	               { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr	               { $$ = new cdk::mod_node(LINE, $1, $3); } /* '%' e apenas para inteiros */
     | expr tLT expr	               { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr tGT expr	               { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	               { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr                   { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	               { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr	               { $$ = new cdk::eq_node(LINE, $1, $3); }
     
     | '(' expr ')'                    { $$ = $2; }
     | lval                            { $$ = new zu::rvalue_node(LINE, $1); }  //FIXME
     | lval '=' expr                   { $$ = new zu::assignment_node(LINE, $1, $3); }
     ;

lval : tIDENTIFIER                     { $$ = new zu::lvalue_node(LINE, $1); }
     | expr '[' expr ']'               { $$ = new zu::index_node(LINE, "no idea what to put here", $1, $3); /* o que por no 2º parametro? */}
     ;
     
epsilon :
        ;
%%
