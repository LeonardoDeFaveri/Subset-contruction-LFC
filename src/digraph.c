#include "digraph.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>

// List of attributes:
const char *FONT_NAME = "fontname";         // 0
const char *SHAPE = "shape";                // 1
const char *COLOR_SCHEME = "colorscheme";   // 2
const char *LABEL = "label";                // 3
const char *FILL_COLOR = "fillcolor";       // 4
const char *STYLE = "style";                // 5

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

short set_node_attr(struct NODE *node, char *attr_name, char *attr_value) {
    if (strcmp(attr_name, FONT_NAME) == 0) {
        node->font_name = attr_value;
        return 0;
    }
    if (strcmp(attr_name, SHAPE) == 0) {
        node->shape = attr_value;
        return 0;
    }
    if (strcmp(attr_name, COLOR_SCHEME) == 0) {
        node->color_scheme = attr_value;
        return 0;
    }
    if (strcmp(attr_name, LABEL) == 0) {
        node->label = attr_value;
        return 0;
    }
    if (strcmp(attr_name, FILL_COLOR) == 0) {
        node->fill_color = attr_value;
        return 0;
    }
    if (strcmp(attr_name, STYLE) == 0) {
        node->style = attr_value;
        return 0;
    }
    return 1;
}

short set_edge_attr(struct EDGE *edge, char *attr_name, char *attr_value) {
if (strcmp(attr_name, FONT_NAME) == 0) {
        edge->font_name = attr_value;
        return 0;
    }
    if (strcmp(attr_name, LABEL) == 0) {
        edge->label = attr_value;
        return 0;
    }
    if (strcmp(attr_name, STYLE) == 0) {
        edge->style = attr_value;
        return 0;
    }
    return 1;
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