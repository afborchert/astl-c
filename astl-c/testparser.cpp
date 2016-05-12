#include <iostream>
#include <fstream>
#include <cstdlib>
#include "scanner.hpp"
#include "parser.hpp"
#include "yytname.hpp"
#include "symtable.hpp"
#include "location.hpp"
#include <astl/token.hpp>

using namespace std;
using namespace AstlC;
using namespace Astl;

int main(int argc, char** argv) {
   char* cmdname = *argv++; --argc;
   if (argc > 1) {
      cerr << "Usage: " << cmdname << " [filename]" << endl;
      exit(1);
   }

   SymTable symtab;
   Scanner* scanner;
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
      scanner = new Scanner(*fin, filename, symtab);
   } else {
      scanner = new Scanner(cin, "stdin", symtab);
   }

   SubnodePtr root;
   parser p(*scanner, symtab, root);
   if (p.parse() == 0) {
      cout << root << endl;
   }
   delete scanner;
}
