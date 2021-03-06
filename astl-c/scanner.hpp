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

#ifndef ASTL_C_SCANNER_H
#define ASTL_C_SCANNER_H

#include <iostream>
#include <memory>
#include "parser.hpp"
#include "symtable.hpp"
#include "location.hpp"

namespace AstlC {

   typedef parser::semantic_type semantic_type;

   class Scanner {
      public:
	 Scanner(std::istream& in, const std::string& input_name,
	    SymTable& symtab);

	 // mutators
	 int get_token(semantic_type& yylval, location& yylloc);

      private:
	 std::istream& in;
	 std::string input_name;
	 unsigned char ch;
	 bool eof;
	 int lasttoken; // last token returned by get_token()
	 position oldpos, pos;
	 location tokenloc;
	 std::unique_ptr<std::string> tokenstr;
	 SymTable& symtab;

	 // private mutators
	 void nextch();
	 void error(char const* msg);
	 void parse_decimal_floating_constant();
	 void parse_hexadecimal_floating_constant();
	 void parse_character_constant();
	 void parse_string_constant();
	 void parse_directive();
	 void convert_to_utf8();
   };

   inline int yylex(semantic_type* yylval, location* yylloc, Scanner& scanner) {
      return scanner.get_token(*yylval, *yylloc);
   }

} // namespace AstlC

#endif
