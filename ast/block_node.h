// $Id: block_node.h,v 1.1 2016/03/17 14:42:32 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_BLOCKNODE_H__
#define __ZU_BLOCKNODE_H__

#include <cdk/ast/sequence_node.h>

namespace zu {

  /**
   * Class for describing block nodes.
   * Blocks have a sequence of declarations(may be empty) and a sequence of instructions(may be empty).
   */
  class block_node: public cdk::basic_node {
    cdk::sequence_node* _declarations;
    cdk::sequence_node* _instructions;
    
  public:
    inline block_node(int lineno, cdk::sequence_node *declarations, cdk::sequence_node *instructions) :
        cdk::basic_node(lineno), _declarations(declarations), _instructions(instructions) 
    {
    }

  public:
    inline cdk::sequence_node *declarations() {
      return _declarations;
    }

    inline cdk::sequence_node *instructions() {
      return _instructions;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_block_node(this, level);
    }

  };

} // zu

#endif
