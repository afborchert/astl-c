#------------------------------------------------------------------------------
# configuration parameters
#------------------------------------------------------------------------------
PREFIX = /usr/local
ASTL_LIBPREFIX = $(PREFIX)/share/astl
ASTL_C_LIBDIR = $(ASTL_LIBPREFIX)/C
BINDIR = $(PREFIX)/bin
DESTDIR =
#------------------------------------------------------------------------------
AstlPath := ../astl
BisonSources :=	$(wildcard *.ypp)
GeneratedCPPSourcesFromBison := $(patsubst %.ypp,%.tab.cpp,$(BisonSources))
GeneratedCPPSources := $(GeneratedCPPSourcesFromBison) \
   $(patsubst %.ypp,%.tab.hpp,$(BisonSources)) \
   yytname.cpp operators.cpp
GeneratedHPPSources := $(patsubst %.ypp,%.tab.hpp,$(BisonSources)) \
   operators.hpp $(wildcard *.hh)
CPPSources := $(GeneratedCPPSources) \
   error.cpp scanner.cpp testlex.cpp keywords.cpp testparser.cpp \
   pp.cpp
Objects := $(patsubst %.cpp,%.o,$(CPPSources))
MainCPPSources := testlex.cpp testparser.cpp run.cpp astl-c.cpp
MainObjects := $(patsubst %.cpp,%.o,$(MainCPPSources))
BISON := bison
stt_lib := $(AstlPath)/astl/libastl.a
core_objs := error.o parser.tab.o scanner.o \
   yytname.o keywords.o operators.o pp.o
testlex_objs := $(core_objs) testlex.o $(stt_lib)
testparser_objs := $(core_objs) testparser.o $(stt_lib)
run_objs := $(core_objs) run.o $(stt_lib)
astlc_objs := $(core_objs) astl-c.o $(stt_lib)
Utils := ../bison-scripts
Binaries := testlex testparser run astl-c

DEFS +=		-DASTL_C_LIBDIR='"$(ASTL_C_LIBDIR)"'
CXX :=		g++
CXXFLAGS :=	-Wall -g -O2
LDFLAGS :=
CPPFLAGS +=	-I$(AstlPath) -std=c++14 $(DEFS)
LDLIBS := -lboost_iostreams -lgmp -lpcre2-8
BISON := bison

.PHONY:		all clean depend
all:		$(GeneratedCPPSourcesFromBison) $(Objects) $(Binaries)
clean:		; rm -f $(Objects) $(GeneratedCPPSources) parser.output \
		   $(MainObjects)
realclean:	clean
		rm -f $(GeneratedCPPSources) $(GeneratedHPPSources) \
		   $(Binaries)

testlex:	$(testlex_objs)
		$(CXX) $(LDFLAGS) -o $@ $(testlex_objs) $(LDLIBS)
testparser:	$(testparser_objs)
		$(CXX) $(LDFLAGS) -o $@ $(testparser_objs) $(LDLIBS)
run:		$(run_objs)
		$(CXX) $(LDFLAGS) -o $@ $(run_objs) $(LDLIBS)
astl-c:		$(astlc_objs)
		$(CXX) $(LDFLAGS) -o $@ $(astlc_objs) $(LDLIBS)

yytname.cpp:	parser.tab.cpp
		perl $(Utils)/extract_yytname.pl AstlC parser.tab.cpp >$@

operators.hpp operators.cpp:	parser.tab.cpp operators.txt
		perl $(Utils)/gen_operators.pl -op operators.txt AstlC astl/operator.hpp parser.tab.cpp \
		  operators.hpp operators.cpp

$(GeneratedCPPSourcesFromBison): %.tab.cpp: %.ypp
	$(BISON) -d $<

parser.tab.o:	operators.hpp

depend:		$(CPPSources)
		perl ../astl/gcc-makedepend/gcc-makedepend.pl \
		   $(CPPFLAGS) $(CPPSources) $(MainCPPSources)

#------------------------------------------------------------------------------
# installation
#------------------------------------------------------------------------------
Binaries := astl-c
ifeq ($(DESTDIR),)
BinDir := $(BINDIR)
else
BinDir := $(DESTDIR)/$(BINDIR)
endif
InstalledBinaries := $(patsubst %,$(BinDir)/%,$(Binaries))
.PHONY:		install
install:	$(InstalledBinaries)
$(InstalledBinaries): $(BinDir)/%: %
		@mkdir -p $(BinDir)
		cp $< $@
		chmod 755 $@

# DO NOT DELETE
parser.tab.o: parser.tab.cpp ../astl/astl/syntax-tree.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/exception.hpp \
 ../astl/astl/treeloc.hpp ../astl/astl/location.hpp \
 ../astl/astl/function.hpp ../astl/astl/arity.hpp \
 ../astl/astl/bindings.hpp ../astl/astl/types.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/integer.hpp \
 ../astl/astl/stream.hpp ../astl/astl/context.hpp \
 ../astl/astl/operator.hpp ../astl/astl/token.hpp operators.hpp \
 scanner.hpp parser.hpp location.hpp position.hh location.hh symtable.hpp \
 scope.hpp symbol.hpp parser.tab.hpp yytname.hpp
parser.tab.o: parser.tab.hpp location.hh
yytname.o: yytname.cpp
operators.o: operators.cpp ../astl/astl/operator.hpp \
 ../astl/astl/token.hpp
error.o: error.cpp error.hpp parser.hpp ../astl/astl/syntax-tree.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/exception.hpp \
 ../astl/astl/treeloc.hpp ../astl/astl/location.hpp \
 ../astl/astl/function.hpp ../astl/astl/arity.hpp \
 ../astl/astl/bindings.hpp ../astl/astl/types.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/integer.hpp \
 ../astl/astl/stream.hpp ../astl/astl/context.hpp \
 ../astl/astl/operator.hpp ../astl/astl/token.hpp location.hpp \
 position.hh location.hh symtable.hpp scope.hpp symbol.hpp parser.tab.hpp
scanner.o: scanner.cpp ../astl/astl/syntax-tree.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/exception.hpp \
 ../astl/astl/treeloc.hpp ../astl/astl/location.hpp \
 ../astl/astl/function.hpp ../astl/astl/arity.hpp \
 ../astl/astl/bindings.hpp ../astl/astl/types.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/integer.hpp \
 ../astl/astl/stream.hpp ../astl/astl/context.hpp \
 ../astl/astl/operator.hpp ../astl/astl/token.hpp ../astl/astl/utf8.hpp \
 error.hpp parser.hpp location.hpp position.hh location.hh symtable.hpp \
 scope.hpp symbol.hpp parser.tab.hpp keywords.hpp scanner.hpp
testlex.o: testlex.cpp ../astl/astl/token.hpp location.hpp \
 ../astl/astl/treeloc.hpp ../astl/astl/location.hpp position.hh \
 location.hh parser.hpp ../astl/astl/syntax-tree.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/exception.hpp \
 ../astl/astl/function.hpp ../astl/astl/arity.hpp \
 ../astl/astl/bindings.hpp ../astl/astl/types.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/integer.hpp \
 ../astl/astl/stream.hpp ../astl/astl/context.hpp \
 ../astl/astl/operator.hpp symtable.hpp scope.hpp symbol.hpp \
 parser.tab.hpp scanner.hpp yytname.hpp
keywords.o: keywords.cpp scanner.hpp parser.hpp \
 ../astl/astl/syntax-tree.hpp ../astl/astl/attribute.hpp \
 ../astl/astl/exception.hpp ../astl/astl/treeloc.hpp \
 ../astl/astl/location.hpp ../astl/astl/function.hpp \
 ../astl/astl/arity.hpp ../astl/astl/bindings.hpp ../astl/astl/types.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/integer.hpp \
 ../astl/astl/stream.hpp ../astl/astl/context.hpp \
 ../astl/astl/operator.hpp ../astl/astl/token.hpp location.hpp \
 position.hh location.hh symtable.hpp scope.hpp symbol.hpp parser.tab.hpp \
 keywords.hpp
testparser.o: testparser.cpp ../astl/astl/token.hpp location.hpp \
 ../astl/astl/treeloc.hpp ../astl/astl/location.hpp position.hh \
 location.hh parser.hpp ../astl/astl/syntax-tree.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/exception.hpp \
 ../astl/astl/function.hpp ../astl/astl/arity.hpp \
 ../astl/astl/bindings.hpp ../astl/astl/types.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/integer.hpp \
 ../astl/astl/stream.hpp ../astl/astl/context.hpp \
 ../astl/astl/operator.hpp symtable.hpp scope.hpp symbol.hpp \
 parser.tab.hpp scanner.hpp yytname.hpp
pp.o: pp.cpp ../astl/astl/exception.hpp ../astl/astl/treeloc.hpp \
 ../astl/astl/location.hpp pp.hpp
testlex.o: testlex.cpp ../astl/astl/token.hpp location.hpp \
 ../astl/astl/treeloc.hpp ../astl/astl/location.hpp position.hh \
 location.hh parser.hpp ../astl/astl/syntax-tree.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/exception.hpp \
 ../astl/astl/function.hpp ../astl/astl/arity.hpp \
 ../astl/astl/bindings.hpp ../astl/astl/types.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/integer.hpp \
 ../astl/astl/stream.hpp ../astl/astl/context.hpp \
 ../astl/astl/operator.hpp symtable.hpp scope.hpp symbol.hpp \
 parser.tab.hpp scanner.hpp yytname.hpp
testparser.o: testparser.cpp ../astl/astl/token.hpp location.hpp \
 ../astl/astl/treeloc.hpp ../astl/astl/location.hpp position.hh \
 location.hh parser.hpp ../astl/astl/syntax-tree.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/exception.hpp \
 ../astl/astl/function.hpp ../astl/astl/arity.hpp \
 ../astl/astl/bindings.hpp ../astl/astl/types.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/integer.hpp \
 ../astl/astl/stream.hpp ../astl/astl/context.hpp \
 ../astl/astl/operator.hpp symtable.hpp scope.hpp symbol.hpp \
 parser.tab.hpp scanner.hpp yytname.hpp
run.o: run.cpp ../astl/astl/run.hpp ../astl/astl/exception.hpp \
 ../astl/astl/treeloc.hpp ../astl/astl/location.hpp \
 ../astl/astl/generator.hpp ../astl/astl/types.hpp \
 ../astl/astl/loader.hpp ../astl/astl/syntax-tree.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/function.hpp \
 ../astl/astl/arity.hpp ../astl/astl/bindings.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/integer.hpp \
 ../astl/astl/stream.hpp ../astl/astl/context.hpp \
 ../astl/astl/operator.hpp ../astl/astl/token.hpp scanner.hpp parser.hpp \
 location.hpp position.hh location.hh symtable.hpp scope.hpp symbol.hpp \
 parser.tab.hpp yytname.hpp operators.hpp pp.hpp
astl-c.o: astl-c.cpp ../astl/astl/run.hpp ../astl/astl/exception.hpp \
 ../astl/astl/treeloc.hpp ../astl/astl/location.hpp \
 ../astl/astl/generator.hpp ../astl/astl/types.hpp \
 ../astl/astl/loader.hpp ../astl/astl/syntax-tree.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/function.hpp \
 ../astl/astl/arity.hpp ../astl/astl/bindings.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/integer.hpp \
 ../astl/astl/stream.hpp ../astl/astl/context.hpp \
 ../astl/astl/operator.hpp ../astl/astl/token.hpp scanner.hpp parser.hpp \
 location.hpp position.hh location.hh symtable.hpp scope.hpp symbol.hpp \
 parser.tab.hpp yytname.hpp operators.hpp pp.hpp
