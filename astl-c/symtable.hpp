#ifndef C_SYMTABLE_H
#define C_SYMTABLE_H

#include <cassert>
#include <string>
#include <map>
#include "symbol.hpp"
#include "scope.hpp"

namespace AstlC {

   class SymTable {
      private:
	 Scope* scope;

      public:
	 // constructors
	 SymTable() : scope(0) {
	 }

	 // accessors
	 bool lookup(const std::string name, Symbol& symbol) const {
	    if (scope) {
	       return scope->lookup(name, symbol);
	    } else {
	       return false;
	    }
	 }

	 // mutators
	 void open() {
	    Scope* inner = new Scope(scope);
	    scope = inner;
	 }
	 void close() {
	    assert(scope);
	    Scope* outer = scope->get_outer();
	    delete scope;
	    scope = outer;
	 }
	 bool insert(const Symbol& symbol) {
	    assert(scope);
	    return scope->insert(symbol);
	 }
   };

} // namespace

#endif
