// $Id: postfix_writer.cpp,v 1.20 2016/05/20 12:41:49 ist175838 Exp $ -*- c++ -*-
#include <string>
#include <sstream>
#include <cstring>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/stack_counter.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------
//     THIS IS THE VISITOR'S DEFINITION
//---------------------------------------------------------------------------

void zu::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  _pf.INT(node->value()); // push an integer
}

void zu::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  
  //incrementar _lbl -> atribuir novo valor a lbl1
  //geral label adicionando "_L" como prefixo, usando a funçao mklbl
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); 
  
  _pf.STR(node->value()); // output string characters

  /* leave the address on the stack */
  _pf.TEXT(); // return to the TEXT segment
  _pf.ADDR(mklbl(lbl1)); // the string to be printed
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.ADD();
}
void zu::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.SUB();
}
void zu::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MUL();
}
void zu::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.DIV();
}
void zu::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void zu::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void zu::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void zu::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void zu::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void zu::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void zu::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_rvalue_node(zu::rvalue_node * const node, int lvl) {
    CHECK_TYPES(_compiler, _symtab, node);
    node->lvalue()->accept(this, lvl);
  
    if(node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE)
    {
        //Load 8 bytes;
        _pf.DLOAD();
    }
    else
        //Load 4 bytes.
        _pf.LOAD();
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_lvalue_node(zu::lvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  // simplified generation: all variables are global 
  // FIXME
  // verificar find_local
  // se estiver, usar LOCAL
  // se nao estiver no find_local, procurar find
  // se estiver em find 
  // |
  // |
  // v
  _pf.ADDR(node->value());
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_assignment_node(zu::assignment_node * const node, int lvl) {
    CHECK_TYPES(_compiler, _symtab, node);

    // DAVID: horrible hack!
    // (this is caused by Zu not having explicit variable declarations)
    const std::string &id = node->lvalue()->value();
    std::shared_ptr<zu::symbol> symbol = _symtab.find(id);
 
    _pf.DATA(); // variables are all global and live in DATA
    _pf.ALIGN(); // make sure we are aligned
    _pf.LABEL(id); // name variable location
    
    if(node->type()->name() == basic_type::TYPE_INT )
    {
        _pf.CONST(0); // initialize it to 0 (zero)
    }
    else if(node->type()->name() == basic_type::TYPE_STRING )
    {   
        //FIXME
    }
    else if(node->type()->name() == basic_type::TYPE_DOUBLE )
    {
        //FIXME
    }
        
    _pf.TEXT(); // return to the TEXT segment

    node->rvalue()->accept(this, lvl); // determine the new value
    _pf.DUP();
    node->lvalue()->accept(this, lvl); // where to store the value
    _pf.STORE(); // store the value at address
}
//---------------------------------------------------------------------------

void zu::postfix_writer::do_evaluation_node(zu::evaluation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.TRASH(4); // delete the evaluated value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.TRASH(4); // delete the evaluated value's address
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void zu::postfix_writer::do_print_node(zu::print_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  
//   std::cout << "constructing print node!" << std::endl;
  
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    addImport("printi");
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    addImport("prints");
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    addImport("printd");
    _pf.CALL("printd");
    _pf.TRASH(8); // delete the printed value's address
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }

  if(node->newline())
  {    
    addImport("println");
    _pf.CALL("println"); // print a newline
  }
      
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_if_else_node(zu::if_else_node * const node, int lvl) {
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_read_node(zu::read_node * const node, int lvl) {
   CHECK_TYPES(_compiler, _symtab, node);
    if(node->type()->name() == basic_type::TYPE_INT){
        addImport("readi"); //add import to be added as EXTERN after main function
        _pf.CALL("readi");
        _pf.PUSH();
    }
    else{
        addImport("readd"); //add import to be added as EXTERN after main function
        _pf.CALL("readd");
        _pf.DPUSH();
  }
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_for_node(zu::for_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_continue_node(zu::continue_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_block_node(zu::block_node * const node, int lvl) {
    _symtab.push();
    if(node->declarations() != NULL){
        node->declarations()->accept(this,lvl + 1);
    }
    if(node->instructions() != NULL){
        node->instructions()->accept(this, lvl +1);
    }
    _symtab.pop();
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_break_node(zu::break_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_return_node(zu::return_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_identifier_node(zu::identifier_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_index_node(zu::index_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_and_node(zu::and_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_or_node(zu::or_node * const node, int lvl) {
    CHECK_TYPES(_compiler, _symtab, node);
    int lbl1 = ++_lbl;
    node->left()->accept(this,lvl);
    _pf.DUP();
    _pf.JNZ(mklbl(lbl1));
    node->right()->accept(this, lvl);
    _pf.OR();
    _pf.LABEL(mklbl(lbl1)); 
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_identity_node(zu::identity_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_symmetry_node(zu::symmetry_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_declare_var_node(zu::declare_var_node * const node, int lvl)
{
    //FIXME
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_function_definition_node(zu::function_definition_node * const node, int lvl) {
    
    std::string func_identifier = node->declaration()->identifier();
    std::string func_final_name;
    
    if(func_identifier == "zu")
    {
        func_final_name = "_main";
    }
    else if(func_identifier =="_main")
    {
        func_final_name = "zu";
    }
    else
    {
        func_final_name = func_identifier;
    }
    
    bool local_function = node->declaration()->isLocal();
    
    if(!local_function)
    {
        _pf.GLOBAL(func_final_name, _pf.FUNC());
    }

    _pf.LABEL(func_final_name);
    
    
    //compute size of local vars for ENTER instruction
    stack_counter *sc = new stack_counter(_compiler, _symtab);

    node->accept(sc, lvl + 2);
    size_t local_size = sc->getByteSize();
    
    size_t return_size = node->declaration()->type()->size();
    
    _pf.ENTER(local_size + return_size);
    
    
    if(node->declaration()->returnValue() != nullptr)
    {
//         if(node->declaration()->type()->name() == basic_type::TYPE_DOUBLE)
//         {
//             
//         }
//         else
//         {    
            node->declaration()->returnValue()->accept(this, lvl + 2);
            _pf.LOCA(-return_size);
//         }
            
    }
    
    node->block()->accept(this, lvl + 2);
    
    //Carrega valor de retorno
    _pf.LOCAL(-return_size);
    _pf.LOAD();
    
    if(node->declaration()->type()->name() == basic_type::TYPE_DOUBLE)
        //Faz POP do valor de retorno para o registo ST0 (registo de 64bits para double vals)
        _pf.DPOP(); 
    else
        //Faz POP do valor de retorno para o registo EAX
        _pf.POP();
        
    _pf.LEAVE();
    _pf.RET();
    
    if(func_final_name == "_main"){ //a way to ensure that only one function uses calls extern function
        externImports(); //extern all imports used
//         externUnDefSymbols(); //extern all symbols undefined 
    }
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_function_call_node(zu::function_call_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_memory_allocation_node(zu::memory_allocation_node * const node, int lvl) {
    //FIXME
}

//---------------------------------------------------------------------------
void zu::postfix_writer::do_memory_address_node(zu::memory_address_node * const node, int lvl)
{
    //FIXME
}