#ifndef C_SYMBOL_H
#define C_SYMBOL_H

#include <string>

namespace AstlC {

   typedef enum {SC_TYPE, SC_UNKNOWN} SymbolClass;

   class Symbol {
      public:
	 // constructors
	 Symbol() : sc(SC_UNKNOWN) {
	 }
	 Symbol(const std::string& name_param) :
	    sc(SC_UNKNOWN), name(name_param) {
	 }
	 Symbol(SymbolClass sc_param, const std::string& name_param) :
	    sc(sc_param), name(name_param) {
	 }
	 // accessors
	 inline SymbolClass get_class() const {
	    return sc;
	 }
	 inline const std::string& get_name() const {
	    return name;
	 }
      private:
	 SymbolClass sc;
	 std::string name;
   };

} // namespace

#endif
