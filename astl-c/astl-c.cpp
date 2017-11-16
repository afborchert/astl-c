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
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <astl/run.hpp>
#include <astl/generator.hpp>
#include <astl/loader.hpp>
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

class SyntaxTreeGeneratorForC: public SyntaxTreeGenerator {
   public:
      virtual NodePtr gen(int& argc, char**& argv) throw(Exception) {
	 Args args;
	 if (argc == 0) {
	    throw Exception("no source file given");
	 }
	 // prepare symbol table
	 SymTable symtab;
	 symtab.open();
	 // insert non-ISO typedefs
	 symtab.insert(Symbol(SC_TYPE, "__builtin_va_list"));
	 // http://gcc.gnu.org/onlinedocs/gcc/Local-Labels.html
	 symtab.insert(Symbol(SC_TYPE, "__label"));
	 symtab.insert(Symbol(SC_TYPE, "__label__"));
	 symtab.open();

	 /* which C preprocessor is to be taken? */
	 const char* cpp = "gcc";
	 if (std::strcmp(*argv, "--cpp") == 0) {
	    --argc; ++argv;
	    if (argc == 0) {
	       throw Exception("argument for --cpp is missing");
	    }
	    cpp = *argv++; --argc;
	 }

	 /* multiple sources to be processed? */
	 bool multiple_sources = false;
	 std::size_t source_count = 0;
	 NodePtr super_root; // collecting multiple sources
	 if (std::strcmp(*argv, "--sources--") == 0) {
	    multiple_sources = true;
	    --argc; ++argv;
	    super_root = std::make_shared<Node>(Location(),
	       Operator("translation_units"));
	 }

	 /* process options and source files */
	 do {
	    /* collect options for the preprocessor */
	    if (std::strcmp(*argv, "--cpp--") == 0) {
	       /* take all arguments enclosed in --cpp-- ... --cpp-- */
	       --argc; ++argv;
	       while (argc > 0 && std::strcmp(*argv, "--cpp--") != 0) {
		  args.push_back(*argv++); --argc;
	       }
	       if (argc > 0) {
		  --argc; ++argv;
	       }
	    } else {
	       /* take all arguments with a leading - */
	       while (argc > 0 && *argv[0] == '-' &&
		     (!multiple_sources ||
			std::strcmp(*argv, "--sources--") != 0)) {
		  args.push_back(*argv++); --argc;
	       }
	    }
	    /* end this if we are running out of arguments */
	    if (argc == 0) {
	       if (multiple_sources && source_count > 0) {
		  throw Exception("closing --sources-- is missing");
	       } else {
		  throw Exception("no source file given");
	       }
	    }
	    /* leave loop,
	       if sources were given and the source list is closed */
	    if (multiple_sources && std::strcmp(*argv, "--sources--") == 0) {
	       if (source_count == 0) {
		  throw Exception("no source files given");
	       }
	       break;
	    }
	    ++source_count;
	    /* take source file name */
	    char* source_name = *argv++; --argc;


	    // various definitions to hack around non-ISO constructs
	    // that are imported from various gcc headers
	    args.push_back("-D__extension__=");

	    /* pass the source through the preprocessor */
	    cpp_istream source(cpp, args, source_name);
	    if (!source) {
	       std::ostringstream os;
	       os << "unable to open " << source_name << " for reading";
	       throw Exception(os.str());
	    }

	    /* accept locale from environment if it works out */
	    std::unique_ptr<std::locale> locale = nullptr;
	    try {
	       locale = std::make_unique<std::locale>("");
	    } catch (std::runtime_error) {
	       locale = nullptr;
	    }
	    if (locale) source.imbue(*locale);
	    /* run the output of the preprocessor through our scanner ... */
	    Scanner scanner(source, source_name, symtab);
	    /* ... and parse it */
	    NodePtr root;
	    parser p(scanner, symtab, root);
	    if (p.parse() != 0) {
	       std::ostringstream os;
	       os << "parsing of " << source_name << " failed";
	       throw Exception(os.str());
	    }
	    if (multiple_sources) {
	       *super_root += root;
	    } else {
	       super_root = root;
	    }
	 } while (multiple_sources);
	 return super_root;
      }
};

int main(int argc, char** argv) {
   try {
      SyntaxTreeGeneratorForC astgen;
      Loader loader;
      char* path = std::getenv("ASTL_C_PATH");
      if (path) {
	 char* dir = path;
	 for (char* cp = path; *cp; ++cp) {
	    if (*cp == ':') {
	       if (dir) {
		  * cp = 0;
		  loader.add_library(dir);
		  dir = 0;
	       }
	    } else if (!dir) {
	       dir = cp;
	    }
	 }
	 if (dir) loader.add_library(dir);
      } else {
	 // default path
	 loader.add_library("/usr/local/share/astl/C");
	 loader.add_library("/usr/share/astl/C");
      }
      run(argc, argv, astgen, loader, Op::LPAREN);
   } catch (Exception e) {
      cout << endl;
      cerr << e.what() << endl;
   } catch (std::exception& e) {
      cout << endl;
      cerr << e.what() << endl;
   }
}

/*

=head1 NAME

astl-c -- run Astl scripts for C99 sources

=head1 SYNOPSIS

B<astl-c> F<astl-script> [B<--cpp> preprocessor] [gcc preprocessor options...] F<C-source> [I<args>]

B<astl-c> F<astl-script> [B<--cpp> preprocessor] [B<--cpp--> gcc preprocessor options... B<--cpp-->] F<C-source> [I<args>]

B<astl-c> F<astl-script> [B<--cpp> preprocessor] B<--sources--> sources and gcc preprocessor options B<--sources--> [I<args>]

=head1 DESCRIPTION

Astl scripts can be started explicitly using the B<astl-c> interpreter
or implicitly using a shebang line in the first line of the script:

   #!/usr/bin/env astl-c

The construct using F</usr/bin/env> attempts to find F<astl-c>
anywhere in the user's path.

F<astl-c> invokes first the preprocessor with the
given preprocessor options. By default, I<gcc> is taken but the
option B<--cpp> can be used to specify an another preprocessor
which should support B<-E> and other common preprocessor options.
If some of the preprocessor options
do not begin with a `-' character, the whole set of gcc preprocessor
options needs to be enclosed in ``--cpp--'' .. ``--cpp--''.

In addition, following defines are added to convert various gcc-specific
constructs found in the gcc headers to ISO C:

=over 4

=item I<__extension__>

is defined to be empty.

=back

In addition, the types I<__builtin_va_list>, I<__label>,
and I<__label__> are recognized as builtin types.

The result of the gcc preprocessor is subsequently parsed
and an abstract syntax tree is built. Any syntax errors cause
the execution to be aborted and an error message to be printed.

Once the abstract syntax tree is present, the Astl script is
loaded and executed following the execution order defined
in section 12.5 of the Report of the Astl Programming Language.

All arguments behind the C source file are put into a list
and bound to the variable I<args> in the I<main> function.

Multiple sources and optional gcc preprocessor options may be passed
to F<astl-c> by enclosing them in B<--sources--> tokens. In this
case, each of the sources is individually passed to the
gcc preprocessor and subsequently parsed. All resulting
abstract syntax trees are then put under a root node in their
original order with ``translation_units'' as operator.

=head1 EXAMPLE

The following example prints a warning message for each
assignment found within a condition:

   #!/usr/bin/env astl-c

   import printer;

   attribution rules {
      ("=" lvalue rvalue) as assignment in ("condition" *) as cond -> {
	 println("warning: assignment within a condition at ",
	    location(assignment), ": ", gentext(cond));
      };
   }

=head1 ENVIRONMENT

The environment variable I<ASTL_C_PATH> can be used to specify
a colon-separated list of directories where library modules
are looked for. By default, F</usr/local/share/astl/C> and
F</usr/share/astl/C> are used. The script itself can add more
directories to the library search path using the Astl
library clause (see section 12.1 in the I<Report of the
Astl Programming Language>).

=head1 BUGS

In its lexical analysis, F<astl-c> tries to record the exact
source locations of all tokens. This is, however, limited as
the preprocessor of F<gcc> does not preserve the correct
spacing even for input lines which otherwise remain untouched.
One option is to use the preprocessor option F<-traditional-cpp>
which preserves spacing to the extent possible but which unfortunately
does not support preprocessor features of ISO C like stringify
operators.

=head1 AUTHOR

Andreas F. Borchert

=cut

*/
