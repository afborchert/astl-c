/*
   Copyright (C) 2009-2017 Andreas Franz Borchert
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

using namespace std;
using namespace Astl;
using namespace AstlC;

int main(int argc, char** argv) {
   char* cmdname = *argv++; --argc;
   if (argc > 1) {
      cerr << "Usage: " << cmdname << " [filename]" << endl;
      exit(1);
   }

   std::unique_ptr<std::locale> locale = nullptr;
   try {
      locale = std::make_unique<std::locale>("");
   } catch (std::runtime_error) {
      locale = nullptr;
   }

   Scanner* scanner;
   SymTable symtab;
   ifstream* fin = 0;
   if (argc > 0) {
      char* fname = *argv++; --argc;
      fin = new ifstream(fname);
      string filename(fname);
      if (!*fin) {
	 cerr << cmdname << ": unable to open " << fname <<
	    " for reading" << endl;
	 exit(1);
      }
      if (locale) fin->imbue(*locale);
      scanner = new Scanner(*fin, filename, symtab);
   } else {
      if (locale) std::cin.imbue(*locale);
      scanner = new Scanner(cin, "stdin", symtab);
   }
   AstlC::location loc;
   semantic_type yylval;
   int token;
   while ((token = scanner->get_token(yylval, loc)) != 0) {
      cout << yytname[token - 255];
      if (yylval) {
	 cout << " \"" << yylval->get_token().get_text() << '"';
	 yylval = NodePtr(nullptr);
      }
      cout << " at " << make_loc(loc);
      cout << endl;
   }
   delete scanner;
   if (fin) delete fin;
}
