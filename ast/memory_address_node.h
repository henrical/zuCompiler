// $Id: memory_address_node.h,v 1.2 2016/04/11 20:36:33 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_MEMORY_ADDRESSNODE_H__
#define __ZU_MEMORY_ADDRESSNODE_H__

namespace zu {

  /**
   * O operador unario '?' e usado para aceder ao endereço de uma expressao.
   */
  class memory_address_node: public cdk::unary_expression_node {

  public:
    inline memory_address_node(int lineno, cdk::expression_node *arg)
    :cdk::unary_expression_node(lineno, arg)
    {
    };
        
  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_memory_address_node(this, level);
    }

  };

} // zu

#endif
 
