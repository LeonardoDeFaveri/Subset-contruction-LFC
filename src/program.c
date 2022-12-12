#include "program.h"
#include "list/list.h"
#include "digraph/digraph.h"

struct PARSE_ARGS *empty_args() {
    struct PARSE_ARGS *args = malloc(sizeof(struct PARSE_ARGS));
    args->id = NULL;
    args->nodes = build_empty_list();
    args->edges = build_empty_list();
    args->default_node = empty_node();
    args->default_edge = empty_edge();
    return args;
}

struct PARSE_ARGS *get(void *args) {
    return (struct PARSE_ARGS *) args;
}

short streqi(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) { return 0; }

    while (*s1 != '\0' && *s2 != '\0') {
        if (*s1 == *s2 || *s1 + 32 == *s2 || *s1 - 32 == *s2) {
            s1++;
            s2++;
        } else {
            return 1;
        }
    }

    return 0;
}

void clean(struct PARSE_ARGS *args, struct DIGRAPH *graph) {
    free(args->id);
    destroy_list(args->nodes);
    destroy_list(args->edges);
    free(args);
    destroy_digraph(graph);
}

short program(struct PARSE_ARGS *args) {
    return 0;
}