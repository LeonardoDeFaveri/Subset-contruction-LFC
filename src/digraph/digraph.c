#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "digraph.h"
#include "../list/list.h"
#include "../map/map.h"

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
    empty->from = NULL;
    empty->to = NULL;
    empty->label = NULL;
    empty->font_name = NULL;
    empty->style = NULL;
    return empty;
}

struct DIGRAPH* empty_digraph() {
    struct DIGRAPH *graph = malloc(sizeof(struct DIGRAPH));
    graph->id = NULL;
    graph->starting_node = NULL;
    graph->nodes = hashmap_create();
    graph->edges = hashmap_create();

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

void set_default_node_attr(struct NODE *node, struct NODE *default_node) {
    if (node->font_name == NULL) {
        node->font_name = default_node->font_name;
    }
    if (node->shape == NULL) {
        node->shape = default_node->shape;
    }
    if (node->color_scheme == NULL) {
        node->color_scheme = default_node->color_scheme;
    }
    if (node->label == NULL) {
        node->label = default_node->label;
    }
    if (node->fill_color == NULL) {
        node->fill_color = default_node->fill_color;
    }
    if (node->style == NULL) {
        node->style = default_node->style;
    }
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

void set_default_edge_attr(struct EDGE *edge, struct EDGE *default_edge) {
    if (edge->font_name == NULL) {
        edge->font_name = default_edge->font_name;
    }
    if (edge->label == NULL) {
        edge->label = default_edge->label;
    }
    if (edge->style == NULL) {
        edge->style = default_edge->style;
    }
}

void destroy_node(struct NODE *node) {
    if (node == NULL) { return; }

    if (node->id != NULL) { free(node->id); }
    //if (node->label != NULL) { free(node->label); }
    //if (node->shape != NULL) { free(node->shape); }
    //if (node->style != NULL) { free(node->style); }
    //if (node->fill_color != NULL) { free(node->fill_color); }
    //if (node->font_name != NULL) { free(node->font_name); }
    //if (node->color_scheme != NULL) { free(node->color_scheme); }
    free(node);
}

void destroy_edge(struct EDGE *edge) {
    if (edge == NULL) { return; }

    //if (edge->label != NULL) { free(edge->label); }
    //if (edge->style != NULL) { free(edge->style); }
    //if (edge->font_name != NULL) { free(edge->font_name); }
    free(edge);
}

void destroy_node_aux(void* key, size_t ksize, uintptr_t value, void* usr) {
    if (usr == NULL) { return; }

    void (*destroyer)(struct NODE *) = usr;
    destroyer((struct NODE *) value);
}

void destroy_edge_aux(void* key, size_t ksize, uintptr_t value, void* usr) {
    if (usr == NULL) { return; }

    void (*destroyer)(struct EDGE *) = usr;
    destroyer((struct EDGE *) value);
}

void destroy_digraph(struct DIGRAPH *graph) {
    if (graph == NULL) { return; }

    if (graph->nodes != NULL) {
        hashmap_iterate(graph->nodes, destroy_node_aux, (void *) destroy_node);
        hashmap_free(graph->nodes);
    }
    if (graph->edges != NULL) {
        hashmap_iterate(graph->edges, destroy_edge_aux, (void *) destroy_edge);
        hashmap_free(graph->edges);
    }
    free(graph);
}

struct NODE *get_node(struct DIGRAPH *graph, char *id) {
    uintptr_t result;
    if (hashmap_get_set(graph->nodes, id, strlen(id), &result)) {
        return (struct NODE *) result;
    }
    return NULL;
}

void add_node(struct DIGRAPH *graph, struct NODE *node) {
    struct NODE *other_node = get_node(graph, node->id);
    if (other_node != NULL) {
        set_default_node_attr(other_node, node);
    } else {
        uintptr_t ptr = (uintptr_t) node;
        hashmap_set(graph->nodes, node->id, strlen(node->id), ptr);
    }
}

struct LIST *get_outgoing_from(struct DIGRAPH *graph, char *source) {
    uintptr_t result;
    if (hashmap_get_set(graph->edges, source, strlen(source), &result)) {
        return (struct LIST *) result;
    }
    return NULL;
}

void add_edge(struct DIGRAPH *graph, struct EDGE *edge) {
    struct LIST *other_edges = get_outgoing_from(graph, edge->from);
    if (other_edges != NULL) {
        push_back(other_edges, (void *) edge);
    } else {
        other_edges = build_empty_list();
        push_back(other_edges, edge);
        uintptr_t ptr = (uintptr_t) other_edges;
        hashmap_set(graph->edges, edge->from, strlen(edge->from), ptr);
    }
}