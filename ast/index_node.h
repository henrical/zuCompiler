// $Id: index_node.h,v 1.5 2016/05/14 22:50:04 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_INDEXNODE_H__
#define __ZU_INDEXNODE_H__

#include <cdk/ast/basic_node.h>

namespace zu {


  class index_node: public zu::lvalue_node {
    cdk::expression_node * _position;
    cdk::expression_node * _offset;

  public:
    inline index_node(int lineno, cdk::expression_node * base_pos, cdk::expression_node * rval  ) :
        zu::lvalue_node(lineno), _position(base_pos), _offset(rval) {
    }
    
//     inline index_node(int lineno, std::string *value, cdk::expression_node * base_pos, cdk::expression_node * rval  ) :
//         zu::lvalue_node(lineno,*value), _position(base_pos), _offset(rval) {
//     }
//     
//     inline index_node(int lineno, const char *value, cdk::expression_node * base_pos, cdk::expression_node * rval  ) :
//         zu::lvalue_node(lineno,value), _position(base_pos), _offset(rval) {
//     }

  public:
    inline cdk::expression_node *position() {
      return _position;
    }
    
     inline cdk::expression_node *offset() {
      return _offset;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // zu

#endif
