#ifndef C_ERROR_H
#define C_ERROR_H

#include "parser.hpp"
#include "location.hpp"

namespace AstlC {

   void yyerror(location const* loc, char const* msg);

} // namespace AstlC

#endif
