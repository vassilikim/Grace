.PHONY: clean distclean default

LLVMCONFIG = llvm-config

CXX=c++
CXXFLAGS=-Wall -g `$(LLVMCONFIG) --cxxflags`
LDFLAGS=`$(LLVMCONFIG) --ldflags --system-libs --libs all`

default: graceCompiler lib.a

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

lexer.o: lexer.cpp lexer.hpp parser.hpp ast.hpp symbol.hpp

parser.hpp parser.cpp: parser.y
	bison -v -dv -o parser.cpp parser.y

parser.o: parser.cpp lexer.hpp ast.hpp symbol.hpp

graceCompiler: lexer.o parser.o
	$(CXX) $(CXXFLAGS) -o graceCompiler lexer.o parser.o $(LDFLAGS)

lib.o: lib.c
	gcc -c lib.c -o lib.o

lib.a: lib.o
	ar rcs lib.a lib.o

clean:
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.o lib.a temp.* a.out

distclean: clean
	$(RM) graceCompiler