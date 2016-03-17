// $Id: program_node.h,v 1.2 2016/03/17 16:39:27 ist173132 Exp $ -*- c++ -*-
#ifndef __ZU_PROGRAMNODE_H__
#define __ZU_PROGRAMNODE_H__

#include <cdk/ast/basic_node.h>

namespace zu {

  /**
   * Class for describing program nodes.
	Eliminar
   */
  class program_node: public cdk::basic_node {
    cdk::basic_node *_statements;

  public:
    inline program_node(int lineno, cdk::basic_node *statements) :
        cdk::basic_node(lineno), _statements(statements) {
    }

  public:
    inline cdk::basic_node *statements() {
      return _statements;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_program_node(this, level);
    }

  };

} // zu

#endif
