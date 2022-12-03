%{
	#include <stdio.h>
	#include <stdlib.h>
    #include "digraph.tab.h"
	#include "../src/digraph.h"
    #include "../src/list.h"

	int yylex(void);
    int yyerror(const char* s);
	void comment(void);
	void inline_comment(void);
%}

%option noyywrap

DIGIT           [0-9]
LETTER          [a-zA-Z]
EXTRA_CHAR      [\200-\377]
CHAR            ({LETTER}|{EXTRA_CHAR}|DIGIT|_)
NUMBER			[-]?{DIGIT}+(.[0-9]+)?

%%

"/*"            { comment(); }
"//" 			{ inline_comment(); }
"\n"			{ /* New lines are ignored */ }
"#"{DIGIT}+     { /* Every output line from C preprocessor is ignored */}

"digraph"       { yylval.digraph = empty_digraph(); return(DIGRAPH); }

"node"          { yylval.node = empty_node(); return(NODE); }
"edge"          { yylval.edge = empty_edge(); return(EDGE); }

"fontname"      { yylval.attr_name = yytext; return(FONT_NAME); }
"shape"         { yylval.attr_name = yytext; return(SHAPE); }
"colorscheme"   { yylval.attr_name = yytext; return(COLOR_SCHEME); }
"fillcolor"     { yylval.attr_name = yytext; return(FILL_COLOR); }
"label"         { yylval.attr_name = yytext; return(LABEL); }
"style"         { yylval.attr_name = yytext; return(STYLE); }

"eps"           { return(EPS); }

","				{ return(COMMA); }
";"				{ return(SEMI_COLON); }
":"				{ return(COLON); }

"="				{ return(EQU_OP); }
"->"            { return(EDGE_OP); }

"["				{ return(LEFT_SQUARE_BRAKET); }
"]"				{ return(RIGHT_SQUARE_BRAKET); }
"{"				{ return(LEFT_CURLY_BRAKET); }
"}"				{ return(RIGHT_CURLY_BRAKET); }

"n"				{ yylval.compass = yytext; return(COMPASS); }
"ne"			{ yylval.compass = yytext; return(COMPASS); }
"e"				{ yylval.compass = yytext; return(COMPASS); }
"se"			{ yylval.compass = yytext; return(COMPASS); }
"s"				{ yylval.compass = yytext; return(COMPASS); }
"sw"			{ yylval.compass = yytext; return(COMPASS); }
"w"				{ yylval.compass = yytext; return(COMPASS); }
"nw"			{ yylval.compass = yytext; return(COMPASS); }
"c"				{ yylval.compass = yytext; return(COMPASS); }
"_"				{ yylval.compass = yytext; return(COMPASS); }

{NUMBER}					{ yylval.id = yytext; return(ID); }
{CHAR}+({CHAR}|{DIGIT})*    { yylval.id = yytext; return(ID); }
\"(\\.|\\\n|.)*\"         	{ yylval.id = yytext; return(ID); }

%%

void comment(void)
{
	char c, prev = 0;
  
	while ((c = input()) != 0)      /* (EOF maps to 0) */
	{
		if (c == '/' && prev == '*') { return; }
		prev = c;
	}
	yyerror("Unterminated block comment");
}

void inline_comment(void) {
	char c;

	while ((c = input()) != 0) {
		if (c == '\n') { return; }
	}
	yyerror("Unterminated inline comment");
}