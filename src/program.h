#ifndef PROGRAM_H
#define PROGRAM_H

#include "digraph/digraph.h"
#include "list/list.h"
#include "map/map.h"
#include "merge_find_set/merge_find_set.h"

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

int streqi(const char *s1, const char *s2);
void clean(struct PARSE_ARGS *args, struct DIGRAPH *graph);

int program(struct PARSE_ARGS *args, struct DIGRAPH *graph);

struct DIGRAPH *minimize(struct DIGRAPH *graph);

#endif