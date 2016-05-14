%{
// $Id: zu_parser.y,v 1.10 2016/04/15 15:44:27 ist175838 Exp $
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
  int                               i;	/* integer value */
  double                            d;       /* double value */
  std::string                       *s;	/* symbol name or string literal */
  cdk::basic_node                   *node;	/* node pointer */
  cdk::sequence_node                *sequence;
  cdk::expression_node              *expression; /* expression nodes */
  basic_type                        *type;
  zu::lvalue_node                   *lvalue;
  zu::function_declaration_node     *funcdecl;
  cdk::string_node                  *string;
};

/* tLSTRING: string literal. 
*/
%token <i> tLINTEGER 
%token <s> tIDENTIFIER tLSTRING  
%token <d> tLDOUBLE
%token tWHILE tIF tPRINT tREAD tBEGIN tEND tSTRING tDOUBLE  tIDENTIFIER tCONTINUE tBREAK tRETURN tNLPRINT

%right '='
%left tGE tLE tEQ tNE 
%left '+' '-'
%left '&' '|'
%left '*' '/' '%'
%nonassoc tUNARY
%left tLEFT_PREC
%right tRIGHT_PREC

%type <node> stmt assign_variable block instruction cond_instruction
%type <sequence> list arguments exprs instructions
%type <expression> expr literal variable_expr func_call
%type <lvalue> lval declare_variable
%type <type> type
%type <funcdecl> declare_function
%type <string> string_literal

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

stmt : assign_variable ';'              { $$ = $1;}
     | declare_function                 { $$ = $1;}
     | declare_function block           { $$ = new zu::function_definition_node(LINE, $1, $2);} 
     ;

block : '{' list '}'                    { $$ = new zu::block_node(LINE, $2, NULL); }                       
      | '{' instructions'}'             { $$ = new zu::block_node(LINE, NULL, $2); } 
      | '{' '}'                         { $$ = new zu::block_node(LINE, NULL, NULL); }
      | '{' list instructions '}'       { $$ = new zu::block_node(LINE, $2, $3); }
      ;
      
instructions: instruction               { $$ = new cdk::sequence_node(LINE, $1); }
            | instructions instruction  { $$ = new cdk::sequence_node(LINE, $2, $1); }
            ;

instruction : expr ';'                  { $$ = new zu::evaluation_node(LINE, $1);}
            | expr '!'                  {/*impressao sem newline*/$$ = new zu::print_node(LINE, $1, false);}
            | expr tNLPRINT             {/*impressao com newline*/ $$ = new zu::print_node(LINE, $1, true);}
            | tBREAK                    { $$ = new zu::break_node(LINE);}
            | tCONTINUE                 { $$ = new zu::continue_node(LINE);}
            | tRETURN                   { $$ = new zu::return_node(LINE);}
            | cond_instruction          { $$ = $1;}
/*            | loop_instruction          { $$ = $1;}*/
            | block                     { $$ = $1;}
            ;
            
cond_instruction : expr '#' instruction                 { $$ = new zu::if_else_node(LINE, $1, $3, NULL);}
                 | expr '?' instruction                 { $$ = new zu::if_else_node(LINE, $1, $3, NULL);}
                 | expr '?' instruction ':' instruction { $$ = new zu::if_else_node(LINE, $1, $3, $5); }
                 ;
   
/*loop_instruction : '[' exprs ';'       ';'       ']' instruction  
                 | '['       ';' exprs ';'       ']' instruction   
                 | '['       ';'       ';' exprs ']' instruction
                 | '[' exprs ';' exprs ';'       ']' instruction
                 | '['       ';' exprs ';' exprs ']' instruction
                 | '[' exprs ';'       ';' exprs ']' instruction
                 | '[' exprs ';' exprs ';' exprs ']' instruction 
                 | '['       ';'       ';'       ']' instruction
                 ; */

/* id_func!() -> funçao global 
   id_func?() -> funcao definida noutro modulo
   
   !id_func() -> retorna void
*/
declare_function : type tIDENTIFIER  '(' arguments ')'                 {$$ = new zu::function_declaration_node(LINE, $1, NULL, true, false, $4 ); }
         | type tIDENTIFIER  '(' arguments ')' '=' literal     {$$ = new zu::function_declaration_node(LINE, $1, $7, true, false, $4 ); }
         | type tIDENTIFIER '!' '(' arguments ')'              {$$ = new zu::function_declaration_node(LINE, $1, NULL, false, false, $5);}
         | type tIDENTIFIER '!' '(' arguments ')' '=' literal  {$$ = new zu::function_declaration_node(LINE, $1, $8, false, false, $5);}
         | type tIDENTIFIER '?' '(' arguments ')'              {$$ = new zu::function_declaration_node(LINE, $1, NULL, true, true, $5 );}
         | type tIDENTIFIER '?' '(' arguments ')' '=' literal  {$$ = new zu::function_declaration_node(LINE, $1, $8, true, true, $5 );}
         ;

   
/* Os argumentos de uma funçao: uma ou mais variaveis. */
arguments: assign_variable                                     {$$ = new cdk::sequence_node(LINE, $1);}
         | arguments ',' assign_variable                       {$$ = new cdk::sequence_node(LINE, $3 ,$1);}
         |                                                     {$$ = NULL;}
         ;


declare_variable : type tIDENTIFIER                     { $$ = new zu::declare_var_node(LINE,$1, $2, true, false, false);}
                 | type tIDENTIFIER '!'                 { $$ = new zu::declare_var_node(LINE,$1, $2, false, false, false);}             
                 | type tIDENTIFIER '?'                 { $$ = new zu::declare_var_node(LINE,$1, $2, true, true, false);}
                 ; 
      
assign_variable : declare_variable                      { $$ = $1; }
                | declare_variable '=' expr             { $$ = new zu::assignment_node(LINE, $1, $3 ); }
                ;
                
/* tipos primitivos
   tipo void? "!" */
type : '#'                                              { $$ = new basic_type(4, basic_type::TYPE_INT); }
     | tDOUBLE                                          { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
     | tSTRING                                          { $$ = new basic_type(4, basic_type::TYPE_STRING); }
     | '<' type '>'                                     { $$ = new basic_type(4, basic_type::TYPE_POINTER); }
     | '!'                                              { $$ = new basic_type(1, basic_type::TYPE_VOID); }
     ;
     
literal : tLINTEGER                                     { $$ = new cdk::integer_node(LINE, $1); }
        | string_literal                                { $$ = $1; std::cout << "New string read: " << $1->value() << std::endl; }
        | tLDOUBLE                                      { $$ = new cdk::double_node(LINE, $1); std::cout << "New double literal read: " << $1 << std::endl; }
        ;
        
string_literal : tLSTRING                               { $$ = new cdk::string_node(LINE, $1); }
               | string_literal tLSTRING                { $$ = new cdk::string_node(LINE, $1->value() + $2->c_str()); }
               ;

/*Falta:
    indexaçao?
    
*/
expr : literal                         { $$ = $1;}
     
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
     | expr '<' expr %prec tLEFT_PREC  { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr %prec tLEFT_PREC  { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	               { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr                   { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	               { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr	               { $$ = new cdk::eq_node(LINE, $1, $3); }
     
     | variable_expr                   { $$ = $1;}
     | '(' expr ')'                    { $$ = $2; }
     | lval '=' expr                   { $$ = new zu::assignment_node(LINE, $1, $3); }
     ;

exprs : expr                           { $$ = new cdk::sequence_node(LINE, $1);}
      | exprs ',' expr                 { $$ = new cdk::sequence_node(LINE, $3, $1); }
      |                                { $$ = NULL;}
      ;
     
variable_expr : lval                            { $$ = new zu::rvalue_node(LINE, $1); }      
              | func_call                       { $$ = $1;}
              ;
     
func_call : tIDENTIFIER '(' exprs ')'  { $$ = new zu::function_call_node(LINE, $1, $3);}
          ;
     
lval : tIDENTIFIER                              { $$ = new zu::lvalue_node(LINE, $1); }
     | expr '[' expr ']'                        { $$ = new zu::index_node(LINE, $1, $3); /* o que por no 2º parametro? */}
     ;
     

%%
