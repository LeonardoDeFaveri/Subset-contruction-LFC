#ifndef SUBSET_CONSTRUCTION_H
#define SUBSET_CONSTRUCTION_H

#include <stddef.h>
#include <stdint.h>
#include <stdint.h>

#include "../map/map.h"
#include "../list/list.h"
#include "../digraph/digraph.h"

struct STATE {
    char *id;
    struct LIST *nodes;
    short is_initial;
    short is_final;
    // Used to distinguish between states
    char *key;
    unsigned long key_length;
};

struct STATE *new_state(char *id, struct LIST *nodes);
struct STATE *get_state(hashmap *states, char *id);
void mark_state(hashmap *states, struct STATE *state);
struct NODE *node_from_state(struct STATE *state);
uint32_t hash(struct STATE *state);

/// `usr` must be an array of `void` in which `usr[0]` is a `hashmap *` and
/// `usr[1]` a `struct MFS *`. Populates `usr[0]` with instances of
/// `struct MFS_ITEM` and creates a set in `usr[1]` for each `value`.
void prepare_mfs(void *key, size_t ksize, uintptr_t value, void *usr);

/// Creates a `struct LIST *` from a hashmap. `usr` is a pointer to `struct LIST *`.
void hashmap_to_list(void *key, size_t ksize, uintptr_t value, void *usr);

void populate_move(void *key, size_t ksize, uintptr_t value, void *usr);
/// Creates a list of all nodes that can be reached by nodes in `nodes` via
/// a transition on `symbol`.
struct LIST *move(struct LIST *nodes, struct DIGRAPH *graph, char *symbol);
/// Auxiliary function used to decide wheter or not `value` must be included
/// into the closure. `value` is of type `struct EDGE *` and `usr` must be
// a `struct LIST *`.
void populate_closure(void *key, size_t ksize, uintptr_t value, void *usr);
/// `node_set` is a list of node ids. `edges` is a `hashmap` of all the edges
/// of the graph. Returns a list of the nodes which are part of the closure of
/// `node_set`.
struct LIST *closure(struct LIST *node_set, hashmap *edges);

/// Converts `id` into a string using the mininum required bytes.
char *parse_id(int id);

struct DIGRAPH *minimize(struct DIGRAPH *graph);

#endif