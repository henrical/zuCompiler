// $Id: index_node.h,v 1.2 2016/03/17 16:01:24 ist173132 Exp $ -*- c++ -*-
#ifndef __ZU_INDEXNODE_H__
#define __ZU_INDEXNODE_H__

#include <cdk/ast/basic_node.h>

namespace zu {

  /**
   * Class for describing program nodes.
   */
  class index_node: public zu::lvalue_node {
    cdk::expression_node * _lval;
    cdk::expression_node * _rval;

  public:
    inline index_node(int lineno, std::string value, cdk::expression_node * lval, cdk::expression_node * rval  ) :
        zu::lvalue_node(lineno,value), _lval(lval), _rval(rval) { //FIXME
    }

  public:
    inline cdk::expression_node *lval() {
      return _lval;
    }
    
     inline cdk::expression_node *rval() {
      return _rval;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // zu

#endif