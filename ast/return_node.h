// $Id: return_node.h,v 1.1 2016/03/17 15:24:46 ist173132 Exp $ -*- c++ -*-
#ifndef __ZU_RETURNNODE_H__
#define __ZU_RETURNNODE_H__

#include <cdk/ast/basic_node.h>

namespace zu {

  /**
   * Class for describing program nodes.
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
 
