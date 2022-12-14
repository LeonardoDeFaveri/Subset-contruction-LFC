#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdio.h>
#include "digraph/digraph.h"
#include "list/list.h"
#include "map/map.h"

struct PARSE_ARGS {
    struct ID *id;
    struct LIST *nodes;
    struct LIST *edges;
    struct NODE *default_node;
    struct EDGE *default_edge;
    hashmap *symbols;
};

struct PARSE_ARGS *empty_args();
struct PARSE_ARGS *get(void *args);

/// Checks if `s1=s2`. The check is not case-sensitive and the returned
/// value is `0` if `s1=s2`, `1` otherwise.
int streqi(const char *s1, const char *s2);
/// Frees memory occupied by `args` and `graph`.
void clean(struct PARSE_ARGS *args, struct DIGRAPH *graph);
/// Saves `graph` using dot notation in `out` file.
void save(FILE *out, struct DIGRAPH *graph, struct NODE *default_node, struct EDGE *default_edge);

int program(struct PARSE_ARGS *args, struct DIGRAPH *graph);

struct DIGRAPH *build_dfs(struct DIGRAPH *graph);

#endif