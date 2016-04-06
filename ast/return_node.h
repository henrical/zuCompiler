// $Id: return_node.h,v 1.2 2016/03/17 22:46:08 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_RETURNNODE_H__
#define __ZU_RETURNNODE_H__

namespace zu {

  /**
   * Class for describing program nodes.
   * Nao estamos a usar isto. Remover?
   */
  class return_node: public cdk::expression_node {
    

  public:
    inline return_node(int lineno)
    :cdk::expression_node(lineno)
    {
    };
        
  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }

  };

} // zu

#endif
 
