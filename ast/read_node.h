// $Id: read_node.h,v 1.2 2016/04/06 16:20:27 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_READNODE_H__
#define __ZU_READNODE_H__

#include "ast/lvalue_node.h"

namespace zu {

  /**
   * Class for describing read nodes.
   */
  class read_node: public cdk::basic_node {
    zu::lvalue_node *_argument;
      
  public:
    inline read_node(int lineno, lvalue_node *argument) :
        cdk::basic_node(lineno), _argument(argument) {
//             std::cout << "READ node." << std::endl;
    }

  public:
    inline zu::lvalue_node *argument()
    {
        return _argument;
    }
      
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_read_node(this, level);
    }

  };

} // zu

#endif