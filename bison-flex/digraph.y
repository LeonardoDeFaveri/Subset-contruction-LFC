%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>
    #include "../src/digraph.h"
    #include "../src/list.h"

    extern FILE *yyin;

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

    struct PARSE_ARGS {
        struct ID *id;
        struct LIST *nodes;
        struct LIST *edges;
    };
    struct PARSE_ARGS *get(void *args);

    struct NODE *default_node = NULL;
    struct EDGE *default_edge = NULL;

    int yylex(void);
    void yyerror(void *args, const char *fmt, ...);

    int line_count = 1;
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
    struct ID *id;
    int id_val;
    char *attr_name;
    struct LIST *attr_list;
}

%type <id> id
%type <attr_name> attr_name
%type <attr_list> attr_list a_list
%type <id_val> node_id

%start graph

%parse-param {void *args}

%%

graph:
    %empty  { get(args)->id = NULL; }
    | DIGRAPH id LEFT_CURLY_BRAKET stmt_list RIGHT_CURLY_BRAKET  {
        get(args)->id = $2;
        YYACCEPT;
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
    | %empty        { yyerror(args, "Missing attribute name\n"); YYABORT; }
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
                yyerror(args, "\"%s\" isn't a valid attribute for a node", pair->attr_name);
                YYABORT;
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
                yyerror(args, "\"%s\" isn't a valid attribute for an edge", pair->attr_name);
                YYABORT;
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
            yyerror(args, "\"%s\" isn't a valid id for a node. It must be an integer value", $1->value);
            YYABORT;
        }
    };

edge_rhs: EDGE_OP node_id optional_edge_rhs;

optional_edge_rhs:
    %empty
    | edge_rhs
    ;

%%

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

struct PARSE_ARGS *get(void *args) {
    return (struct PARSE_ARGS *) args;
}

//void yyerror(const char *fmt, ...) {
void yyerror(void *args, const char *fmt, ...) {
    fprintf(stderr, "Error on line %d: ", line_count);

    va_list ap;
    va_start(ap, fmt);

    const char *ptr = fmt;
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

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "You need to provide a file in input\n");
        exit(1);
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        fprintf(stderr, "Error: couldn't open \"%s\" file\n", argv[1]);
        exit(1);
    }

    yyin = input;

    /*#ifdef YYDEBUG
        yydebug = 1;
    #endif*/

    struct DIGRAPH *graph = empty_digraph();
    struct LIST *nodes = build_empty_list();
    struct LIST *edges = build_empty_list();

    struct PARSE_ARGS *args = malloc(sizeof(struct PARSE_ARGS));
    args->id = NULL;
    args->nodes = nodes;
    args->edges = edges;

    int res = yyparse((void *) args);
    
    if (res != 0) {
        fprintf(stderr, "Parsing failed\n");
    } else {
        if (args->id != NULL) {
            graph->id = args->id->value;
            printf("Successfully parsed a digraph with id: %s\n", graph->id);
        } else {
            printf("Well... the file was empty, so of course the parsing went well...\n");
        }
    }

    free(args->id);
    free(args);
    destroy_digraph(graph);
    destroy_list(nodes);
    destroy_list(edges);
    fclose(input);
    return res;
}