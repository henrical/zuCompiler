 
// $Id: continue_node.h,v 1.2 2016/04/14 17:17:19 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_CONTINUENODE_H__
#define __ZU_CONTINUENODE_H__


namespace zu {

  /**
   * Class for describing read nodes.
   */
  class continue_node: public cdk::basic_node {

  public:
    inline continue_node(int lineno) :
        cdk::basic_node(lineno){
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_continue_node(this, level);
    }

  };

} // zu

#endif