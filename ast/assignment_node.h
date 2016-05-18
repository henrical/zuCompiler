// $Id: assignment_node.h,v 1.2 2016/05/18 20:32:57 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_ASSIGNMENTNODE_H__
#define __ZU_ASSIGNMENTNODE_H__

#include "ast/lvalue_node.h"

namespace zu {

  /**
   * Class for describing assignment nodes.
   */
  class assignment_node: public cdk::expression_node {
    zu::lvalue_node *_lvalue;
    cdk::expression_node *_rvalue;  
    
//     bool _newVariable;

  public:
    inline assignment_node(int lineno, zu::lvalue_node *lvalue, cdk::expression_node *rvalue/*, bool new_var*/) :
        cdk::expression_node(lineno), _lvalue(lvalue), _rvalue(rvalue) 
    {
//             std::cout << "ASSIGNMENT node." << std::endl;
//         _newVariable = new_var;
    }

  public:
    inline zu::lvalue_node *lvalue() {
      return _lvalue;
    }
    inline cdk::expression_node *rvalue() {
      return _rvalue;
    }
    
//     inline bool newVariable()
//     {
//         return _newVariable;
//     }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_assignment_node(this, level);
    }

  };

} // zu

#endif
