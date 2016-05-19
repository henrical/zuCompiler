// $Id: type_checker.cpp,v 1.17 2016/05/19 12:27:59 ist175838 Exp $ -*- c++ -*-
#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void zu::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void zu::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

//---------------------------------------------------------------------------

inline void zu::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in argument of unary expression");

  // in Zu, expressions are always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void zu::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

inline void zu::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in right argument of binary expression");

  // in Zu, expressions are always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void zu::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void zu::type_checker::do_rvalue_node(zu::rvalue_node * const node, int lvl) {
  node->lvalue()->accept(this, lvl);
  node->type(node->lvalue()->type());
}

//---------------------------------------------------------------------------

void zu::type_checker::do_lvalue_node(zu::lvalue_node * const node, int lvl) {
  const std::string &id = node->value();
  std::shared_ptr<zu::symbol> symbol = _symtab.find(id);
  if (symbol == nullptr) throw id + " undeclared";
  // hackish stuff...
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

//---------------------------------------------------------------------------

void zu::type_checker::do_assignment_node(zu::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;

  // DAVID: horrible hack!
  // (this is caused by Zu not having explicit variable declarations)
//   const std::string &id = node->lvalue()->value();
//   if (!_symtab.find(id)) 
//   {
//       int offset = -1;
//       bool defined = true;
//      _symtab.insert(id, std::make_shared<zu::symbol>(new basic_type(4, basic_type::TYPE_INT), id, defined, offset)); // put in the symbol table
//   }

    node->lvalue()->accept(this, lvl + 2);
    node->rvalue()->accept(this, lvl + 2);
    if (node->lvalue()->type()->name() != node->rvalue()->type()->name())
    {
        //Conversao implicita double para integer.
        if(node->lvalue()->type()->name() == basic_type::TYPE_INT 
                                    && 
           node->rvalue()->type()->name() == basic_type::TYPE_DOUBLE)
        {
            //converter valor a ser atribuido para inteiro.
            std::cout << "Conversao double para int." << std::endl;
            node->rvalue()->type(new basic_type(4, basic_type::TYPE_INT));     
        }
        else
            throw std::string("Mismatched types in assignment expression of variable \"" + node->lvalue()->value() + "\": cannot implicitely convert rvalue to type of lvalue." );
//             throw std::string("Mismatched types in assignment expression of variable \"" + node->lvalue()->value() + "\": cannot implicitely convert rvalue of type \"" + node->rvalue()->type()->name() + "\" to \"" + node->lvalue()->type()->name() + "\"." );
    }
    
    
  // in Zu, expressions are always int
  node->type(node->lvalue()->type());
}

//---------------------------------------------------------------------------

void zu::type_checker::do_evaluation_node(zu::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void zu::type_checker::do_print_node(zu::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void zu::type_checker::do_read_node(zu::read_node * const node, int lvl) {
    //FIXME: o readnode esta a correto?
    //     ASSERT_UNSPEC;
    node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------
void zu::type_checker::do_for_node(zu::for_node * const node, int lvl) {
    node->init()->accept(this, lvl + 2);
    node->condition()->accept(this, lvl + 2);
    node->increment()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------
void zu::type_checker::do_if_else_node(zu::if_else_node * const node, int lvl) {
    node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------
void zu::type_checker::do_continue_node(zu::continue_node * const node, int lvl) {
    //do nothing...
}

//---------------------------------------------------------------------------

void zu::type_checker::do_block_node(zu::block_node * const node, int lvl) {
    node->declarations()->accept(this, lvl + 4);
    node->instructions()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void zu::type_checker::do_break_node(zu::break_node * const node, int lvl) {
    //do nothing...
}

//---------------------------------------------------------------------------

void zu::type_checker::do_return_node(zu::return_node * const node, int lvl) {
    //do nothing...
}

//---------------------------------------------------------------------------

void zu::type_checker::do_identifier_node(zu::identifier_node * const node, int lvl) {
    ASSERT_UNSPEC;
    
    const std::string &id = node->value();
    
    std::shared_ptr<zu::symbol> symbol = _symtab.find(id);
    
    if (symbol == nullptr) 
        throw id + " undeclared";
    
    const basic_type* const_type = symbol->type();
    basic_type type = *const_type;
    
    node->type(&type);
}

//---------------------------------------------------------------------------

void zu::type_checker::do_index_node(zu::index_node * const node, int lvl) {
    //FIXME: corrigir index_node (yacc, etc..)
    //FIXME: implementar este visitor
}

//---------------------------------------------------------------------------

void zu::type_checker::do_and_node(zu::and_node * const node, int lvl) {
    processBinaryExpression(node, lvl + 2);
}

//---------------------------------------------------------------------------

void zu::type_checker::do_or_node(zu::or_node * const node, int lvl) {
    processBinaryExpression(node, lvl + 2);
}

//---------------------------------------------------------------------------

void zu::type_checker::do_identity_node(zu::identity_node * const node, int lvl) {
    processUnaryExpression(node, lvl + 2);
}

//---------------------------------------------------------------------------

void zu::type_checker::do_symmetry_node(zu::symmetry_node * const node, int lvl) {
    processUnaryExpression(node, lvl + 2);
}


//---------------------------------------------------------------------------
void zu::type_checker::do_declare_var_node(zu::declare_var_node * const node, int lvl)
{  
   const std::string &id = node->value();
   bool local = node->isLocal();
   
   
    if(!local)
    { // GLOBAL VARIABLE 
        bool defined = false;
        bool imported = node->isImport();
        std::string label = ""; //atribuir label mais tarde
        _symtab.insert(id, std::make_shared<zu::symbol>(node->type(),
                                                        id,
                                                        defined,
                                                        imported,
                                                        label)
                      );
    }
    else
    { // LOCAL VARIABLE
        if(_symtab.find_local(node->value())!= nullptr){
            throw std::string("Error: \"" + node->value() + "\" already defined in this scope.");
        }
        
        bool defined = false;
        int offset = -1;
        _symtab.insert( id, std::make_shared<zu::symbol>(node->type(),
                                                         id,
                                                         defined,
                                                         offset)
                      );
    }
}

//---------------------------------------------------------------------------

void zu::type_checker::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) {
    if(node->type() == NULL){
        node->type(new basic_type(0,basic_type::TYPE_VOID));
    }
    
    std::string id = node->identifier();
    
    bool defined = false; //nao definida, por enquanto.
    bool imported = node->isImported();
    
//     cdk::sequence_node* arguments = node->arguments();
//     size_t num_args = arguments->size();
//     std::vector<basic_type*> arguments_type = std::vector<basic_type*>();
//     
//     std::vector<var
    
    //TODO: o sequence node que guarda os argumentos esta a guardar basic_nodes...
    //resolver isto no yacc para para poder fazer a verificação de tipos
    
    _symtab.insert(id, std::make_shared<zu::symbol>(node->type(),
                                                      id,
                                                      defined,
                                                      imported,
                                                      nullptr )//FIXME)
                  );

    

    
}

void zu::type_checker::do_function_definition_node(zu::function_definition_node * const node, int lvl) 
{    
    std::shared_ptr<zu::symbol> symbol = _symtab.find(node->declaration()->identifier());
    
    if(symbol->defined()){ //someone defined this already
        throw std::string("Error: multiple definitions for function \"" + node->declaration()->identifier() + "\".");
    }
    else
    {
        symbol->defined(true);
        
    }
}

//---------------------------------------------------------------------------
void zu::type_checker::do_function_call_node(zu::function_call_node * const node, int lvl) {
    std::shared_ptr<zu::symbol> symbol = _symtab.find(node->functionIdentifier());
    if (symbol == nullptr ) {
        throw std::string("Error: attempt to call inexistant function \"" + node->functionIdentifier());
    }
    
    //FIXME: verificar tipos dos argumentos
//       if(node->arguments() != NULL){
// 	variable_node * arg;
// 	for (size_t i = 0; i < node->arguments()->size(); i++) {
// 	  arg = (variable_node*) node->arguments()->node(i);
// 	  arg->type(getBasicType((symbol->getArgsTypes())[i]));
// 	}
//       } 

//   node->type(s->type());
}

//---------------------------------------------------------------------------
void zu::type_checker::do_memory_allocation_node(zu::memory_allocation_node * const node, int lvl) {
    //FIXME: unused node.
}

//---------------------------------------------------------------------------
void zu::type_checker::do_memory_address_node(zu::memory_address_node * const node, int lvl)
{
    processUnaryExpression(node, lvl + 4);
}