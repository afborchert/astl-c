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

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <locale>
#include <stdexcept>
#include <astl/token.hpp>
#include "location.hpp"
#include "parser.hpp"
#include "scanner.hpp"
#include "symtable.hpp"
#include "yytname.hpp"

using namespace Astl;
using namespace AstlC;

int main(int argc, char** argv) {
   char* cmdname = *argv++; --argc;
   if (argc > 1) {
      std::cerr << "Usage: " << cmdname << " [filename]" << std::endl;
      exit(1);
   }

   std::unique_ptr<std::locale> locale = nullptr;
   try {
      locale = std::make_unique<std::locale>("");
   } catch (std::runtime_error&) {
      locale = nullptr;
   }

   SymTable symtab;
   Scanner* scanner;
   std::ifstream* fin = 0;
   if (argc > 0) {
      char* fname = *argv++; --argc;
      fin = new std::ifstream(fname);
      std::string filename(fname);
      if (!*fin) {
	 std::cerr << cmdname << ": unable to open " << fname <<
	    " for reading" << std::endl;
	 exit(1);
      }
      if (locale) fin->imbue(*locale);
      scanner = new Scanner(*fin, filename, symtab);
   } else {
      if (locale) std::cin.imbue(*locale);
      scanner = new Scanner(std::cin, "stdin", symtab);
   }

   NodePtr root;
   parser p(*scanner, symtab, root);
   if (p.parse() == 0) {
      std::cout << root << std::endl;
   }
   delete scanner;
}
