/*
   Copyright (C) 2009-2016 Andreas Franz Borchert
   ----------------------------------------------------------------------------
   Astl-C is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either version
   2 of the License, or (at your option) any later version.

   The Astl Library for C is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

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
