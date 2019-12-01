CC = g++
CXXFLAGS = -g -Winline  --std=c++14
OBJECTS = Common.o YangAst.o DbVal.o DbTree.o Db.o Schema.o y.tab.o lex.yy.o Ydb.o Config.o Node.o NetconfMessage.o NetconfOperation.o NetconfResponse.o NetconfIdentities.o NetconfException.cc NetconfSessions.o DomUtils.o YdbApi.o

main: $(OBJECTS)
	$(CC) $(CXXFLAGS) $(OBJECTS) main.cc -o ydb -lxerces-c -lpthread

test: $(OBJECTS)
	$(CC) $(CXXFLAGS) $(OBJECTS) test.cc -o test -lxerces-c -lpthread
	$(CC) $(CXXFLAGS) $(OBJECTS) schemaTest.cc -o schemaTest -lxerces-c -lpthread

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
	rm ydb
#	rm test
#	rm schemaTest
	rm TAGS
	rm test/*.pyc
	rm test/*~
	rm test_models/*~
	rm *~

tags :
	find . -name "*.cc" -o -name "*.hh" -print | etags -	

