// $Id: function_declaration_node.h,v 1.2 2016/04/06 16:20:27 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_FUNCTION_DECLARATIONNODE_H__
#define __ZU_FUNCTION_DECLARATIONNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>

namespace zu {

  /**
   * Class for describing function declaration nodes.
   */
  class function_declaration_node: public cdk::basic_node {
    basic_type *_type;
    cdk::expression_node *_literal; // A function literal is an expression that represents the default return value of the function.
    cdk::sequence_node *_arguments;
    
    bool _isLocal;
    bool _isImported;
    
      
  public:
    inline function_declaration_node(int lineno, basic_type * type, cdk::expression_node *literal, bool is_local, bool is_imported, cdk::sequence_node *arguments) :
        cdk::basic_node(lineno), _type(type), _literal(literal), _arguments(arguments), _isLocal(is_local), _isImported(is_imported)
    {   
//        std::cout << "FUNCTION_DECLARATION node." << std::endl; 
    }

  public:
    inline basic_type* type()
    {
        return _type;
    }
    
    inline void type(basic_type * type)
    {
        _type = type;
    }
    
    inline cdk::expression_node* literal()
    {
        return _literal;
    }
    
    inline cdk::sequence_node* arguments()
    {
        return _arguments;
    }
    
    inline bool isLocal()
    {
        return _isLocal;
    }
    
    inline bool isImported()
    {
        return _isImported;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_declaration_node(this, level);
    }

  };

} // zu

#endif
