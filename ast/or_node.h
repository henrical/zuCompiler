// $Id: or_node.h,v 1.2 2016/05/18 20:32:57 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_ORNODE_H__
#define __ZU_ORNODE_H__

#include <cdk/ast/binary_expression_node.h>

namespace zu {

  class or_node: public cdk::binary_expression_node {
    

  public:
    inline or_node(int lineno, cdk::expression_node * left,  cdk::expression_node * right ) :
        cdk::binary_expression_node(lineno, left, right){
//             std::cout << "OR node." << std::endl;
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_or_node(this, level);
    }

  };

} // zu

#endif
