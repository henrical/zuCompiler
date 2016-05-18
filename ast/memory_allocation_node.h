// $Id: memory_allocation_node.h,v 1.2 2016/03/18 12:35:47 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_MEMORY_ALLOCATIONNODE_H__
#define __ZU_MEMORY_ALLOCATIONNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace zu {

  class memory_allocation_node: public cdk::unary_expression_node {
    

  public:
    inline memory_allocation_node(int lineno, cdk::expression_node *argument) :
        cdk::unary_expression_node(lineno, argument)
    {
//         std::cout << "MEMORY_ALLOCATION node." << std::endl;
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_memory_allocation_node(this, level);
    }

  };

} // zu

#endif
