/*
   Copyright (C) 2009-2016 Andreas Franz Borchert
   ----------------------------------------------------------------------------
   Astl-C is free software; you can redistribute it
   and/or modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either version
   2 of the License, or (at your option) any later version.

   The Astl Library for C is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef C_PP_H
#define C_PP_H

#include <iostream>
#include <string>
#include <vector>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <astl/exception.hpp>

namespace AstlC {

   typedef std::vector<std::string> Args;
   typedef boost::iostreams::stream<boost::iostreams::file_descriptor_source>
      fdistream;

   class cpp_istream: public fdistream {
      public:
	 cpp_istream(const std::string& cpp_path,
	    const Args& args, const std::string& input_file)
	    throw(Astl::Exception);
   };

} // namespace AstlC

#endif
