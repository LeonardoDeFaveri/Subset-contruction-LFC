#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdio.h>
#include "digraph/digraph.h"
#include "list/list.h"
#include "map/map.h"

/// Structure used to pass arguments to the parser function.
struct PARSE_ARGS {
    /// Graph id
    struct ID *id;
    /// List of every node of the graph
    struct LIST *nodes;
    /// List of every edge of the graph
    struct LIST *edges;
    /// Node used to store default attributes for nodes
    struct NODE *default_node;
    /// Edge used to store default attributes for edges
    struct EDGE *default_edge;
    /// Hashmap of every symbol used to transition between nodes
    /// (aka edge labels)
    hashmap *symbols;
};

/// Creates an empty instance of `PARSE_ARGS`. Every attribute is
/// initialized except for `id` which set to `NULL`.
struct PARSE_ARGS *empty_args();
/// Courtesy functions to get a `struct PARSE_ARGS *` from a `void *`.
struct PARSE_ARGS *get(void *args);

/// Checks if `s1=s2`. The check is not case-sensitive and the returned
/// value is `0` if `s1=s2`, `1` otherwise.
int streqi(const char *s1, const char *s2);
/// Frees memory occupied by `args` and `graph`.
void clean(struct PARSE_ARGS *args, struct DIGRAPH *graph);
/// Given an NFA, constructs the corresponding DFA. If the input is
/// already a DFA, the costructed graph will be identical to the first,
/// minus attribute values.
struct DIGRAPH *build_dfs(struct DIGRAPH *graph);

int program(struct PARSE_ARGS *args, struct DIGRAPH *graph);

#endif