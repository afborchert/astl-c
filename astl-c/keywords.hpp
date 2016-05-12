#ifndef C_KEYWORDS_H
#define C_KEYWORDS_H

#include <map>
#include <string>

namespace AstlC {

   class KeywordTable {
      public:
	 // constructor
	 KeywordTable();

	 // accessor
	 bool lookup(const std::string& ident, int& token) const;

      private:
	 std::map<std::string, int> tab;
   };

   extern KeywordTable keyword_table;

} // namespace AstlC

#endif
