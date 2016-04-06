// $Id: function_definition_node.h,v 1.1 2016/03/17 22:22:33 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_FUNCTION_DEFINITIONNODE_H__
#define __ZU_FUNCTION_DEFINITIONNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>

namespace zu {

  /**
   * Class for describing function definition nodes.
   */
  class function_definition_node: public cdk::basic_node {
    function_declaration_node *_declaration;
    block_node *_block;                             //declaration nd instruction _block
    cdk::expression_node *_returnValue;
    
      
  public:
    inline function_definition_node(int lineno, function_declaration_node *declaration, block_node *block, cdk::expression_node *return_value) :
        cdk::basic_node(lineno), _declaration(declaration), _block(block), _returnValue(return_value)
    {
    }

  public:
    inline function_declaration_node* declaration()
    {
            return _declaration;
    }
    
    inline block_node* block()
    {
            return _block;
    }
    
    inline cdk::expression_node* returnValue()
    {
            return _returnValue;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // zu

#endif
