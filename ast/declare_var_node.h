// $Id: declare_var_node.h,v 1.2 2016/04/06 16:20:27 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_DECLAREVARNODE_H__
#define __ZU_DECLAREVARDNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>
#include <cdk/basic_type.h>
#include <ast/lvalue_node.h>

namespace zu {

class declare_var_node: public zu::variable_node  {
   bool _local;
   bool _import;
   bool _func_arg;
   bool _isConst;

  public:
    inline declare_var_node(int lineno, basic_type * type, std::string * identifier, bool local, bool import, bool func_arg, bool isConst) :
      zu::variable_node(lineno,identifier)
        ,_local(local),_import(import),_func_arg(func_arg),_isConst(isConst) {
	_type=type;
  }

  public:

     inline bool isLocal(){
      return _local;
    }
    inline bool isImport(){
      return _import;
    }
    inline bool isFuncArg(){
      return _func_arg;
    }
    
    inline bool isConst(){
      return _isConst;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_declare_var_node(this, level);
    }

  };

} // zu

#endif
