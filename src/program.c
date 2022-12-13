#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "map/map.h"
#include "merge_find_set/merge_find_set.h"
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

int streqi(const char *s1, const char *s2) {
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

void print_edge(void* key, size_t ksize, uintptr_t value, void* usr) {
    struct LIST *edges = (struct LIST *) value;
    
    struct L_NODE *node = edges->first;
    while (node != NULL) {
        struct EDGE *edge = (struct EDGE *) node->value;
        printf("Edge(%s->%s) {\n", edge->from, edge->to);
        printf("\t label: %s\n", edge->label);
        printf("\t style: %s\n", edge->style);
        printf("\t font_name: %s\n", edge->font_name);
        printf("}\n");
        node = node->next;
    }
}

void print_node(void* key, size_t ksize, uintptr_t value, void* usr) {
    struct NODE *node = (struct NODE *) value;
    printf("Node[%s] {\n", node->id);
    printf("\t label: %s\n", node->label);
    printf("\t shape: %s\n", node->shape);
    printf("\t style: %s\n", node->style);
    printf("\t fill_color: %s\n", node->fill_color);
    printf("\t font_name: %s\n", node->font_name);
    printf("\t color_scheme: %s\n", node->color_scheme);
    printf("}\n");
}

int program(struct PARSE_ARGS *args, struct DIGRAPH *graph) {
    void *item = pop_first(args->nodes);
    while (item != NULL) {
        struct NODE *node = (struct NODE *) item;
        set_default_node_attr(node, args->default_node);
        item = pop_first(args->nodes);

        if (streqi(node->shape, "doublecircle") == 0) {
            if (graph->starting_node == NULL) {
                graph->starting_node = node->id;
            } else {
                fprintf(stderr, "Non unique starting node\n");
                fprintf(stderr, "Identified two candidate nodes: %s and %s\n", graph->starting_node, node->id);
                return 1;
            }
        }

        add_node(graph, node);
    }

    short error = 0;
    item = pop_first(args->edges);
    while (item != NULL) {
        struct EDGE *edge = (struct EDGE *) item;
        set_default_edge_attr(edge, args->default_edge);
        item = pop_first(args->edges);

        if (get_node(graph, edge->from) != NULL) {
            add_edge(graph, edge);
        } else {
            fprintf(stderr, "Error on edge (%s->%s): ", edge->from, edge->to);
            fprintf(stderr, "there's no node with id=\"%s\"\n", edge->from);
            error = 1;
        }
    }

    if (error) {
        return error;
    }

    struct DIGRAPH *minimized_graph = minimize(graph);

    return 0;
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

struct DIGRAPH *minimize(struct DIGRAPH *graph) {
    hashmap *mfs_nodes = hashmap_create();
    struct MFS *initial = make_mfs();

    //*************************************************************************
    // Initializes a support hashmap that maps node ids to their representation
    // as `struct MFS_ITEM`.
    void **data = malloc(sizeof(void *) * 2);
    data[0] = (void *) mfs_nodes;
    data[1] = (void *) initial;
    hashmap_iterate(graph->nodes, prepare_mfs, (void *) mfs_nodes);
    free(data);
    //*************************************************************************

    return NULL;
}