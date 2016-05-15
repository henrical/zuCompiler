// $Id: for_node.h,v 1.2 2016/04/15 15:44:27 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_FORNODE_H__
#define __ZU_FORNODE_H__

#include <cdk/ast/expression_node.h>

namespace zu {

  /**
   * Class for describing for-cycle nodes.
   */
  class for_node: public cdk::basic_node {
    
    cdk::expression_node *_init; //a inicializacao da variavel de iteraçao (i = 0)  
    cdk::expression_node *_condition; //a condiçao (i < 5)
    cdk::expression_node *_increment; //o incremento (i++)
    cdk::basic_node *_instruction; //o bloco de instruçoes dentro do ciclo

  public:
    inline for_node(int lineno,cdk::expression_node *init, cdk::expression_node *condition, cdk::expression_node *increment, cdk::basic_node *instruction) 
    :basic_node(lineno),_init(init), _condition(condition), _increment(increment), _instruction(instruction) 
    { }

  public:
    inline cdk::expression_node *init() {
      return _init;
    }  
      
    inline cdk::expression_node *condition() {
      return _condition;
    }
    
    inline cdk::expression_node *increment() {
      return _increment;
    }
    
    inline cdk::basic_node *instruction() {
      return _instruction;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_for_node(this, level);
    }

  };

} // zu

#endif
