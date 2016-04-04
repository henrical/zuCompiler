// $Id: variable_node.h,v 1.2 2016/03/18 12:59:55 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_VARIABLENODE_H__
#define __ZU_VARIABLENODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>
#include <cdk/basic_type.h>
#include <ast/lvalue_node.h>

namespace zu {

    class variable_node: public zu::lvalue_node {
      bool _global;

    public:
        inline variable_node(int lineno, std::string * identifier, bool global = false) :
	lvalue_node(lineno,identifier), _global(global)
        { }

    public:
    inline bool isGlobal(){
      return _global;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_node(this, level);
    }

  };

} // zu

#endif

