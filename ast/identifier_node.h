// $Id: identifier_node.h,v 1.2 2016/05/18 20:32:57 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_IDENTIFIERNODE_H__
#define __ZU_IDENTIFIERNODE_H__

#include <cdk/ast/basic_node.h>
#include "ast/lvalue_node.h"

namespace zu {


  class identifier_node: public zu::lvalue_node {

  public:
    inline identifier_node(int lineno, std::string &value) :
        zu::lvalue_node(lineno,value)
    {
//         std::cout << "IDENTIFIER node." << std::endl;
    }
    
    inline identifier_node(int lineno, std::string *value) :
        zu::lvalue_node(lineno,*value)
    {
//         std::cout << "IDENTIFIER node." << std::endl;
    }
    
    inline identifier_node(int lineno, const char *value) :
        zu::lvalue_node(lineno,value)
    {
//         std::cout << "IDENTIFIER node." << std::endl;
    }
    
    

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identifier_node(this, level);
    }

  };

} // zu

#endif
