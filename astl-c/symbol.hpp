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

#ifndef ASTL_C_SYMBOL_H
#define ASTL_C_SYMBOL_H

#include <string>

namespace AstlC {

   typedef enum {SC_TYPE, SC_UNKNOWN} SymbolClass;

   class Symbol {
      public:
	 // constructors
	 Symbol() : sc(SC_UNKNOWN) {
	 }
	 Symbol(const std::string& name_param) :
	    sc(SC_UNKNOWN), name(name_param) {
	 }
	 Symbol(SymbolClass sc_param, const std::string& name_param) :
	    sc(sc_param), name(name_param) {
	 }
	 // accessors
	 inline SymbolClass get_class() const {
	    return sc;
	 }
	 inline const std::string& get_name() const {
	    return name;
	 }
      private:
	 SymbolClass sc;
	 std::string name;
   };

} // namespace

#endif
