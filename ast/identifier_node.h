// $Id: identifier_node.h,v 1.2 2016/03/17 16:01:24 ist173132 Exp $ -*- c++ -*-
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
    }
    
    inline identifier_node(int lineno, std::string *value) :
        zu::lvalue_node(lineno,*value)
    {
    }
    
    inline identifier_node(int lineno, const char *value) :
        zu::lvalue_node(lineno,value)
    {
    }
    
    

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identifier_node(this, level);
    }

  };

} // zu

#endif
