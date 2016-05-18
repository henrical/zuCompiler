 
// $Id: break_node.h,v 1.3 2016/05/18 20:32:57 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_BREAKNODE_H__
#define __ZU_BREAKNODE_H__


namespace zu {

  /**
   * Class for describing read nodes.
   */
  class break_node: public cdk::basic_node {

  public:
    inline break_node(int lineno) :
        cdk::basic_node(lineno) {
//             std::cout << "BREAK node." << std::endl;
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_break_node(this, level);
    }

  };

} // zu

#endif