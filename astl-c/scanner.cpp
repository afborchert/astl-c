/*
   Copyright (C) 2009 Andreas Franz Borchert
   ----------------------------------------------------------------------------
   The AST Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either version
   2 of the License, or (at your option) any later version.

   Ulm's Oberon Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "scanner.hpp"
#include "error.hpp"
#include "keywords.hpp"
#include "location.hpp"
#include <astl/syntax-tree.hpp>
#include <astl/token.hpp>

using namespace Astl;

namespace AstlC {

// private functions =========================================================

bool is_letter(char ch) {
   return (ch >= 'a') && (ch <= 'z') ||
      (ch >= 'A') && (ch <= 'Z') || ch == '_';
}

bool is_digit(char ch) {
   return (ch >= '0') && (ch <= '9');
}

bool is_octaldigit(char ch) {
   return (ch >= '0') && (ch <= '7');
}

bool is_hexdigit(char ch) {
   return (ch >= '0') && (ch <= '9') ||
      ch >= 'a' && ch <= 'f' || ch >= 'A' && ch <= 'F';
}

bool is_whitespace(char ch) {
   return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' ||
      ch == '\f' || ch == '\v';
}

// constructor ===============================================================

Scanner::Scanner(std::istream& in_param, const std::string& input_name_param,
      SymTable& symtab_param) :
      in(in_param), input_name(input_name_param), symtab(symtab_param),
      eof(false), ch(0), tokenstr(0) {
   pos.initialize(&input_name);
   nextch();
}

// mutator ===================================================================

int Scanner::get_token(semantic_type& yylval, location& yylloc) {
   int token = 0;
   yylval = SubnodePtr((Node*)0);

restart:
   for(;;) {
      if (eof) {
	 break;
      } else if (is_whitespace(ch)) {
	 nextch();
      } else {
	 break;
      }
   }
   tokenloc.begin = oldpos;
   if (is_letter(ch)) {
      tokenstr = new std::string();
      bool char_or_string_const_possible = ch == 'L';
      nextch();
      if (char_or_string_const_possible && ch == '\'') {
	 parse_character_constant();
	 token = parser::token::CHAR_CONSTANT;
      } else if (char_or_string_const_possible && ch == '"') {
	 parse_string_constant();
	 token = parser::token::STRING_LITERAL;
      } else {
	 while (is_letter(ch) || is_digit(ch)) {
	    nextch();
	 }
	 int keyword_token;
	 if (keyword_table.lookup(*tokenstr, keyword_token)) {
	    token = keyword_token;
	    /* a semantic value is no longer required for keywords */
	    delete tokenstr;
	    tokenstr = 0;
	 } else {
	    Symbol symbol;
	    if (symtab.lookup(*tokenstr, symbol)) {
	       switch (symbol.get_class()) {
		  case SC_TYPE:
		     token = parser::token::TYPE_IDENT;
		     break;
		  default:
		     token = parser::token::IDENT;
		     break;
	       }
	    } else {
	       token = parser::token::IDENT;
	    }
	 }
      }
      if (tokenstr) {
	 yylval = SubnodePtr(new Node(make_loc(tokenloc),
	    Token(token, tokenstr)));
      }
   } else if (is_digit(ch)) {
      tokenstr = new std::string();
      enum {OCTAL, DECIMAL, HEXADECIMAL} repr;
      bool octal_to_float = false;
      if (ch == '0') {
	 // representation?
	 nextch();
	 if (ch == 'x' || ch == 'X') {
	    repr = HEXADECIMAL;
	    nextch();
	    if (!is_hexdigit(ch)) {
	       error("invalid hexadecimal constant");
	    }
	    do {
	       nextch();
	    } while (is_hexdigit(ch));
	 } else {
	    repr = OCTAL;
	    while (is_octaldigit(ch)) {
	       nextch();
	    }
	    if (is_digit(ch)) {
	       // could be the beginning of a floating-constant
	       octal_to_float = true;
	       repr = DECIMAL;
	       do {
		  nextch();
	       } while (is_digit(ch));
	    }
	 }
      } else {
	 repr = DECIMAL;
	 do {
	    nextch();
	 } while (is_digit(ch));
      }
      switch (ch) {
	 case 'u':
	 case 'U':
	    /* unsigned */
	    nextch();
	    if (ch == 'l' || ch == 'L') {
	       /* long */
	       nextch();
	       if (ch == 'l' || ch == 'L') {
		  /* long long */
		  nextch();
	       }
	    }
	    break;
	 case 'l':
	 case 'L':
	    /* long */
	    nextch();
	    if (ch == 'l' || ch == 'L') {
	       /* long long */
	       nextch();
	    }
	    if (ch == 'u' || ch == 'U') {
	       /* long unsigned or long long unsigned */
	       nextch();
	    }
	    break;

	 case '.':
	    if (repr == DECIMAL || repr == OCTAL) {
	       octal_to_float = false; // already taken care of
	       parse_decimal_floating_constant();
	       token = parser::token::DECIMAL_FLOATING_CONSTANT;
	    } else {
	       parse_hexadecimal_floating_constant();
	       token = parser::token::HEXADECIMAL_FLOATING_CONSTANT;
	    }
	    break;

	 case 'p':
	 case 'P':
	    if (repr != HEXADECIMAL) break;
	    /* hexadecimal floating constant */
	    parse_hexadecimal_floating_constant();
	    token = parser::token::HEXADECIMAL_FLOATING_CONSTANT;
	    break;

	 case 'e':
	 case 'E':
	    /* decimal floating point constant */
	    octal_to_float = false; // already taken care of
	    if (repr != DECIMAL && repr != OCTAL) break;
	    parse_decimal_floating_constant();
	    token = parser::token::DECIMAL_FLOATING_CONSTANT;
	    break;
	 default:
	    break;
      }
      if (octal_to_float) {
	 // leading 0 with non-octal digits following
	 // which did not turn into a decimal floating constant
	 error("invalid octal constant");
      }
      if (!token) {
	 switch (repr) {
	    case OCTAL:
	       token = parser::token::OCTAL_CONSTANT;
	       break;
	    case DECIMAL:
	       token = parser::token::DECIMAL_CONSTANT;
	       break;
	    case HEXADECIMAL:
	       token = parser::token::HEXADECIMAL_CONSTANT;
	       break;
	 }
      }
      yylval = SubnodePtr(new Node(make_loc(tokenloc), Token(token, tokenstr)));
   } else {
      switch (ch) {
	 case 0:
	    /* eof */
	    break;
	 case '/':
	    nextch();
	    if (ch == '/') {
	       /* single-line comment */
	       while (!eof && ch != '\n') {
		  nextch();
	       }
	       if (eof) {
		  error("unexpected eof in single-line comment");
	       }
	    } else if (ch == '*') {
	       /* delimited comment */
	       nextch();
	       bool star = false;
	       while (!eof && (!star || ch != '/')) {
		  star = ch == '*';
		  nextch();
	       }
	       if (eof) {
		  error("unexpected eof in delimited comment");
	       } else {
		  nextch();
	       }
	    } else if (ch == '=') {
	       nextch(); token = parser::token::SLASH_EQ; break;
	    } else {
	       token = parser::token::SLASH; break;
	    }
	    goto restart;
	 case '\'':
	    tokenstr = new std::string();
	    parse_character_constant();
	    token = parser::token::CHAR_CONSTANT;
	    yylval = SubnodePtr(new Node(make_loc(tokenloc),
	       Token(token, tokenstr)));
	    break;
	 case '"':
	    tokenstr = new std::string();
	    parse_string_constant();
	    token = parser::token::STRING_LITERAL;
	    yylval = SubnodePtr(new Node(make_loc(tokenloc),
	       Token(token, tokenstr)));
	    break;
	 case '.':
	    nextch();
	    if (is_digit(ch)) {
	       tokenstr = new std::string();
	       *tokenstr += '.';
	       /* decimal floating constant */
	       parse_decimal_floating_constant();
	       token = parser::token::DECIMAL_FLOATING_CONSTANT;
	       yylval = SubnodePtr(new Node(make_loc(tokenloc),
		  Token(token, tokenstr)));
	    } else if (ch == '.') {
	       nextch();
	       if (ch != '.') {
		  error("invalid token");
	       }
	       nextch();
	       token = parser::token::DOTS;
	    } else {
	       token = parser::token::DOT;
	    }
	    break;
	 // punctuators
	 case '[':
	    nextch(); token = parser::token::LBRACKET; break;
	 case ']':
	    nextch(); token = parser::token::RBRACKET; break;
	 case '(':
	    nextch(); token = parser::token::LPAREN; break;
	 case ')':
	    nextch(); token = parser::token::RPAREN; break;
	 case '{':
	    nextch(); token = parser::token::LBRACE; break;
	 case '}':
	    nextch(); token = parser::token::RBRACE; break;
	 case '~':
	    nextch(); token = parser::token::TILDE; break;
	 case '?':
	    nextch(); token = parser::token::QMARK; break;
	 case ';':
	    nextch(); token = parser::token::SEMICOLON; break;
	 case ',':
	    nextch(); token = parser::token::COMMA; break;
	 // composite punctuators
	 case '-':
	    nextch();
	    if (ch == '>') {
	       nextch(); token = parser::token::ARROW;
	    } else if (ch == '=') {
	       nextch(); token = parser::token::MINUS_EQ;
	    } else if (ch == '-') {
	       nextch(); token = parser::token::MINUSMINUS;
	    } else {
	       token = parser::token::MINUS;
	    }
	    break;
	 case '+':
	    nextch();
	    if (ch == '=') {
	       nextch(); token = parser::token::PLUS_EQ;
	    } else if (ch == '+') {
	       nextch(); token = parser::token::PLUSPLUS;
	    } else {
	       token = parser::token::PLUS;
	    }
	    break;
	 case '&':
	    nextch();
	    if (ch == '&') {
	       nextch(); token = parser::token::AND;
	    } else if (ch == '=') {
	       nextch(); token = parser::token::AMPERSAND_EQ;
	    } else {
	       token = parser::token::AMPERSAND;
	    }
	    break;
	 case '*':
	    nextch();
	    if (ch == '=') {
	       nextch(); token = parser::token::STAR_EQ;
	    } else {
	       token = parser::token::STAR;
	    }
	    break;
	 case '%':
	    nextch();
	    if (ch == '=') {
	       nextch(); token = parser::token::PERCENT_EQ;
	    } else if (ch == '>') {
	       nextch(); token = parser::token::PERCENT_GT;
	    } else if (ch == ':') {
	       nextch();
	       if (ch == '%') {
		  nextch();
		  if (ch != ':') {
		     error("invalid token");
		  }
		  nextch();
		  token = parser::token::PERCENT_COLON_PERCENT_COLON;
	       } else {
		  token = parser::token::PERCENT_COLON;
	       }
	    } else {
	       token = parser::token::PERCENT;
	    }
	    break;
	 case '<':
	    nextch();
	    if (ch == '<') {
	       nextch();
	       if (ch == '=') {
		  nextch();
		  token = parser::token::LTLT_EQ;
	       } else {
		  token = parser::token::LTLT;
	       }
	    } else if (ch == '=') {
	       nextch(); token = parser::token::LE;
	    } else if (ch == ':') {
	       nextch(); token = parser::token::LT_COLON;
	    } else if (ch == '%') {
	       nextch(); token = parser::token::LT_PERCENT;
	    } else {
	       token = parser::token::LT;
	    }
	    break;
	 case '>':
	    nextch();
	    if (ch == '>') {
	       nextch();
	       if (ch == '=') {
		  nextch();
		  token = parser::token::GTGT_EQ;
	       } else {
		  token = parser::token::GTGT;
	       }
	    } else if (ch == '=') {
	       nextch(); token = parser::token::GE;
	    } else {
	       token = parser::token::GT;
	    }
	    break;
	 case '=':
	    nextch();
	    if (ch == '=') {
	       nextch(); token = parser::token::EQEQ;
	    } else {
	       token = parser::token::EQ;
	    }
	    break;
	 case '!':
	    nextch();
	    if (ch == '=') {
	       nextch(); token = parser::token::NE;
	    } else {
	       token = parser::token::EXCLAMATION;
	    }
	    break;
	 case '^':
	    nextch();
	    if (ch == '=') {
	       nextch(); token = parser::token::CARET_EQ;
	    } else {
	       token = parser::token::CARET;
	    }
	    break;
	 case '|':
	    nextch();
	    if (ch == '=') {
	       nextch(); token = parser::token::BAR_EQ;
	    } else if (ch == '|') {
	       nextch(); token = parser::token::OR;
	    } else {
	       token = parser::token::BAR;
	    }
	    break;
	 case ':':
	    nextch();
	    if (ch == '>') {
	       nextch(); token = parser::token::COLON_GT;
	    } else {
	       token = parser::token::COLON;
	    }
	    break;
	 case '#':
	    if (pos.column == 2) {
	       // preprocessor directive
	       nextch();
	       parse_directive();
	       goto restart;
	    }
	    nextch();
	    if (ch == '#') {
	       nextch(); token = parser::token::HASH_HASH;
	    } else {
	       token = parser::token::HASH;
	    }
	    break;
	 default:
	    nextch();
	    error("invalid token");
	    goto restart;
      }
   }

   yylloc = tokenloc;
   return token;
}

// private methods ===========================================================

/*
 * get next character from the input stream, if available:
 *  - pos gets updated (
 */
void Scanner::nextch() {
   tokenloc.end = pos;
   oldpos = pos;
   if (eof) {
      ch = 0; return;
   }
   if (tokenstr) {
      *tokenstr += ch;
   }
   char c;
   if (!in.get(c)) {
      eof = true; ch = 0; return;
   }
   ch = c;
   if (ch == '\n') {
      pos.lines();
   } else {
      pos.columns();
   }
}

void Scanner::parse_decimal_floating_constant() {
   // we are at a '.', a digit behind the '.', at 'e' or 'E'
   if (ch != 'e' && ch != 'E') {
      // parse digit sequence of fractional constant, if ch neither 'e' nor 'E'
      do {
	 nextch();
      } while (is_digit(ch));
   }
   if (ch == 'e' || ch == 'E') {
      // parse exponential part
      nextch();
      if (ch == '+' || ch == '-') {
	 nextch();
      }
      if (!is_digit(ch)) {
	 error("digits missing in exponent");
      }
      nextch();
      while (is_digit(ch)) {
	 nextch();
      }
   }
   if (ch == 'f' || ch == 'l' || ch == 'F' || ch == 'L') {
      // floating suffix
      nextch();
   }
}

void Scanner::parse_hexadecimal_floating_constant() {
   // we are at a '.', at 'e' or 'E'
   if (ch == '.') {
      nextch();
      while (is_hexdigit(ch)) {
	 nextch();
      };
   }
   if (ch != 'p' && ch != 'P') {
      error("exponent missing in hexadecimal floating constant");
   }
   nextch();
   // parse exponential part
   if (ch == '+' || ch == '-') {
      nextch();
   }
   if (!is_digit(ch)) {
      error("digits missing in exponent");
   }
   nextch();
   while (is_digit(ch)) {
      nextch();
   }
   if (ch == 'f' || ch == 'l' || ch == 'F' || ch == 'L') {
      // floating suffix
      nextch();
   }
}

void Scanner::parse_character_constant() {
   // ch == '\''
   nextch();
   if (ch == '\'') {
      error("'' is not permitted as character constant");
   }
   do {
      if (ch == '\n') {
	 error("newline not permitted in character constant");
      } else if (ch == '\\') {
	 nextch();
	 if (is_octaldigit(ch)) {
	    nextch();
	    if (is_octaldigit(ch)) nextch();
	    if (is_octaldigit(ch)) nextch();
	 } else {
	    switch (ch) {
	       /* simple-escape-sequence */
	       case '\'': case '"': case '?': case '\\': case 'a':
	       case 'b': case 'f': case 'n': case 'r': case 't': case 'v':
		  nextch();
		  break;
	       case 'x':
		  nextch();
		  if (!is_hexdigit(ch)) {
		     error("hexadecimal digit expected");
		  }
		  nextch();
		  while (is_hexdigit(ch)) {
		     nextch();
		  }
		  break;
	       default:
		  error("invalid character constant");
	    }
	 }
      } else {
	 nextch();
      }
   } while (ch != '\'');
   nextch();
}

void Scanner::parse_string_constant() {
   // ch == '"'
   nextch();
   while (ch != '"') {
      if (ch == '\n') {
	 error("newline not permitted in string constant");
      } else if (ch == '\\') {
	 nextch();
	 if (is_octaldigit(ch)) {
	    nextch();
	    if (is_octaldigit(ch)) nextch();
	    if (is_octaldigit(ch)) nextch();
	 } else {
	    switch (ch) {
	       /* simple-escape-sequence */
	       case '\'': case '"': case '?': case '\\': case 'a':
	       case 'b': case 'f': case 'n': case 'r': case 't': case 'v':
		  nextch();
		  break;
	       case 'x':
		  nextch();
		  if (!is_hexdigit(ch)) {
		     error("hexadecimal digit expected");
		  }
		  nextch();
		  while (is_hexdigit(ch)) {
		     nextch();
		  }
		  break;
	       default:
		  error("invalid string constant");
	    }
	 }
      } else {
	 nextch();
      }
   }
   nextch();
}

void Scanner::parse_directive() {
   while (ch != '\n' && is_whitespace(ch)) {
      nextch();
   }
   if (is_letter(ch)) {
      // regular directives including #pragma are ignored
      while (ch != '\n') {
	 nextch();
      }
   } else if (is_digit(ch)) {
      // line number and actual pathname in gcc preprocessor output syntax,
      // see http://boredzo.org/blog/archives/category/programming/toolchain/gcc
      int line = ch - '0';
      nextch();
      while (is_digit(ch)) {
	 line = line * 10 + ch - '0';
	 nextch();
      }
      // skip whitespace
      while (is_whitespace(ch)) {
	 nextch();
      }
      // extract pathname
      if (ch != '"') {
	 error("broken linemarker in cpp output");
      }
      nextch();
      tokenstr = new std::string();
      while (ch != '"' && ch != '\n') {
	 nextch();
      }
      if (ch == '\n') {
	 error("broken linemarker in cpp output");
      }
      std::string* filename = tokenstr; tokenstr = 0;
      // skip flags (1 = push, 2 = pop, 3 = system header,
      // 4 = requires extern "C") as they do not concern us
      while (ch != '\n') {
	 nextch();
      }
      // fix current position
      pos.filename = filename;
      pos.line = line;
   } else if (ch != '\n') {
      error("broken preprocessor directive");
   }
}

void Scanner::error(char const* msg) {
   yyerror(&tokenloc, msg);
}

} // namespace AstlC
