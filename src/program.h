#ifndef PROGRAM_H
#define PROGRAM_H

#include "list/list.h"
#include "map/map.h"
#include "merge_find_set/merge_find_set.h"

struct PARSE_ARGS {
    struct ID *id;
    struct LIST *nodes;
    struct LIST *edges;
    struct NODE *default_node;
    struct EDGE *default_edge;
};

struct PARSE_ARGS *empty_args();
struct PARSE_ARGS *get(void *args);

short streqi(const char *s1, const char *s2);
void clean(struct PARSE_ARGS *args, struct DIGRAPH *graph);

short program(struct PARSE_ARGS *args);

#endif