#ifndef C_LOCATION_H
#define C_LOCATION_H

#ifdef BISON_LOCATION_HH
#undef BISON_LOCATION_HH
#endif

#ifdef BISON_POSITION_HH
#undef BISON_POSITION_HH
#endif

#ifdef YY_YY_LOCATION_HH_INCLUDED
#undef YY_YY_LOCATION_HH_INCLUDED
#endif

#ifdef YY_YY_POSITION_HH_INCLUDED
#undef YY_YY_POSITION_HH_INCLUDED
#endif

#include "position.hh"
#include "location.hh"
#include <astl/treeloc.hpp>

namespace AstlC {

   inline Astl::Location make_loc(const location& loc) {
      return Astl::Location(
	 Astl::Position(
	    /* fix a bison bug where the initial position is w/o filename */
	    loc.begin.filename? loc.begin.filename: loc.end.filename,
	    loc.begin.line, loc.begin.column),
	 Astl::Position(loc.end.filename,
	    loc.end.line, loc.end.column));
   }

} // namespace AstlC

#endif
