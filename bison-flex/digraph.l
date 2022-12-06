%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
    #include "digraph.tab.h"
	#include "../src/digraph.h"
    #include "../src/list.h"

	int yylex(void);
	int yyerror(char* fmt, ...);

	struct ID *new_id(char *value);
    struct ID *new_num(char *value);

	void block_comment(void);
	void inline_comment(void);
	char *quoted_string(void);

	char *copy(const char *s);

	extern int line_count;
%}

%option noyywrap

DIGIT           [0-9]
LETTER          [a-zA-Z]
EXTRA_CHAR      [\200-\377]
CHAR            ({LETTER}|{EXTRA_CHAR}|_)
NUMBER			[-]?{DIGIT}+(.[0-9]+)?

%%

"/*"            { block_comment(); }
"//" 			{ inline_comment(); }
"\n"			{ line_count++; }
"#"{DIGIT}+     { /* Every output line from C preprocessor is ignored */}

"digraph"       { return(DIGRAPH); }

"node"          { return(NODE); }
"edge"          { return(EDGE); }

"eps"           { return(EPS); }

","				{ return(COMMA); }
";"				{ return(SEMI_COLON); }

"="				{ return(EQU_OP); }
"->"            { return(EDGE_OP); }

"["				{ return(LEFT_SQUARE_BRAKET); }
"]"				{ return(RIGHT_SQUARE_BRAKET); }
"{"				{ return(LEFT_CURLY_BRAKET); }
"}"				{ return(RIGHT_CURLY_BRAKET); }

{NUMBER}					{ yylval.id = new_num(copy(yytext)); return(ID); }
{CHAR}({CHAR}|{DIGIT})*  	{ yylval.id = new_id(copy(yytext)); return(ID); }
\"				{ yylval.id = new_id(quoted_string());; return(ID); }


%%

void block_comment(void)
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

char *quoted_string() {
	char c, prev = 0;

	struct LIST *text = build_empty_list();

	while ((c = input()) != 0) {
		if (c == '"' && prev != '\\') {
			char *str = to_string(text);
			destroy_list(text);
			return str;
		}
		char *character = malloc(sizeof(char));
		*character = c;
		push_back(text, (void *) character);
		prev = c;
	}

	yyerror("Missing double-quote \" in string");
}

char *copy(const char *s) {
	size_t len = strlen(s);
	char *str = malloc(sizeof(char) * (len + 1));
	strcpy(str, s);
	str[len] = '\0';
	return str;
}