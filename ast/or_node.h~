// $Id: program_node.h,v 1.1 2016/02/19 19:29:48 david Exp $ -*- c++ -*-
#ifndef __ZU_ORNODE_H__
#define __ZU_ORNODE_H__

#include <cdk/ast/binary_expression_node.h>

namespace zu {

  class or_node: public cdk::binary_expression_node {
    

  public:
    inline or_node(int lineno, cdk::expression_node * left,  cdk::expression_node * right ) :
        cdk::binary_expression_node(lineno, left, right){
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_or_node(this, level);
    }

  };

} // zu

#endif
