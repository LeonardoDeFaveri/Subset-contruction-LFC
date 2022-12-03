%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>
    #include "../src/digraph.h"

    struct ATTR_PAIR {
        char *attr_name;
        char *attr_value;
    };
    struct ATTR_PAIR *new_pair(char *attr_name, char *attr_value);

    int yylex(void);
    int yyerror(char* fmt, ...);
%}

%token <id> ID
%token <digraph> DIGRAPH
%token <node> NODE
%token <edge> EDGE
%token <attr_name> FONT_NAME SHAPE COLOR_SCHEME FILL_COLOR LABEL STYLE
%token EPS
%token COMMA SEMI_COLON COLON
%token EDGE_OP EQU_OP
%token LEFT_SQUARE_BRAKET RIGHT_SQUARE_BRAKET
%token LEFT_CURLY_BRAKET RIGHT_CURLY_BRAKET
%token <compass> COMPASS

%union {
    struct DIGRAPH *digraph;
    struct NODE *node;
    struct EDGE *edge;
    char *id;
    char *attr_name;
    char *compass;
}

%type <id> id
%type <attr_name> attr_name

%start graph

%%

graph: DIGRAPH id LEFT_CURLY_BRAKET stmt_list RIGHT_CURLY_BRAKET  { printf("Succefully parsed a digraph with id = \"%s\"", $2); exit(0); };

id: ID          { $$ = $1; }
    | %empty    { $$ = ""; }
    ;   

stmt_list:
    %empty
    | stmt semicolon stmt_list
    ;

semicolon:
    SEMI_COLON
    | %empty
    ;

separator:
    SEMI_COLON
    | COMMA
    | %empty
    ;

stmt:
    node_stmt
    | edge_stmt
    | attr_stmt
    | attr_name EQU_OP ID
    ;

attr_name:
    FONT_NAME       { $$ = $1; }
    | SHAPE         { $$ = $1; }
    | COLOR_SCHEME  { $$ = $1; }
    | FILL_COLOR    { $$ = $1; }
    | LABEL         { $$ = $1; }
    | STYLE         { $$ = $1; }
    | %empty        { yyerror("Missing attribute name\n"); exit(1); }
    | ID            { yyerror("%s is not a valid attribute name\n", $1); exit(1); }
    ;

attr_stmt:
    NODE attr_list
    | EDGE attr_list
    ;

attr_list: LEFT_SQUARE_BRAKET optional_a_list RIGHT_SQUARE_BRAKET optional_attr_list;

optional_attr_list:
    %empty
    | attr_list
    ;

a_list: attr_name EQU_OP ID separator optional_a_list;

optional_a_list:
    %empty
    | a_list
    ;

node_stmt: node_id optional_attr_list;

edge_stmt: node_id edge_rhs optional_attr_list;

port:
    COLON ID port_aux
    | port_aux
    ;

optional_port:
    %empty
    | port
    ;

node_id: ID optional_port;

port_aux: COLON COMPASS;

edge_rhs: EDGE_OP node_id optional_edge_rhs;

optional_edge_rhs:
    %empty
    | edge_rhs
    ;

%%

int main(int argc, char **argv) {
    return yyparse ();
    return 0;
}

struct ATTR_PAIR *new_pair(char *attr_name, char *attr_value) {
    struct ATTR_PAIR *pair = malloc(sizeof(struct ATTR_PAIR));
    pair->attr_name = attr_name;
    pair->attr_value = attr_value;
    return pair;
}

int yyerror(char* fmt, ...) {
    fprintf(stderr, "Error: ");

    va_list ap;
    va_start(ap, fmt);

    char *ptr = fmt;
    char last = '\0';
    while (*ptr) {
        if (*ptr == '%') {
            ptr++;
            switch (*ptr) {
                case 's':
                    fprintf(stderr, "%s", va_arg(ap, char *));
                    break;
                case 'i':
                case 'd':
                    fprintf(stderr, "%d", va_arg(ap, int));
                    break;
            }
        } else {
            fprintf(stderr, "%c", *ptr);
            if (*ptr != ' ' && *ptr != '\t') {
                last = *ptr;
            }
        }
        ptr++;
    }

    if (last != '\n') {
        fprintf(stderr, "\n");
    }

    va_end(ap);
}