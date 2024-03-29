// $Id: lvalue_node.h,v 1.4 2016/05/18 20:32:57 ist175838 Exp $
#ifndef __ZU_NODE_EXPRESSION_LEFTVALUE_H__
#define __ZU_NODE_EXPRESSION_LEFTVALUE_H__

#include <cdk/ast/simple_value_node.h>
#include <string>

namespace zu {

  /**
   * Class for describing syntactic tree leaves for holding lvalues.
   */
  class lvalue_node: public cdk::simple_value_node<std::string> {
  
  public:
    inline lvalue_node(int lineno, const char *s) :
        cdk::simple_value_node<std::string>(lineno, s) {
//             std::cout << "LVALUE node." << std::endl;
    }
    inline lvalue_node(int lineno, const std::string &s) :
        cdk::simple_value_node<std::string>(lineno, s) {
            
//             std::cout << "LVALUE node." << std::endl;
    }
    inline lvalue_node(int lineno, const std::string *s) :
        cdk::simple_value_node<std::string>(lineno, *s) {
            
//             std::cout << "LVALUE node." << std::endl;
    }
    
    inline lvalue_node(int lineno) :
        cdk::simple_value_node<std::string>(lineno, nullptr) {
//             std::cout << "LVALUE node." << std::endl;
    }

  public:
    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    virtual void accept(basic_ast_visitor *sp, int level) {
      sp->do_lvalue_node(this, level);
    }

  };

} // zu

#endif
