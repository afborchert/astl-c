/*
   Copyright (C) 2009-2016 Andreas Franz Borchert
   ----------------------------------------------------------------------------
   Astl-C is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either version
   2 of the License, or (at your option) any later version.

   Astl-C is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef ASTL_C_SYMTABLE_H
#define ASTL_C_SYMTABLE_H

#include <cassert>
#include <map>
#include <string>
#include "scope.hpp"
#include "symbol.hpp"

namespace AstlC {

   class SymTable {
      private:
	 Scope* scope;

      public:
	 // constructors
	 SymTable() : scope(nullptr) {
	 }
	 ~SymTable() {
	    while (scope) close();
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
	    scope = new Scope(scope);
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
