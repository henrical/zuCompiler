// $Id: stack_counter.cpp,v 1.17 2016/05/19 12:27:59 ist175838 Exp $ -*- c++ -*-
#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/stack_counter.h"
#include "ast/all.h"  // all.h is automatically generated

#define DEBUG 0

void debugPrint(std::string msg)
{
    if(DEBUG == 1)
      std::cout << "[Stack counter] " << msg << std::endl;  
}

//---------------------------------------------------------------------------
//     THIS IS THE VISITOR'S DEFINITION
//---------------------------------------------------------------------------

void zu::stack_counter::do_sequence_node(cdk::sequence_node * const node, int lvl) 
{
    for (size_t i = 0; i < node->size(); i++)
        node->node(i)->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void zu::stack_counter::do_integer_node(cdk::integer_node * const node, int lvl) {
}

void zu::stack_counter::do_string_node(cdk::string_node * const node, int lvl) {

}

//---------------------------------------------------------------------------

void zu::stack_counter::do_neg_node(cdk::neg_node * const node, int lvl) {
}

//---------------------------------------------------------------------------

void zu::stack_counter::do_add_node(cdk::add_node * const node, int lvl) {
}
void zu::stack_counter::do_sub_node(cdk::sub_node * const node, int lvl) {
}
void zu::stack_counter::do_mul_node(cdk::mul_node * const node, int lvl) {
}
void zu::stack_counter::do_div_node(cdk::div_node * const node, int lvl) {
}
void zu::stack_counter::do_mod_node(cdk::mod_node * const node, int lvl) {
}
void zu::stack_counter::do_lt_node(cdk::lt_node * const node, int lvl) {
}
void zu::stack_counter::do_le_node(cdk::le_node * const node, int lvl) {
}
void zu::stack_counter::do_ge_node(cdk::ge_node * const node, int lvl) {
}
void zu::stack_counter::do_gt_node(cdk::gt_node * const node, int lvl) {
}
void zu::stack_counter::do_ne_node(cdk::ne_node * const node, int lvl) {
}
void zu::stack_counter::do_eq_node(cdk::eq_node * const node, int lvl) {
}

//---------------------------------------------------------------------------

void zu::stack_counter::do_rvalue_node(zu::rvalue_node * const node, int lvl) {
}

//---------------------------------------------------------------------------

void zu::stack_counter::do_lvalue_node(zu::lvalue_node * const node, int lvl) {
}

//---------------------------------------------------------------------------

void zu::stack_counter::do_assignment_node(zu::assignment_node * const node, int lvl) 
{
    node->lvalue()->accept(this, lvl +2);
}
//---------------------------------------------------------------------------

void zu::stack_counter::do_evaluation_node(zu::evaluation_node * const node, int lvl) {
}

void zu::stack_counter::do_print_node(zu::print_node * const node, int lvl) {
}

//---------------------------------------------------------------------------

void zu::stack_counter::do_if_else_node(zu::if_else_node * const node, int lvl) {
    if(node->thenblock() != NULL)
    {
        debugPrint("Going into \"then\" block inside \"if/else\" statement...");
        node->thenblock()->accept(this, lvl + 4);
    }
    else
        debugPrint("Skipping empty \"then\" block inside \"if/else\" statement...");
        
    if(node->elseblock() != NULL)
    {
        debugPrint("Going into \"else\" block inside \"if/else\" statement...");
        node->elseblock()->accept(this, lvl + 4);
    } 
    else
        debugPrint("Skipping empty \"else\" block inside \"if/else\" statement...");
    
}

//---------------------------------------------------------------------------

void zu::stack_counter::do_read_node(zu::read_node * const node, int lvl) {
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_for_node(zu::for_node * const node, int lvl)
{
    //do initialization part of "for" loop header...
    if(node->init()!= NULL)
    {
        debugPrint("Going into initialization segment of \"for\" loop...");
        node->init()->accept(this, lvl + 4);
    }
    else
        debugPrint("Skipping empty \"initialization\" segment of \"for\" loop...");
  
    //compute stack size inside instructions block of "for" loop
    if(node->instructions()!= NULL)
    {
        debugPrint("Going into instructions segment of \"for\" loop...");
        node->instructions()->accept(this, lvl + 4);
    }
     else
        debugPrint("Skipping empty \"instructions\" segment of \"for\" loop...");
    
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_continue_node(zu::continue_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_block_node(zu::block_node * const node, int lvl) {
    if(node->declarations()!= NULL)
    {
        debugPrint("Going into declarations segment...");
        node->declarations()->accept(this, lvl + 2);
        
    }
    if(node->instructions()!= NULL)
    {
        debugPrint("Going into instructions segment...");
        node->instructions()->accept(this,lvl + 2);
        
    }
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_break_node(zu::break_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_return_node(zu::return_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_identifier_node(zu::identifier_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_index_node(zu::index_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_and_node(zu::and_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_or_node(zu::or_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_identity_node(zu::identity_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_symmetry_node(zu::symmetry_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_declare_var_node(zu::declare_var_node * const node, int lvl)
{
     _bytes += node->type()->size();
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_function_definition_node(zu::function_definition_node * const node, int lvl) {
    debugPrint("Going into function definition node...");
    node->block()->accept(this, lvl + 4);
    
    std::cout << "Counting allocated bytes inside function definition." << std::endl << "Result: " << _bytes + node->declaration()->type()->size() << "." << std::endl;
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_function_call_node(zu::function_call_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_memory_allocation_node(zu::memory_allocation_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::stack_counter::do_memory_address_node(zu::memory_address_node * const node, int lvl)
{
    //FIXME
}