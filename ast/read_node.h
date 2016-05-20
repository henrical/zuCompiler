// $Id: read_node.h,v 1.3 2016/05/18 20:32:57 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_READNODE_H__
#define __ZU_READNODE_H__

#include "ast/lvalue_node.h"

namespace zu {

  /**
   * Class for describing read nodes.
   */
  class read_node: public cdk::expression_node {
      
      bool _printed;
      
  public:
    inline read_node(int lineno, bool printed) :
        cdk::expression_node(lineno){
//             std::cout << "READ node." << std::endl;
        _printed = printed;
    }

  public:
      
    inline void printed(bool value)
    {
        _printed = value;
    }
      
    inline bool printed()
    {
        return _printed;
    }  
      
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_read_node(this, level);
    }

  };

} // zu

#endif
