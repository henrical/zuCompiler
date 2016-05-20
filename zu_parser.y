%{
// $Id: zu_parser.y,v 1.15 2016/05/19 19:55:10 ist175838 Exp $
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

%token <i> tLINTEGER 
%token <s> tIDENTIFIER tLSTRING  
%token <d> tLDOUBLE
%token tWHILE tIF tPRINT tREAD tBEGIN tEND tSTRING tIDENTIFIER tCONTINUE tBREAK tRETURN tNLPRINT

%right '='
%left tGE tLE tEQ tNE 
%left '+' '-'
%left '&' '|'
%left '*' '/' '%'
%left '<' '>'
/* %left ' */
%nonassoc tLOWEST_ASSOC
%nonassoc '[' ']'
%nonassoc '{' '}'
/* %nonassoc tIDENTIFIER */
/* %nonassoc tEMPTY */
%nonassoc tUNARY 
/* %nonassoc tEXPR */
%nonassoc tLINTEGER tLSTRING tLDOUBLE
%nonassoc ','
%nonassoc '!' '?'
%nonassoc ':' ';'
%nonassoc tHIGHEST_ASSOC
%left tLEFT_PREC
%right tRIGHT_PREC

%type <node> stmt assign_variable block instruction cond_instruction loop_instruction
%type <sequence> list variables exprs instructions
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

stmt : assign_variable ';'                  { $$ = $1;}
     | declare_function %prec tLOWEST_ASSOC { $$ = $1;}
     | declare_function block               { $$ = new zu::function_definition_node(LINE, $1, $2);} 
     ;


/*  
    Bloco.
    Termina sempre com return ("!!!"), ver análise semantica.
    A sequencia de declaraçoes (list) vem sempre antes da lista de instruçoes.
*/     
block : '{' list '}'                    { $$ = new zu::block_node(LINE, $2, NULL); }                       
      | '{' instructions'}'             { $$ = new zu::block_node(LINE, NULL, $2); } 
      | '{' '}'                         { $$ = new zu::block_node(LINE, NULL, NULL); }
      | '{' list instructions '}'       { $$ = new zu::block_node(LINE, $2, $3); }
      ;
    
/*  
    Sequencia de instruções.
*/
instructions: instruction               { $$ = new cdk::sequence_node(LINE, $1); }
            | instructions instruction  { $$ = new cdk::sequence_node(LINE, $2, $1); }
            ;

/*  
    Instruções.
*/
instruction : expr ';'                  { $$ = new zu::evaluation_node(LINE, $1);}
/*             | '@' '!'                   { $$ = new zu::print_node(LINE, new zu::read_node(LINE, true), false);} */
            | expr '!'                  { $$ = new zu::print_node(LINE, $1, false);}
/*             | '@' tNLPRINT              { $$ = new zu::print_node(LINE, new zu::print_node(LINE, true), false);} */
            | expr tNLPRINT             { $$ = new zu::print_node(LINE, new zu::read_node(LINE, true), true);}
            | tBREAK                    { $$ = new zu::break_node(LINE);}
            | tCONTINUE                 { $$ = new zu::continue_node(LINE);}
            | tRETURN                   { $$ = new zu::return_node(LINE);}
            | cond_instruction          { $$ = $1;}
            | loop_instruction          { $$ = $1;}
            | block                     { $$ = $1;}
            ;
            
/*  
    Instruções condicionais.
*/
cond_instruction : '[' expr ']' '#' instruction                  { $$ = new zu::if_else_node(LINE, $2, $5, NULL);}
                 | '[' expr ']' '?' instruction                  { $$ = new zu::if_else_node(LINE, $2, $5, NULL);}
                 | '[' expr ']' '?' instruction ':' instruction  { $$ = new zu::if_else_node(LINE, $2, $5, $7); }
                 ;
 
/*
   Instruções de ciclo.

   Fica com aspecto ridiculo, mas nao conseguia resolver conflito reduce/reduce sem ser assim.
   É possivel fazer isto com duas regras, adicionando uma regra vazia ao não-terminal 'exprs', mas aparece o conflito.
*/
loop_instruction : '[' exprs     ';'       ';'       ']' instruction        { $$ = new zu::for_node(LINE, $2, NULL, NULL, $6);}
                 | '['           ';' exprs ';'       ']' instruction        { $$ = new zu::for_node(LINE, NULL, $3, NULL, $6);}
                 | '['           ';'       ';' exprs ']' instruction        { $$ = new zu::for_node(LINE, NULL , NULL, $4, $6);}
                 | '[' exprs     ';' exprs ';'       ']' instruction        { $$ = new zu::for_node(LINE, $2, $4, NULL, $7);}
                 | '['           ';' exprs ';' exprs ']' instruction        { $$ = new zu::for_node(LINE, NULL, $3, $5 , $7);}
                 | '[' exprs     ';'       ';' exprs ']' instruction        { $$ = new zu::for_node(LINE, $2, NULL, $5, $7);}
                 | '[' exprs     ';' exprs ';' exprs ']' instruction        { $$ = new zu::for_node(LINE, $2, $4, $6 , $8);}
                 | '['           ';'       ';'       ']' instruction        { $$ = new zu::for_node(LINE, NULL, NULL, NULL, $5);}
                 | '[' variables ';'       ';'       ']' instruction        { $$ = new zu::for_node(LINE, $2, NULL, NULL, $6);}
                 | '[' variables ';' exprs ';'       ']' instruction        { $$ = new zu::for_node(LINE, $2, $4, NULL, $7);}
                 | '[' variables ';'       ';' exprs ']' instruction        { $$ = new zu::for_node(LINE, $2, NULL, $5, $7);}
                 | '[' variables ';' exprs ';' exprs ']' instruction        { $$ = new zu::for_node(LINE, $2, $4, $6, $8);}
                 ; 

/* 
   Declaraçao de funçoes.
   
   id_func!() -> funçao global 
   id_func?() -> funcao definida noutro modulo
   
   !id_func() -> retorna void
*/
declare_function 
: type tIDENTIFIER  '(' variables ')'                 {$$ = new zu::function_declaration_node(LINE, $2, $1, NULL, true, false, $4 ); }
| type tIDENTIFIER  '(' variables ')' '=' literal     {$$ = new zu::function_declaration_node(LINE, $2, $1, $7, true, false, $4 ); }
| type tIDENTIFIER '!' '(' variables ')'              {$$ = new zu::function_declaration_node(LINE, $2, $1, NULL, false, false, $5);}
| type tIDENTIFIER '!' '(' variables ')' '=' literal  {$$ = new zu::function_declaration_node(LINE, $2, $1, $8, false, false,$5);}
| type tIDENTIFIER '?' '(' variables ')'              {$$ = new zu::function_declaration_node(LINE, $2, $1, NULL, true, true, $5 );}
| type tIDENTIFIER '?' '(' variables')' '=' literal   {$$ = new zu::function_declaration_node(LINE, $2, $1, $8, true, true, $5 );}
;

   
/* 
   Os argumentos de uma funçao, por exemplo: uma ou mais variaveis. 
   Tambem usada na parte das inicializaçoes de um ciclo for.
*/
variables : assign_variable                                     {$$ = new cdk::sequence_node(LINE, $1);}
          | variables ',' assign_variable                       {$$ = new cdk::sequence_node(LINE, $3 ,$1);}
          | %prec tLOWEST_ASSOC                                 {$$ = NULL;}
          ;

/*
   Declaraçao de variaveis. 
   ! -> variavel global.
   ? -> variavel definida noutro módulo
*/
declare_variable : type tIDENTIFIER                     { $$ = new zu::declare_var_node(LINE,$1, $2, true, false, false);}
                 | type tIDENTIFIER '!'                 { $$ = new zu::declare_var_node(LINE,$1, $2, false, false, false);}             
                 | type tIDENTIFIER '?'                 { $$ = new zu::declare_var_node(LINE,$1, $2, true, true, false);}
                 ; 
      
/*
   Inicializaçao de variaveis. 
*/    
assign_variable : declare_variable                      { $$ = $1; }
                | declare_variable '=' expr             { $$ = new zu::assignment_node(LINE, $1, $3/*, false*/); }
                ;
                
/* 
   Tipos primitivos.
   # -> inteiro
   $ -> cadeia de caracteres
   % -> numero virgual flutuante (8 bytes => double)
   <#>, <$>, <%> -> ponteiro para inteiro, etc..
   ! -> void
*/
type : '#'                                              { $$ = new basic_type(4, basic_type::TYPE_INT); }
     | '%'                                              { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
     | tSTRING                                          { $$ = new basic_type(4, basic_type::TYPE_STRING); }
     | '<' type '>'                                     { $$ = new basic_type(4, basic_type::TYPE_POINTER); }
     | '!'                                              { $$ = new basic_type(0, basic_type::TYPE_VOID); }
     ;
 
 
/*
    Literais:
    inteiros 
    doubles: 1.2, .2, 1.1e3, 10e12, etc...
    cadeias de caracteres: "uma string"
    Concatenaçao: 
        $str1 = "uma string"; 
        $str2 = "uma" " string";
        str1 == str2 devolve true
*/
literal : tLINTEGER                                    { $$ = new cdk::integer_node(LINE, $1); }
        | string_literal %prec tLOWEST_ASSOC           { $$ = $1; }
        | tLDOUBLE                                     { $$ = new cdk::double_node(LINE, $1);}
        ;
        
string_literal : tLSTRING                              { $$ = new cdk::string_node(LINE, $1); }
               | string_literal tLSTRING               { $$ = new cdk::string_node(LINE, $1->value() + $2->c_str()); }
               ;

/*
    Expressoes.
    
    Falta:  
        reserva de memoria: i.e "<#>int_pointer = [5]"
                            aloca 5 inteiros
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
     | expr '%' expr	               { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '<' expr                   { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr                   { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	               { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr                   { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	               { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr	               { $$ = new cdk::eq_node(LINE, $1, $3); }
     
     | variable_expr                   { $$ = $1;}
     | '(' expr ')'                    { $$ = $2; }
     | lval '=' expr                   { $$ = new zu::assignment_node(LINE, $1, $3/*, false*/); }
     | '@'                             { $$ = new zu::read_node(LINE,false);}
/*      | '[' expr ']'                    { memory reservation node} */
     ;

/*
    Sequencia de expressoes.
*/
exprs : expr                           { $$ = new cdk::sequence_node(LINE, $1);}
      | exprs ',' expr                 { $$ = new cdk::sequence_node(LINE, $3, $1); }
      ;
     
/*  
    Expressões com variaveis.
*/
variable_expr : lval                   { $$ = new zu::rvalue_node(LINE, $1); }      
              | func_call              { $$ = $1;}
              ;
     
/*
    Chamada de funçoes, com ou sem argumentos.
*/
func_call : tIDENTIFIER '(' exprs ')'  { $$ = new zu::function_call_node(LINE, $1, $3);}
          | tIDENTIFIER '(' ')'        { $$ = new zu::function_call_node(LINE, $1, NULL);}
          ;
     
/*
    Left values.
    i.e variable_id, array_id[3], etc...
*/
lval : tIDENTIFIER                     { $$ = new zu::lvalue_node(LINE, $1); }
     | expr '[' expr ']'               { $$ = new zu::index_node(LINE, $1, $3);} //CAUSING SEG FAULT
     ;
     

%%
