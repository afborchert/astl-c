System := $(shell uname -s)
BisonSources :=	$(wildcard *.ypp)
GeneratedCPPSourcesFromBison := $(patsubst %.ypp,%.tab.cpp,$(BisonSources))
GeneratedCPPSources := $(GeneratedCPPSourcesFromBison) \
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
stt_lib := ../astl/astl/libastl.a
core_objs := error.o parser.tab.o scanner.o \
   yytname.o keywords.o operators.o pp.o
testlex_objs := $(core_objs) testlex.o $(stt_lib)
testparser_objs := $(core_objs) testparser.o $(stt_lib)
run_objs := $(core_objs) run.o $(stt_lib)
astlc_objs := $(core_objs) astl-c.o $(stt_lib)
Utils := ../bison-scripts
Binaries := testlex testparser run astl-c

CXX :=		g++
CXXFLAGS :=	-std=c++11 -g -O2
LDFLAGS :=
CPPFLAGS +=	-I../astl
LDLIBS := -lboost_iostreams -lgmp
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
# DO NOT DELETE
parser.tab.o: parser.tab.cpp ../astl/astl/syntax-tree.hpp \
 ../astl/astl/config.hpp ../astl/astl/types.hpp \
 ../astl/astl/exception.hpp ../astl/astl/treeloc.hpp \
 ../astl/astl/location.hpp ../astl/astl/position.hh \
 ../astl/astl/location.hh ../astl/astl/position.hh \
 ../astl/astl/operator.hpp ../astl/astl/token.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/function.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/bindings.hpp \
 ../astl/astl/integer.hpp ../astl/astl/context.hpp symtable.hpp \
 symbol.hpp scope.hpp yystype.hpp scanner.hpp parser.hpp location.hpp \
 position.hh location.hh parser.tab.hpp stack.hh operators.hpp \
 yytname.hpp
yytname.o: yytname.cpp
operators.o: operators.cpp ../astl/astl/operator.hpp \
 ../astl/astl/token.hpp
error.o: error.cpp error.hpp parser.hpp location.hpp position.hh \
 location.hh ../astl/astl/treeloc.hpp ../astl/astl/location.hpp \
 ../astl/astl/position.hh ../astl/astl/location.hh \
 ../astl/astl/position.hh symtable.hpp symbol.hpp scope.hpp yystype.hpp \
 ../astl/astl/syntax-tree.hpp ../astl/astl/config.hpp \
 ../astl/astl/types.hpp ../astl/astl/exception.hpp \
 ../astl/astl/operator.hpp ../astl/astl/token.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/function.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/bindings.hpp \
 ../astl/astl/integer.hpp ../astl/astl/context.hpp parser.tab.hpp \
 stack.hh
scanner.o: scanner.cpp scanner.hpp parser.hpp location.hpp position.hh \
 location.hh ../astl/astl/treeloc.hpp ../astl/astl/location.hpp \
 ../astl/astl/position.hh ../astl/astl/location.hh \
 ../astl/astl/position.hh symtable.hpp symbol.hpp scope.hpp yystype.hpp \
 ../astl/astl/syntax-tree.hpp ../astl/astl/config.hpp \
 ../astl/astl/types.hpp ../astl/astl/exception.hpp \
 ../astl/astl/operator.hpp ../astl/astl/token.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/function.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/bindings.hpp \
 ../astl/astl/integer.hpp ../astl/astl/context.hpp parser.tab.hpp \
 stack.hh error.hpp keywords.hpp
testlex.o: testlex.cpp scanner.hpp parser.hpp location.hpp position.hh \
 location.hh ../astl/astl/treeloc.hpp ../astl/astl/location.hpp \
 ../astl/astl/position.hh ../astl/astl/location.hh \
 ../astl/astl/position.hh symtable.hpp symbol.hpp scope.hpp yystype.hpp \
 ../astl/astl/syntax-tree.hpp ../astl/astl/config.hpp \
 ../astl/astl/types.hpp ../astl/astl/exception.hpp \
 ../astl/astl/operator.hpp ../astl/astl/token.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/function.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/bindings.hpp \
 ../astl/astl/integer.hpp ../astl/astl/context.hpp parser.tab.hpp \
 stack.hh yytname.hpp
keywords.o: keywords.cpp scanner.hpp parser.hpp location.hpp position.hh \
 location.hh ../astl/astl/treeloc.hpp ../astl/astl/location.hpp \
 ../astl/astl/position.hh ../astl/astl/location.hh \
 ../astl/astl/position.hh symtable.hpp symbol.hpp scope.hpp yystype.hpp \
 ../astl/astl/syntax-tree.hpp ../astl/astl/config.hpp \
 ../astl/astl/types.hpp ../astl/astl/exception.hpp \
 ../astl/astl/operator.hpp ../astl/astl/token.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/function.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/bindings.hpp \
 ../astl/astl/integer.hpp ../astl/astl/context.hpp parser.tab.hpp \
 stack.hh keywords.hpp
testparser.o: testparser.cpp scanner.hpp parser.hpp location.hpp \
 position.hh location.hh ../astl/astl/treeloc.hpp \
 ../astl/astl/location.hpp ../astl/astl/position.hh \
 ../astl/astl/location.hh ../astl/astl/position.hh symtable.hpp \
 symbol.hpp scope.hpp yystype.hpp ../astl/astl/syntax-tree.hpp \
 ../astl/astl/config.hpp ../astl/astl/types.hpp \
 ../astl/astl/exception.hpp ../astl/astl/operator.hpp \
 ../astl/astl/token.hpp ../astl/astl/attribute.hpp \
 ../astl/astl/function.hpp ../astl/astl/builtin-functions.hpp \
 ../astl/astl/bindings.hpp ../astl/astl/integer.hpp \
 ../astl/astl/context.hpp parser.tab.hpp stack.hh yytname.hpp
pp.o: pp.cpp ../astl/astl/exception.hpp ../astl/astl/treeloc.hpp \
 ../astl/astl/location.hpp ../astl/astl/position.hh \
 ../astl/astl/location.hh ../astl/astl/position.hh pp.hpp
testlex.o: testlex.cpp scanner.hpp parser.hpp location.hpp position.hh \
 location.hh ../astl/astl/treeloc.hpp ../astl/astl/location.hpp \
 ../astl/astl/position.hh ../astl/astl/location.hh \
 ../astl/astl/position.hh symtable.hpp symbol.hpp scope.hpp yystype.hpp \
 ../astl/astl/syntax-tree.hpp ../astl/astl/config.hpp \
 ../astl/astl/types.hpp ../astl/astl/exception.hpp \
 ../astl/astl/operator.hpp ../astl/astl/token.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/function.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/bindings.hpp \
 ../astl/astl/integer.hpp ../astl/astl/context.hpp parser.tab.hpp \
 stack.hh yytname.hpp
testparser.o: testparser.cpp scanner.hpp parser.hpp location.hpp \
 position.hh location.hh ../astl/astl/treeloc.hpp \
 ../astl/astl/location.hpp ../astl/astl/position.hh \
 ../astl/astl/location.hh ../astl/astl/position.hh symtable.hpp \
 symbol.hpp scope.hpp yystype.hpp ../astl/astl/syntax-tree.hpp \
 ../astl/astl/config.hpp ../astl/astl/types.hpp \
 ../astl/astl/exception.hpp ../astl/astl/operator.hpp \
 ../astl/astl/token.hpp ../astl/astl/attribute.hpp \
 ../astl/astl/function.hpp ../astl/astl/builtin-functions.hpp \
 ../astl/astl/bindings.hpp ../astl/astl/integer.hpp \
 ../astl/astl/context.hpp parser.tab.hpp stack.hh yytname.hpp
run.o: run.cpp ../astl/astl/run.hpp ../astl/astl/operator.hpp \
 ../astl/astl/token.hpp ../astl/astl/exception.hpp \
 ../astl/astl/treeloc.hpp ../astl/astl/location.hpp \
 ../astl/astl/position.hh ../astl/astl/location.hh \
 ../astl/astl/position.hh ../astl/astl/types.hpp ../astl/astl/loader.hpp \
 ../astl/astl/syntax-tree.hpp ../astl/astl/config.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/function.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/bindings.hpp \
 ../astl/astl/integer.hpp ../astl/astl/context.hpp \
 ../astl/astl/generator.hpp scanner.hpp parser.hpp location.hpp \
 position.hh location.hh symtable.hpp symbol.hpp scope.hpp yystype.hpp \
 parser.tab.hpp stack.hh yytname.hpp operators.hpp pp.hpp
astl-c.o: astl-c.cpp ../astl/astl/run.hpp ../astl/astl/operator.hpp \
 ../astl/astl/token.hpp ../astl/astl/exception.hpp \
 ../astl/astl/treeloc.hpp ../astl/astl/location.hpp \
 ../astl/astl/position.hh ../astl/astl/location.hh \
 ../astl/astl/position.hh ../astl/astl/types.hpp ../astl/astl/loader.hpp \
 ../astl/astl/syntax-tree.hpp ../astl/astl/config.hpp \
 ../astl/astl/attribute.hpp ../astl/astl/function.hpp \
 ../astl/astl/builtin-functions.hpp ../astl/astl/bindings.hpp \
 ../astl/astl/integer.hpp ../astl/astl/context.hpp \
 ../astl/astl/generator.hpp scanner.hpp parser.hpp location.hpp \
 position.hh location.hh symtable.hpp symbol.hpp scope.hpp yystype.hpp \
 parser.tab.hpp stack.hh yytname.hpp operators.hpp pp.hpp
