 
// $Id: continue_node.h,v 1.1 2016/03/17 14:08:40 ist173132 Exp $ -*- c++ -*-
#ifndef __ZU_CONTINUENODE_H__
#define __ZU_CONTINUENODE_H__


namespace zu {

  /**
   * Class for describing read nodes.
   */
  class continue_node: public cdk::basic_node {
    int *_level;

  public:
    inline continue_node(int lineno, int *level) :
        cdk::basic_node(lineno), _level(level) {
    }

  public:
    inline int *level() {
      return _level;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_continue_node(this, level);
    }

  };

} // zu

#endif