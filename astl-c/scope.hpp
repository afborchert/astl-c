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

#ifndef ASTL_C_SCOPE_H
#define ASTL_C_SCOPE_H

#include <map>
#include <string>
#include "symbol.hpp"

namespace AstlC {

   class Scope {
      private:
	 typedef std::map<std::string, Symbol> SymMap;
	 SymMap symbols;
	 Scope* outer;

      public:
	 // constructors
	 Scope() : outer(nullptr) {
	 }
	 Scope(Scope* outer_param) : outer(outer_param) {
	 }

	 // accessors
	 bool lookup(const std::string& name, Symbol& symbol) const {
	    auto it = symbols.find(name);
	    if (it == symbols.end()) {
	       if (outer) {
		  return outer->lookup(name, symbol);
	       } else {
		  return false;
	       }
	    } else {
	       symbol = it->second;
	       return true;
	    }
	 }

	 Scope* get_outer() const {
	    return outer;
	 }

	 // mutators
	 bool insert(const Symbol& symbol) {
	    auto result =
	       symbols.insert(SymMap::value_type(symbol.get_name(), symbol));
	    return result.second;
	 }
   };

} // namespace AstlC

#endif
