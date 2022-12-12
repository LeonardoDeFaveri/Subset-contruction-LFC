%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>
    #include "../src/program.h"
    #include "../src/digraph/digraph.h"
    #include "../src/list/list.h"
    #include "../src/map/map.h"
    #include "../src/merge_find_set/merge_find_set.h"

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
    char *id_val;
    char *attr_name;
    struct LIST *attr_list;
    struct LIST *edges;
}

%type <id> id
%type <attr_name> attr_name
%type <attr_list> attr_list a_list
%type <id_val> node_id
%type <edges> edge_rhs optional_edge_rhs

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
        struct NODE *default_node = get(args)->default_node;

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
        struct EDGE *default_edge = get(args)->default_edge;

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

node_stmt: node_id attr_list {
        struct NODE *node = empty_node();
        node->id = $1;

        void *item = pop_first($2);
        while (item != NULL) {
            struct ATTR_PAIR *pair = (struct ATTR_PAIR*) item;
            if (set_node_attr(node, pair->attr_name, pair->attr_value)) {
                yyerror(args, "\"%s\" isn't a valid attribute for a node", pair->attr_name);
                YYABORT;
            }
            item = pop_first($2);
        }

        push_back(get(args)->nodes, (void *) node);
    };  

edge_stmt: node_id edge_rhs attr_list {
        struct PARSE_ARGS *p_args = get(args);

        struct EDGE *edge_attr = empty_edge();
        void *attr = pop_first($3);
        while (attr != NULL) {
            struct ATTR_PAIR *attr_pair = (struct ATTR_PAIR *)attr;
            set_edge_attr(edge_attr, attr_pair->attr_name, attr_pair->attr_value);
            attr = pop_first($3);
        } 

        void *item = pop_first($2);
        while (item != NULL) {
            struct EDGE *edge = (struct EDGE *) item;
            edge->from = $1;
            set_default_edge_attr(edge, edge_attr);
            push_back(p_args->edges, (void *) edge);
            item = pop_first($2);
        }
    };

node_id: ID {
        if ($1->value == "") {
            yyerror(args, "Empty string can't be used as node id");
            YYABORT;
        }
        $$ = $1->value;
    };

edge_rhs: EDGE_OP node_id optional_edge_rhs {
        $$ = $3;
        struct EDGE *edge = empty_edge();
        edge->to = $2;
        push_back($$, (void *) edge);
    };

optional_edge_rhs:
    %empty          { $$ = build_empty_list(); }
    | edge_rhs      { $$ = $1; }
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

    struct PARSE_ARGS *args = empty_args();
    struct DIGRAPH *graph = empty_digraph();

    int res = yyparse((void *) args);
    fclose(input);
    
    if (res != 0) {
        fprintf(stderr, "Parsing failed\n");
        clean(args, graph);
        return res;
    } else if (args->id == NULL) {
        printf("Well... the file was empty, so of course the parsing went well...\n");
        clean(args, graph);
        return res;
    }

    graph->id = args->id->value;
    printf("Successfully parsed a digraph with id: %s\n", graph->id);

    res = program(args, graph);

    clean(args, graph);
    return res;
}