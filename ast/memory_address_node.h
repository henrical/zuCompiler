// $Id: memory_address_node.h,v 1.3 2016/05/18 20:32:57 ist175838 Exp $ -*- c++ -*-
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
//         std::cout << "MEMORY_ADDRESS node." << std::endl;
    }
        
  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_memory_address_node(this, level);
    }

  };

} // zu

#endif
 
