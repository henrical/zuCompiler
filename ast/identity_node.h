// $Id: identity_node.h,v 1.1 2016/03/17 22:22:33 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_IDENTITYNODE_H__
#define __ZU_IDENTITYNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace zu {

  class identity_node: public cdk::unary_expression_node {

  public:
    inline identity_node(int lineno, cdk::expression_node *argument) :
        cdk::unary_expression_node(lineno, argument) {
    }
  
  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identity_node(this, level);
    }

  };

} // zu

#endif
