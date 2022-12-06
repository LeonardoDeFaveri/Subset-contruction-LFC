.SILENT: parser target debug

target:
	gcc src/main.c src/digraph.c src/list.c -o ssc

debug:
	gcc src/main.c src/digraph.c src/list.c -g -o ssc

parser: bison-flex/digraph.l bison-flex/digraph.y
	flex -o bison-flex/lex.yy.c bison-flex/digraph.l
	bison --defines=bison-flex/digraph.tab.h bison-flex/digraph.y -o bison-flex/digraph.tab.c
	gcc bison-flex/lex.yy.c bison-flex/digraph.tab.c src/list.c src/digraph.c -o ./parser -g
	rm bison-flex/lex.yy.c
	rm bison-flex/digraph.tab.h
	rm bison-flex/digraph.tab.c
	