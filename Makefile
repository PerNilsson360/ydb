CC = g++
CXXFLAGS = -g -Winline --std=c++11
OBJECTS = Common.o YangAst.o DbVal.o DbTree.o Db.o Schema.o y.tab.o lex.yy.o

main: $(OBJECTS)
	$(CC) $(CXXFLAGS) $(OBJECTS) test.cc -o test
	$(CC) $(CXXFLAGS) $(OBJECTS) schemaTest.cc -o schemaTest

y.tab.cc : parser.y
	bison --verbose -y -d -o y.tab.cc parser.y

lex.yy.cc : lexer.l y.tab.o y.tab.hh y.tab.cc
	flex -o lex.yy.cc lexer.l

clean :
	rm *.o
	rm y.tab.cc
	rm y.tab.hh
	rm lex.yy.cc
	rm y.output
	rm test
	rm schemaTest
	rm TAGS
	rm test_models/*~
	rm *~

tags :
	find . -name "*.[chCH]" -print | etags -	

