#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "program.h"
#include "map/map.h"
#include "list/list.h"
#include "digraph/digraph.h"
#include "subset_construction/subset_construction.h"
#include "saver/saver.h"

struct PARSE_ARGS *empty_args() {
    struct PARSE_ARGS *args = malloc(sizeof(struct PARSE_ARGS));
    args->id = NULL;
    args->nodes = build_empty_list();
    args->edges = build_empty_list();
    args->default_node = empty_node();
    args->default_edge = empty_edge();
    args->symbols = hashmap_create();
    return args;
}

struct PARSE_ARGS *get(void *args) {
    return (struct PARSE_ARGS *) args;
}

int streqi(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) { return 1; }

    while (*s1 != '\0' && *s2 != '\0') {
        if (*s1 == *s2 || *s1 + 32 == *s2 || *s1 - 32 == *s2) {
            s1++;
            s2++;
        } else {
            return 1;
        }
    }

    if (*s1 == '\0' && *s2 == '\0') {
        return 0;
    }

    return 1;
}

void set_defaults_for_nodes(void *key, size_t ksize, uintptr_t value, void *usr) {
    if (usr == NULL) { return; }

    struct NODE *default_node_attr = (struct NODE *) usr;
    set_default_node_attr((struct NODE *) value, default_node_attr);
}

void set_defaults_for_edges(void *key, size_t ksize, uintptr_t value, void *usr) {
    if (usr == NULL) { return; }

    struct EDGE *default_edge_attr = (struct EDGE *) usr;
    struct LIST *eges = (struct LIST *) value;

    struct L_NODE *item = eges->first;
    while (item != NULL) {
        set_default_edge_attr((struct EDGE *) item->value, default_edge_attr);
        item = item->next;
    }
}

void clean(struct PARSE_ARGS *args, struct DIGRAPH *graph) {
    free(args->id);
    destroy_list(args->nodes);
    destroy_list(args->edges);
    free(args);
    destroy_digraph(graph);
}

int program(struct PARSE_ARGS *args, struct DIGRAPH *graph) {
    short find_final = 0;
    void *item = pop_first(args->nodes);
    while (item != NULL) {
        struct NODE *node = (struct NODE *) item;
        set_default_node_attr(node, args->default_node);
        item = pop_first(args->nodes);

        if (streqi(node->shape, "doublecircle") == 0) {
            node->is_final = 1;
            find_final = 1;
        }

        add_node(graph, node);
    }

    if (find_final == 0) {
        fprintf(stderr, "No final node. At least one node must be a final node (shape=doublecircle)\n");
        return 1;
    }

    struct NODE *starting_node = get_node(graph, "0");
    if (starting_node == NULL) {
        fprintf(stderr, "No starting node. You must add a node with id=0\n");
        return 1;
    }
    graph->starting_node = starting_node->id;

    short error = 0;
    item = pop_first(args->edges);
    while (item != NULL) {
        struct EDGE *edge = (struct EDGE *) item;
        set_default_edge_attr(edge, args->default_edge);
        item = pop_first(args->edges);

        if (get_node(graph, edge->from) == NULL) {
            fprintf(stderr, "Error on edge (%s->%s): ", edge->from, edge->to);
            fprintf(stderr, "there's no node with id=\"%s\"\n", edge->from);
            error = 1;  
        } else if (get_node(graph, edge->to) == NULL) {
            fprintf(stderr, "Error on edge (%s->%s): ", edge->from, edge->to);
            fprintf(stderr, "there's no node with id=\"%s\"\n", edge->to);
            error = 1;  
        } else {
            add_edge(graph, edge);
        }
    }

    if (error) {
        return error;
    }

    hashmap_remove(graph->symbols, "eps", 3);
    struct DIGRAPH *dfa = build_dfa(graph);

    //hashmap_iterate(dfa->nodes, set_defaults_for_nodes, (void *) args->default_node);
    //hashmap_iterate(dfa->edges, set_defaults_for_edges, (void *) args->default_edge);

    FILE *out = fopen("out.dot", "w");
    if (out == NULL) {
        fprintf(stderr, "Error: couldn't open output file: out.dot\n");
        destroy_digraph(dfa);
        return 1;
    }
    save(out, dfa, args->default_node, args->default_edge);

    return 0;
}