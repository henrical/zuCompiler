// $Id: for_node.h,v 1.5 2016/05/19 14:22:54 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_FORNODE_H__
#define __ZU_FORNODE_H__

#include <cdk/ast/expression_node.h>

namespace zu {

  /**
   * Class for describing for-cycle nodes.
   */
  class for_node: public cdk::basic_node {
    
    cdk::sequence_node *_init; //a inicializacao da variavel de iteraçao (i = 0)  
    cdk::sequence_node *_condition; //a condiçao (i < 5)
    cdk::sequence_node *_increment; //o incremento (i++)
    cdk::basic_node *_instructions; //o bloco de instruçoes dentro do ciclo

  public:
    inline for_node(int lineno,cdk::sequence_node *init, cdk::sequence_node *condition, cdk::sequence_node *increment, cdk::basic_node *instructions) 
    :basic_node(lineno),_init(init), _condition(condition), _increment(increment), _instructions(instructions) 
    { 
//         std::cout << "FOR node." << std::endl;
    }

  public:
    inline cdk::sequence_node *init() {
      return _init;
    }  
      
    inline cdk::sequence_node *condition() {
      return _condition;
    }
    
    inline cdk::sequence_node *increment() {
      return _increment;
    }
    
    inline cdk::basic_node *instructions() {
      return _instructions;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_for_node(this, level);
    }

  };

} // zu

#endif
