/*
   Copyright (C) 2009 Andreas Franz Borchert
   ----------------------------------------------------------------------------
   Astl-C is free software; you can redistribute it
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


#include <boost/version.hpp>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <astl/exception.hpp>
#include "pp.hpp"

namespace AstlC {

static int create_pipe(const std::string& cpp_path,
      const Args& args, const std::string& input_file) throw(Astl::Exception) {
   int fds[2];
   if (pipe(fds) < 0) {
      throw Astl::Exception("unable to create a pipe");
   }
   pid_t pid = fork();
   if (pid < 0) {
      throw Astl::Exception("unable to fork");
   }
   if (pid == 0) {
      close(fds[0]);
      dup2(fds[1], 1);
      close(fds[1]);
      int argc = 4 + args.size();
      const char** argv = new const char*[argc];
      int argi = 0;
      argv[argi++] = cpp_path.c_str();
      argv[argi++] = "-E";
      for (int i = 0; i < args.size(); ++i) {
	 argv[argi++] = args[i].c_str();
      }
      argv[argi++] = input_file.c_str();
      argv[argi++] = 0;
      execvp(argv[0], (char* const*) argv);
      exit(255);
   }
   close(fds[1]);
   return fds[0];
}

cpp_istream::cpp_istream(const std::string& cpp_path,
      const Args& args, const std::string& input_file) throw(Astl::Exception) :
   fdistream(create_pipe(cpp_path, args, input_file)
#if BOOST_VERSION >= 104601
      , boost::iostreams::close_handle
#endif
      ) {
}

} // namespace AstlC
