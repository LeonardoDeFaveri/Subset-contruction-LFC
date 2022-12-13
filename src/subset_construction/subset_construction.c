#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "subset_construction.h"
#include "../merge_find_set/merge_find_set.h"
#include "../program.h"

struct STATE *new_state(char *id, struct LIST *nodes) {
    struct STATE *state = malloc(sizeof(struct STATE));
    state->id = id;
    state->nodes = nodes;
    state->is_initial = 0;
    state->is_final = 0;
    return state;
}

struct STATE *get_state(hashmap *states, char *id) {
    uintptr_t result;
    if (hashmap_get(states, id, strlen(id), &result)) {
        return (struct STATE *) result;
    }
    return NULL;
}

void mark_state(hashmap *states, struct STATE *state) {
    hashmap_set(states, state->id, strlen(state->id), (uintptr_t) state);
}

/// `usr` must be an array of `void` in which `usr[0]` is a `hashmap *` and
/// `usr[1]` a `struct MFS *`. Populates `usr[0]` with instances of
/// `struct MFS_ITEM` and creates a set in `usr[1]` for each `value`.
void prepare_mfs(void *key, size_t ksize, uintptr_t value, void *usr) {
    if (usr == NULL) { return; }

    void **data = (void **) usr;
    hashmap *nodes = (hashmap *) data[0];
    struct MFS *mfs = (struct MFS *) data[1];

    void *mfs_item = (void *) make_set(mfs, (void *) value);
    hashmap_set(nodes, key, ksize, (uintptr_t) mfs_item);
}

void populate_closure(void *key, size_t ksize, uintptr_t value, void *usr) {
    if (usr == NULL) { return; }

    struct LIST *set = (struct LIST *) usr;
    struct EDGE *edge = (struct EDGE *) value;
    if (streqi(edge->label, "eps") == 0) {
        push_back(set, (void *) edge->to);
    }
}

/// `node_set` is a list of node ids. `edges` is a `hashmap` of all the edges
/// of the graph. Returns a list of the nodes which are part of the closure of
/// `node_set`.
struct LIST *closure(struct LIST *node_set, hashmap *edges) {
    struct LIST *result = build_empty_list();
    if (length(node_set) == 0) { return result; }

    char *id = (char *) pop_first(node_set);
    while (id != NULL) {
        hashmap_iterate(edges, populate_closure, (void *) result);
        id = pop_first(node_set);
    }

    return result;
}

char *parse_id(int id) {
    char *str = NULL;
    int len = snprintf(str, 0, "%d", id);
    str = malloc(sizeof(char) * (len + 1));
    snprintf(str, 0, "%d", id);
    str[len - 1] = '\0';
    return str;
}

struct DIGRAPH *minimize(struct DIGRAPH *graph) {
    hashmap *mfs_nodes = hashmap_create();
    struct MFS *initial = make_mfs();

    //*************************************************************************
    // Initializes a support hashmap that maps node ids to their representation
    // as `struct MFS_ITEM`.
    void **data = malloc(sizeof(void *) * 2);
    data[0] = (void *) mfs_nodes;
    data[1] = (void *) initial;
    hashmap_iterate(graph->nodes, prepare_mfs, (void *) data);
    free(data);
    //*************************************************************************

    int state_id = 0;
    hashmap *marked_states = hashmap_create();
    struct LIST *unmarked_states = build_empty_list();

    void *state = (void *) build_empty_list();
    push_back((struct LIST *) state, (void *) graph->starting_node);

    while (state != NULL) {
        struct STATE *marked = new_state(parse_id(state_id), state);
        mark_state(marked_states, marked);

        state = pop_first(unmarked_states);
    }

    return NULL;
}