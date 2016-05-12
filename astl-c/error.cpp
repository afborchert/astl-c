#include <cstdlib>
#include <iostream>
#include "error.hpp"

using namespace std;

namespace AstlC {

void yyerror(location const* loc, char const* msg) {
   cerr << *loc << ": " << msg << endl;
   exit(1);
}

void parser::error(const location_type& loc, const std::string& msg) {
   yyerror(&loc, msg.c_str());
}

} // namespace AstlC
