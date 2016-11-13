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

#include "scanner.hpp"
#include "parser.hpp"
#include "keywords.hpp"

namespace AstlC {

static struct {
   char const* keyword;
   int token;
} keywords[] = {
   {"_Bool", parser::token::_BOOL_T},
   {"_Complex", parser::token::_COMPLEX_T},
   {"_Imaginary", parser::token::_IMAGINARY_T},
   {"auto", parser::token::AUTO},
   {"break", parser::token::BREAK},
   {"case", parser::token::CASE},
   {"char", parser::token::CHAR},
   {"const", parser::token::CONST},
   {"continue", parser::token::CONTINUE},
   {"default", parser::token::DEFAULT},
   {"do", parser::token::DO},
   {"double", parser::token::DOUBLE},
   {"else", parser::token::ELSE},
   {"enum", parser::token::ENUM},
   {"extern", parser::token::EXTERN},
   {"float", parser::token::FLOAT},
   {"for", parser::token::FOR},
   {"goto", parser::token::GOTO},
   {"if", parser::token::IF},
   {"inline", parser::token::INLINE},
   {"int", parser::token::INT},
   {"long", parser::token::LONG},
   {"register", parser::token::REGISTER},
   {"restrict", parser::token::RESTRICT},
   {"return", parser::token::RETURN},
   {"short", parser::token::SHORT},
   {"signed", parser::token::SIGNED},
   {"sizeof", parser::token::SIZEOF},
   {"static", parser::token::STATIC},
   {"struct", parser::token::STRUCT},
   {"switch", parser::token::SWITCH},
   {"typedef", parser::token::TYPEDEF},
   {"union", parser::token::UNION},
   {"unsigned", parser::token::UNSIGNED},
   {"void", parser::token::VOID},
   {"volatile", parser::token::VOLATILE},
   {"while", parser::token::WHILE},
   // common extensions to implement standard features
   {"__builtin_va_arg", parser::token::VA_ARG},
   // gcc extensions and alternate keywords
   {"__alignof__", parser::token::ALIGNOF},
   {"asm", parser::token::ASM},
   {"__asm", parser::token::ASM},
   {"__asm__", parser::token::ASM},
   {"__attribute", parser::token::ATTRIBUTE},
   {"__attribute__", parser::token::ATTRIBUTE},
   {"__builtin_choose_expr", parser::token::CHOOSE_EXPR},
   {"__builtin_offsetof", parser::token::BUILTIN_OFFSETOF},
   {"__builtin_types_compatible_p", parser::token::TYPES_COMPATIBLE_P},
   {"__builtin_unreachable", parser::token::UNREACHABLE},
   {"__complex__", parser::token::_COMPLEX_T},
   {"__const", parser::token::CONST},
   {"__const__", parser::token::CONST},
   {"__inline", parser::token::INLINE},
   {"__inline__", parser::token::INLINE},
   {"__restrict", parser::token::RESTRICT},
   {"__restrict__", parser::token::RESTRICT},
   {"__signed", parser::token::SIGNED},
   {"__signed__", parser::token::SIGNED},
   {"typeof", parser::token::TYPEOF},
   {"__typeof", parser::token::TYPEOF},
   {"__typeof__", parser::token::TYPEOF},
   {"__volatile", parser::token::VOLATILE},
   {"__volatile__", parser::token::VOLATILE},
};

KeywordTable::KeywordTable() {
   for (std::size_t i = 0; i < sizeof(keywords)/sizeof(keywords[0]); ++i) {
      tab[keywords[i].keyword] = keywords[i].token;
   }
}

bool KeywordTable::lookup(const std::string& ident, int& token) const {
   std::map<std::string, int>::const_iterator it = tab.find(ident);
   if (it == tab.end()) {
      return false;
   } else {
      token = it->second;
      return true;
   }
}

KeywordTable keyword_table;

} // namespace AstlC
