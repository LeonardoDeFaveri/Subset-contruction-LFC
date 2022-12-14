#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "subset_construction.h"
#include "../merge_find_set/merge_find_set.h"
#include "../program.h"

// Function for sorted insertion of string values
short cmp(const void *v1, const void *v2) {
    return strcmp((char *) v1, (char *) v2);
}

struct STATE *new_state(char *id, struct LIST *nodes) {
    struct STATE *state = malloc(sizeof(struct STATE));
    state->id = id;
    state->nodes = nodes;
    state->is_initial = 0;
    state->is_final = 0;
    state->key = NULL;
    state->key_length = 0;
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

struct NODE *node_from_state(struct STATE *state) {
    struct NODE *node = empty_node();
    node->id = state->id;
    if (state->is_final) {
        node->shape = "doublecircle";
        node->is_final = 1;
    }
    return node;
}

struct EDGE *edge_from_states(struct NODE *from, struct NODE *to, char *symbol) {
    struct EDGE *edge = empty_edge();
    edge->from = from->id;
    edge->to = to->id;
    edge->label = symbol;
    return edge;
}

void calculate_key(struct STATE *state) {
    if (state->nodes->size == 0) {
        state->key_length = 1; // 1 because of '\0'
        state->key = "";
        return ;
    }

    int len = strlen((char *) state->nodes->last->value);
    unsigned long size = sizeof(char) * len * state->nodes->size;
    size += state->nodes->size - 1; // Between every node there's a separator
    char *key = malloc(size);

    unsigned long key_i = 0;
    
    struct L_NODE *node = (struct L_NODE *) state->nodes->first;
    while (node != NULL) {
        char *id = (char *) ((struct NODE *) node->value);
        while (*id != '\0') {
            key[key_i++] = *id;
            id++;
        }
        node = node->next;
        if (node != NULL) {
            key[key_i++] = ',';
        }
    }
    key[key_i] = '\0';

    state->key_length = key_i;
    state->key = key;
}

uint32_t hash(struct STATE *state) {
    if (state->key == NULL) {
        calculate_key(state);
    }

    return hash_data((const unsigned char *) state->key, state->key_length);
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

void hashmap_to_list(void *key, size_t ksize, uintptr_t value, void *usr) {
    struct LIST *list = usr;
    push_back(list, (void *) value);
}

struct LIST *move(struct LIST *nodes, struct DIGRAPH *graph, char *symbol) {
    hashmap *tmp = hashmap_create();

    struct L_NODE *item = nodes->first;
    while (item != NULL) {
        struct NODE *node = (struct NODE *) item;
        struct LIST *edges = get_outgoing_from(graph, node->id);

        struct L_NODE *item2 = edges->first;
        while (item2 != NULL) {
            struct EDGE *edge = (struct EDGE *) item2->value;
            if (strcmp(edge->label, symbol) == 0) {
                hashmap_set(tmp, (void *) edge->to, strlen(edge->to), (uintptr_t) edge->to);
            }
            item2 = item2->next;
        }

        item = item->next;
    }
    struct LIST *result = build_empty_list();
    hashmap_iterate(tmp, hashmap_to_list, (void *) result);
    hashmap_free(tmp);
    return result;
}

void populate_closure(void *key, size_t ksize, uintptr_t value, void *usr) {
    if (usr == NULL) { return; }

    hashmap *set = (hashmap *) usr;
    struct LIST *edges = (struct LIST *) value;

    struct L_NODE *item = edges->first;
    while (item != NULL) {
        struct EDGE *edge = (struct EDGE *) item->value;
        if (strcmp(edge->label, "eps") == 0) {
            hashmap_set(set, (void *) edge->to, strlen(edge->to), (uintptr_t) edge->to);
            //push_sorted_unique(set, (void *) edge->to, cmp);
            //push_back(set, (void *) edge->to);
        }
        item = item->next;
    }
}

/// `node_set` is a list of node ids. `edges` is a `hashmap` of all the edges
/// of the graph. Returns a list of the nodes which are part of the closure of
/// `node_set`.
struct LIST *closure(struct LIST *node_set, struct DIGRAPH *graph) {
    struct LIST *result = build_empty_list();
    hashmap *tmp = hashmap_create();
    if (length(node_set) == 0) { return result; }

    struct L_NODE *item = node_set->first;
    while (item != NULL) {
        void *id = item->value;
        hashmap_set(tmp, (void *) id, strlen((char *) id), (uintptr_t) id);
        //push_sorted_unique(result, (void *) id, cmp);
        struct LIST *edges = get_outgoing_from(graph, (char *) id);
        struct L_NODE *item2 = edges->first;
        while (item2 != NULL) {
            struct EDGE *edge = (struct EDGE *) item2->value;
            if (strcmp(edge->label, "eps") == 0) {
                hashmap_set(tmp, (void *) edge->to, strlen(edge->to), (uintptr_t) edge->to);
            }
            item2 = item2->next;
        }

        item = item->next;
    }

    hashmap_iterate(tmp, hashmap_to_list, (void *) result);
    hashmap_free(tmp);
    return result;
}

char *parse_id(int id) {
    char *str = NULL;
    int len = snprintf(str, 0, "%d", id);
    str = malloc(sizeof(char) * (len + 1));
    snprintf(str, len + 1, "%d", id);
    str[len] = '\0';
    return str;
}

void find_duplicate_aux(void *key, size_t ksize, uintptr_t value, void *usr) {
    if (usr == NULL) { return; }

    void **data = (void **) usr;
    uint32_t state_hash = *((uint32_t *) data[0]);
    struct STATE **dup_ref = (struct STATE **) data[1];
    struct STATE *state = (struct STATE *) value;
    if (state_hash == hash(state)) {
        *dup_ref = state;
    }
}

struct STATE *find_duplicate(struct STATE *state, struct LIST *unmarked_states, hashmap *marked_states) {
    uint32_t state_hash = hash(state);

    struct L_NODE *item = unmarked_states->first;
    while (item != NULL) {
        if (state_hash == hash((struct STATE *) item->value)) {
            return (struct STATE *) item->value;
        }
        item = item->next;
    }

    struct STATE *duplicate = NULL;
    struct STATE **dup_ref = &duplicate;
    void **data = malloc(sizeof(void *) * 2);
    data[0] = (void *) &state_hash;
    data[1] = (void *) dup_ref;
    hashmap_iterate(marked_states, find_duplicate_aux, (void *) data);
    free(data);
    return *dup_ref;
}

void print_nodes(struct LIST *list) {
    if (list == NULL) {
        printf("NULL\n");
        return;
    }
    printf("[");

    struct L_NODE *node = list->first;
    while (node != NULL) {
        printf("%s", (char *) node->value);
        if (node->next != NULL) {
            printf(", ");
        }
        node = node->next;
    }

    printf("]\n");
}

struct DIGRAPH *minimize(struct DIGRAPH *graph) {
    printf("\n");
    struct DIGRAPH *minimized = empty_digraph();
    minimized->id = "minimized";
    struct LIST *symbols = build_empty_list();
    hashmap_iterate(graph->symbols, hashmap_to_list, (void *) symbols);

    /*hashmap *mfs_nodes = hashmap_create();
    struct MFS *initial = make_mfs();

    *************************************************************************
     Initializes a support hashmap that maps node ids to their representation
     as `struct MFS_ITEM`.
    void **data = malloc(sizeof(void *) * 2);
    data[0] = (void *) mfs_nodes;
    data[1] = (void *) initial;
    hashmap_iterate(graph->nodes, prepare_mfs, (void *) data);
    free(data);
    *************************************************************************
    */

    int state_id = 0;
    hashmap *marked_states = hashmap_create();
    struct LIST *unmarked_states = build_empty_list();

    // Creates the initial state
    struct STATE *state = new_state(parse_id(state_id++), build_empty_list());
    push_back(state->nodes, (void *) graph->starting_node);
    state->nodes = closure(state->nodes, graph);    
    state->is_initial = 1;

    // Creates the initial node of the graph from the initial state
    struct NODE *from = node_from_state(state);
    add_node(minimized, from);  // Adds the initial node to the graph
    minimized->starting_node = state->id;

    while (state != NULL) {
        printf("Current state: { id = %s, nodes = ", state->id);
        print_nodes(state->nodes);
        printf(" }\n");
        mark_state(marked_states, state);

        struct L_NODE *symbol = symbols->first;
        while (symbol != NULL) {
            struct LIST *moves = move(state->nodes, graph, (char *) symbol->value);
            struct LIST *nodes = closure(moves, graph);
            printf("Closure(Move on %s): ", (char *) symbol->value);
            print_nodes(nodes);

            if (nodes->size != 0) {
                struct STATE *candidate = new_state(NULL, nodes);
                struct STATE *dup = find_duplicate(candidate, unmarked_states, marked_states);
                struct NODE *to;

                // Creates a new node in the graph and a new state
                if (dup == NULL) {
                    candidate->id = parse_id(state_id++);
                    to = node_from_state(candidate);
                    add_node(minimized, to);
                    push_back(unmarked_states, (void *) candidate);
                } else {
                    to = get_node(minimized, dup->id);
                }

                // Adds a transition from node `from` to node `to` on `symbol`.
                add_edge(minimized, edge_from_states(from, to, (char *) symbol->value));
            }

            symbol = symbol->next;
        }

        state = (struct STATE *) pop_first(unmarked_states);
    }

    return minimized;
}