#ifndef C_SCOPE_H
#define C_SCOPE_H

#include <string>
#include <map>
#include "symbol.hpp"

namespace AstlC {

   class Scope {
      private:
	 typedef std::map<std::string, Symbol> SymMap;
	 SymMap symbols;
	 Scope* outer;
      public:
	 // constructors
	 Scope() : outer(0) {
	 }
	 Scope(Scope* outer_param) : outer(outer_param) {
	 }

	 // accessors
	 bool lookup(const std::string& name, Symbol& symbol) const {
	    SymMap::const_iterator it = symbols.find(name);
	    if (it == symbols.end()) {
	       if (outer) {
		  return outer->lookup(name, symbol);
	       } else {
		  return false;
	       }
	    } else {
	       symbol = it->second;
	       return true;
	    }
	 }

	 Scope* get_outer() const {
	    return outer;
	 }

	 // mutators
	 bool insert(const Symbol& symbol) {
	    std::pair<SymMap::iterator, bool> result =
	       symbols.insert(SymMap::value_type(symbol.get_name(), symbol));
	    return result.second;
	 }
   };

} // namespace AstlC

#endif
