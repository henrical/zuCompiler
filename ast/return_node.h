// $Id: return_node.h,v 1.4 2016/04/14 17:17:19 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_RETURNNODE_H__
#define __ZU_RETURNNODE_H__

namespace zu {

  /**
   * Class for describing program nodes.
   * Nao estamos a usar isto. Remover?
   */
  class return_node: public cdk::basic_node {
    

  public:
    inline return_node(int lineno)
    :cdk::basic_node(lineno)
    {
    };
        
  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }

  };

} // zu

#endif
 
