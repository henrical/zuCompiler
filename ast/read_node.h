// $Id: read_node.h,v 1.1 2016/02/19 19:29:48 david Exp $ -*- c++ -*-
#ifndef __ZU_READNODE_H__
#define __ZU_READNODE_H__

#include "ast/lvalue_node.h"

namespace zu {

  /**
   * Class for describing read nodes.
   */
  class read_node: public cdk::basic_node {

  public:
    inline read_node(int lineno, lvalue_node *argument) :
        cdk::basic_node(lineno) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_read_node(this, level);
    }

  };

} // zu

#endif
