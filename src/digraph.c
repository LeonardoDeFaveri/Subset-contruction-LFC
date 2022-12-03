#include "digraph.h"
#include "list.h"
#include <stdlib.h>

short is_whitespace(char c) {
    switch (c) {
        case ' ':
        case '\n':
        case '\t': return 1;
    }
    return 0;
}

short is_digit(char c) {
    if (c >= '0' && c <= '9') { return 1; }
    return 0;
}

char* to_string(struct LIST *list) {
    if (list == NULL || list->size == 0) { return ""; }
    char *string = malloc((list->size + 1) * sizeof(char));
    long index = 0;
    L_NODE *node = list->first;
    while (node != NULL) {
        char c = *(char *) node->value;
        node = node->next;
        string[index++] = c;
    }
    string[list->size] = '\0';
    return string;
}

struct NODE* empty_node() {
    struct NODE *empty = malloc(sizeof(struct NODE));
    empty->color_scheme = NULL;
    empty->fill_color = NULL;
    empty->font_name = NULL;
    empty->label = NULL;
    empty->shape = NULL;
    empty->style = NULL;
    return empty;
}

struct EDGE* empty_edge() {
    struct EDGE *empty = malloc(sizeof(struct EDGE));
    empty->to = NULL;
    empty->label = NULL;
    empty->font_name = NULL;
    empty->style = NULL;
    return empty;
}

struct DIGRAPH* empty_digraph() {
    struct DIGRAPH *graph = malloc(sizeof(struct DIGRAPH));

    graph->nodes = NULL;
    graph->edges = NULL;
    graph->edges_count = NULL;

    graph->nodes_count = 0;
    graph->nodes_inserted = 0;

    return graph;
}

void destroy_digraph(struct DIGRAPH *graph) {
    if (graph == NULL) { return; }

    if (graph->nodes != NULL) { free(graph->nodes); }
    for (int i = 0; i < graph->nodes_count; i++) {
        if (graph->edges[i] != NULL) { free(graph->edges[i]); }
    }
    if (graph->edges) { free(graph->edges); }
    if (graph->edges_count != NULL) { free(graph->edges_count); }
    free(graph);
    graph = NULL;
}