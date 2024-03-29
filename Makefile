.SILENT: parser
.PHONY: parser

parser: bison-flex/digraph.l bison-flex/digraph.y
	flex -o bison-flex/lex.yy.c bison-flex/digraph.l
	bison --defines=bison-flex/digraph.tab.h bison-flex/digraph.y -o bison-flex/digraph.tab.c
	gcc bison-flex/lex.yy.c bison-flex/digraph.tab.c \
		src/list/list.c src/digraph/digraph.c src/map/map.c \
		src/subset_construction/subset_construction.c \
		src/saver/saver.c src/program.c \
    	-g -o ./parser
	rm bison-flex/lex.yy.c
	rm bison-flex/digraph.tab.h
	rm bison-flex/digraph.tab.c