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

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <astl/run.hpp>
#include "scanner.hpp"
#include "parser.hpp"
#include "yytname.hpp"
#include "symtable.hpp"
#include "location.hpp"
#include "operators.hpp"
#include "pp.hpp"

using namespace std;
using namespace Astl;
using namespace AstlC;

static char* cmdname;

void usage() {
   cerr << "Usage: " << cmdname << " source rules [pattern [count]]" << endl;
   exit(1);
}

int main(int argc, char** argv) {
   cmdname = *argv++; --argc;
   /* fetch source argument */
   if (argc == 0) usage();
   char* source_name = *argv++; --argc;
   /* fetch rules argument */
   if (argc == 0) usage();
   char* rules_name = *argv++; --argc;
   /* fetch pattern argument, if given */
   std::size_t count = 0; char* pattern = 0;
   if (argc > 0) {
      pattern = *argv++; --argc;
   }
   /* fetch count argument, if given */
   if (argc > 0) {
      char* count_string = *argv++; --argc;
      std::istringstream is(count_string);
      is >> count;
      if (!is || count < 1) usage();
   }
   /* all arguments should have been processed by now */
   if (argc > 0) usage();

   try {
      SymTable symtab;
      symtab.open();
      // non-ISO typedefs
      symtab.insert(Symbol(SC_TYPE, "__builtin_va_list"));
      // http://gcc.gnu.org/onlinedocs/gcc/Local-Labels.html
      symtab.insert(Symbol(SC_TYPE, "__label"));
      symtab.insert(Symbol(SC_TYPE, "__label__"));
      symtab.open();

      // ifstream source(source_name);
      Args args;
      // various definitions to hack around non-ISO constructs
      // that are imported from various gcc headers
      args.push_back("-D__extension__=");
      cpp_istream source("gcc", args, source_name);
      if (!source) {
	 cerr << cmdname << ": unable to open " << source_name <<
	    " for reading" << endl;
	 exit(1);
      }
      Scanner scanner(source, source_name, symtab);
      NodePtr root;
      parser p(scanner, symtab, root);
      if (p.parse() != 0) exit(1);

      run(root, rules_name, pattern, count, Op::LPAREN, cout);
   } catch (Exception e) {
      cout << endl;
      cerr << e.what() << endl;
   }
}
