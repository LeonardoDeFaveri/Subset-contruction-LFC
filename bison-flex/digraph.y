%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>
    #include "../src/digraph.h"
    #include "../src/list.h"

    struct ATTR_PAIR {
        char *attr_name;
        char *attr_value;
    };
    struct ATTR_PAIR *new_pair(char *attr_name, char *attr_value);

    struct ID {
        short is_number;
        char *value;
    };
    struct ID *new_id(char *value);
    struct ID *new_num(char *value);

    struct NODE *default_node = NULL;
    struct EDGE *default_edge = NULL;

    int yylex(void);
    int yyerror(char* fmt, ...);
%}

%token <id> ID
%token DIGRAPH
%token NODE
%token EDGE
%token <attr_name> FONT_NAME SHAPE COLOR_SCHEME FILL_COLOR LABEL STYLE
%token EPS
%token COMMA SEMI_COLON
%token EDGE_OP EQU_OP
%token LEFT_SQUARE_BRAKET RIGHT_SQUARE_BRAKET
%token LEFT_CURLY_BRAKET RIGHT_CURLY_BRAKET

%union {
    struct NODE *node;
    struct EDGE *edge;
    struct ID *id;
    char *attr_name;
    struct LIST *attr_list;
    int id_val;
}

%type <id> id
%type <attr_name> attr_name
%type <attr_list> attr_list a_list
%type <id_val> node_id
%type <digraph> graph

%start graph

%%

graph: DIGRAPH id LEFT_CURLY_BRAKET stmt_list RIGHT_CURLY_BRAKET  {
    printf("Succefully parsed a digraph with id = \"%s\"", $2->value);
    return 0;
};

id: ID          { $$ = $1; }
    | %empty    { $$ = new_id(""); }
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
    ID              { $$ = $1->value; }
    | %empty        { yyerror("Missing attribute name\n"); exit(1); }
    ;

attr_stmt:
    NODE attr_list      {
        if (default_node == NULL) {
            default_node = empty_node();
        }

        void *item = pop_first($2);
        while (item != NULL) {
            struct ATTR_PAIR *pair = (struct ATTR_PAIR*) item;
            if (set_node_attr(default_node, pair->attr_name, pair->attr_value)) {
                yyerror("\"%s\" isn't a valid attribute for a node", pair->attr_name);
                exit(1);
            }
            item = pop_first($2);
        }
    }
    | EDGE attr_list {
        if (default_edge == NULL) {
            default_edge = empty_edge();
        }

        void *item = pop_first($2);
        while (item != NULL) {
            struct ATTR_PAIR *pair = (struct ATTR_PAIR*) item;
            if (set_edge_attr(default_edge, pair->attr_name, pair->attr_value)) {
                yyerror("\"%s\" isn't a valid attribute for an edge", pair->attr_name);
                exit(1);
            }
            item = pop_first($2);
        }
    }
    ;

attr_list:
    %empty  { $$ = build_empty_list(); }
    | LEFT_SQUARE_BRAKET a_list RIGHT_SQUARE_BRAKET attr_list {
        $$ = $4;
        push_list($$, $2);
    };

a_list:
    %empty      { $$ = build_empty_list(); }
    | attr_name EQU_OP ID separator a_list {
        $$ = $5;
        struct ATTR_PAIR *pair = new_pair($1, $3->value);
        push_back($$, (void *) pair);
    };

node_stmt: node_id attr_list;  

edge_stmt: node_id edge_rhs attr_list;

node_id: ID {
        if ($1->is_number) {
            $$ = atoi($1->value);
        } else {
            yyerror("\"%s\" isn't a valid id for a node. It must be an integer value", $1->value);
            exit(1);
        }
    };

edge_rhs: EDGE_OP node_id optional_edge_rhs;

optional_edge_rhs:
    %empty
    | edge_rhs
    ;

%%

int main(int argc, char **argv) {
    /*#ifdef YYDEBUG
        yydebug = 1;
    #endif*/

    return yyparse ();
    return 0;
}

struct ATTR_PAIR *new_pair(char *attr_name, char *attr_value) {
    struct ATTR_PAIR *pair = malloc(sizeof(struct ATTR_PAIR));
    pair->attr_name = attr_name;
    pair->attr_value = attr_value;
    return pair;
}

struct ID *new_id(char *value) {
    struct ID *id = malloc(sizeof(struct ID));
    id->is_number = 0;
    id->value = value;
    return id;
}
struct ID *new_num(char *value) {
    struct ID *id = malloc(sizeof(struct ID));
    id->is_number = 1;
    id->value = value;
    return id;
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