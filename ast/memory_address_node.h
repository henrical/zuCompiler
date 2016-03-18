// $Id: memory_address_node.h,v 1.2 2016/03/17 22:46:08 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_MEMORY_ADDRESSNODE_H__
#define __ZU_MEMORY_ADDRESSNODE_H__

namespace zu {

  /**
   * Class for describing program nodes.
   */
  class memory_address_node: public cdk::expression_node {
    lvalue_node *_lvalue;

  public:
    inline memory_address_node(int lineno, lvalue_node *lvalue)
    :cdk::expression_node(lineno), _lvalue(lvalue)
    {
    };
        
  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_memory_address_node(this, level);
    }

  };

} // zu

#endif
 
