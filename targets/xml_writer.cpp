// $Id: xml_writer.cpp,v 1.16 2016/05/18 20:32:57 ist175838 Exp $ -*- c++ -*-
#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void zu::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  processSimple(node, lvl);
}

void zu::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  processSimple(node, lvl);
}

//---------------------------------------------------------------------------

inline void zu::xml_writer::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

inline void zu::xml_writer::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_rvalue_node(zu::rvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_lvalue_node(zu::lvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  processSimple(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_assignment_node(zu::assignment_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  openTag("rvalue", lvl + 2);
  node->rvalue()->accept(this, lvl + 4);
  closeTag("rvalue", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_evaluation_node(zu::evaluation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_print_node(zu::print_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_read_node(zu::read_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_for_node(zu::for_node * const node, int lvl)
{
        //FIXME
}
void zu::xml_writer::do_if_else_node(zu::if_else_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

//-----------------------------------------------------------------------------
void zu::xml_writer::do_continue_node(zu::continue_node * const node, int lvl) {
    //FIXME
}

//-----------------------------------------------------------------------------

void zu::xml_writer::do_block_node(zu::block_node * const node, int lvl)
{
  openTag("block", lvl );
//   if(node->stmts()!= NULL){node->stmts()->accept(this, lvl + 2);}
//   if(node->instrs()!= NULL){node->instrs()->accept(this,lvl + 2);}
//   closeTag("block", lvl );
}

//------------------------------------------------------------------------------

void zu::xml_writer::do_break_node(zu::break_node * const node, int lvl) {
    //FIXME
}

//------------------------------------------------------------------------------
    
void zu::xml_writer::do_return_node(zu::return_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

//------------------------------------------------------------------------------

void zu::xml_writer::do_identifier_node(zu::identifier_node * const node, int lvl) {
    //FIXME
}

//------------------------------------------------------------------------------

void zu::xml_writer::do_index_node(zu::index_node * const node, int lvl) {
  openTag(node, lvl);
  
  openTag("lvalue_node",lvl+2);
  node->position()->accept(this, lvl + 4 );
  closeTag("lvalue_node",lvl+2);
  openTag("index",lvl+2);
  node->offset()->accept(this, lvl + 4);
  closeTag("index",lvl+2);
  closeTag(node, lvl);
}

//------------------------------------------------------------------------------

void zu::xml_writer::do_and_node(zu::and_node * const node, int lvl) {
  processBinaryExpression(node,lvl);
}

//------------------------------------------------------------------------------

void zu::xml_writer::do_or_node(zu::or_node * const node, int lvl) {
  processBinaryExpression(node,lvl);
}

//------------------------------------------------------------------------------

void zu::xml_writer::do_identity_node(zu::identity_node * const node, int lvl) {
  processUnaryExpression(node, lvl); 
}

//------------------------------------------------------------------------------

void zu::xml_writer::do_symmetry_node(zu::symmetry_node * const node, int lvl) {
    //FIXME
}


//---------------------------------------------------------------------------
void zu::xml_writer::do_declare_var_node(zu::declare_var_node * const node, int lvl)
{
    //FIXME
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::xml_writer::do_function_definition_node(zu::function_definition_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::xml_writer::do_function_call_node(zu::function_call_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::xml_writer::do_memory_allocation_node(zu::memory_allocation_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::xml_writer::do_memory_address_node(zu::memory_address_node * const node, int lvl)
{
    //FIXME
}
