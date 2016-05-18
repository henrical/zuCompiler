 
// $Id: continue_node.h,v 1.3 2016/05/18 20:32:57 ist175838 Exp $ -*- c++ -*-
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
//             std::cout << "CONTINUE node." << std::endl;
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_continue_node(this, level);
    }

  };

} // zu

#endif