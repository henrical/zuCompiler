// $Id: function_call_node.h,v 1.3 2016/05/18 20:32:57 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_FUNCTION_CALLNODE_H__
#define __ZU_FUNCTION_CALLNODE_H__

namespace zu {

  /**
   * Class for describing program nodes.
   */
  class function_call_node: public cdk::expression_node {
    std::string *_functionIdentifier; //usar identifier_node?
    cdk::sequence_node *_arguments;

  public:
    inline function_call_node(int lineno, std::string *f_identifier, cdk::sequence_node *arguments)
    :cdk::expression_node(lineno), _functionIdentifier(f_identifier), _arguments(arguments)
    {
//         std::cout << "FUNCTION_CALL node." << std::endl;
    };
    
    inline std::string functionIdentifier()
    {
        return *_functionIdentifier;
    }
    
    inline cdk::sequence_node* arguments()
    {
        return _arguments;
    }
        
  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_call_node(this, level);
    }

  };

} // zu

#endif
 
