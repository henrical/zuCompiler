// $Id: symbol.h,v 1.2 2016/05/18 20:32:57 ist175838 Exp $ -*- c++ -*-
#ifndef __ZU_SEMANTICS_SYMBOL_H__
#define __ZU_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>

namespace zu {

    enum SymbolType
    {
        LocalVariable,
        GlobalVariable,
        Function
    };
    
    class symbol {
      const basic_type *_type;
      std::string _name;
      long _value; // hack!
      
      bool _defined;
      bool _local;
      bool _import ;
      
      int _offset = -1;   //offset -1 é invalido, porque a memoria esta alinhada ao byte, logo os offsets sao 
                          //sempre multiplos de 4 (so negativos ou positivos)
                          //de qualquer maneira, o offset nao e relevante numa variavel global, porque é acedida
                          //com uma label.
      
      std::string _label;
      
      std::vector<basic_type*> _argumentsTypes;
      SymbolType _symType;
      

    public:
        //==================================================================================================
        //constructor variaveis locais
        inline symbol(basic_type *type, const std::string &name, bool defined, int offset) :
          _type(type),  _name(name)
        {  
            _defined = defined;
            _offset = offset;
            _local = true;
            _import = false; //Variaveis locais nao podem ser importadas.
            
            _symType = SymbolType::LocalVariable;
        } 
        //==================================================================================================
       
        //==================================================================================================
        //constructor variaveis globais
        inline symbol(basic_type *type, const std::string &name, bool defined,bool import, std::string label) :
          _type(type),  _name(name)
        {  
            _defined = defined;
            _label = label;
            _import = import;
            
            _local = false;
            
            _symType = SymbolType::GlobalVariable;
        } 
        //==================================================================================================
        
        //==================================================================================================
        //constructor funçoes
        inline symbol(const basic_type *type, const std::string &name,  bool defined,bool import, std::vector<basic_type*> args_type) :
            _type(type), _name(name), _defined(defined), _import(import)
        {
            _argumentsTypes = args_type;
            _symType = SymbolType::Function;
        }
        //==================================================================================================

        inline symbol(const basic_type *type, const std::string &name, long value, bool defined, bool local, bool import) :
            _type(type), _name(name), _value(value), _defined(defined), _local(local), _import(import)
        {
            
            _symType = SymbolType::Function;
        }
        
      virtual ~symbol() {
        delete _type;
      }

      inline const basic_type *type() const {
        return _type;
      }
      inline const std::string &name() const {
        return _name;
      }
      inline long value() const {
        return _value;
      }
      inline long value(long v) {
        return _value = v;
      }
      
      
      inline void offset(int offset)
      {
            _offset = offset;
      }
      
      inline int offset()
      {
            return _offset;
      }
      
      inline void defined(bool defined)
      {
            _defined = defined;
      }
      
      inline bool defined()
      {
            return _defined;
      }
    };

} // zu

#endif
