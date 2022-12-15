#include "saver.h"
#include "../map/map.h"

void write_node_attr(struct NODE *node, FILE *out) {
    short has_prev = 0;
    if (node->label != NULL) {
        fprintf(out, "label = \"%s\"", node->label);
        has_prev = 1;
    }
    if (node->shape != NULL) {
        if (has_prev) {
            fprintf(out, ", ");
        }
        fprintf(out, "shape = \"%s\"", node->shape);
        has_prev = 1;
    }
    if (node->style != NULL) {
        if (has_prev) {
            fprintf(out, ", ");
        }
        fprintf(out, "style = \"%s\"", node->style);
        has_prev = 1;
    }
    if (node->font_name != NULL) {
        if (has_prev) {
            fprintf(out, ", ");
        }
        fprintf(out, "fontname = \"%s\"", node->font_name);
        has_prev = 1;
    }
    if (node->fill_color != NULL) {
        if (has_prev) {
            fprintf(out, ", ");
        }
        fprintf(out, "fillcolor = \"%s\"", node->fill_color);
        has_prev = 1;
    }
    if (node->color_scheme != NULL) {
        if (has_prev) {
            fprintf(out, ", ");
        }
        fprintf(out, "colorscheme = \"%s\"", node->color_scheme);
        has_prev = 1;
    }
}

void write_edge_attr(struct EDGE *edge, FILE *out) {
    short has_prev = 0;
    if (edge->label != NULL) {
        fprintf(out, "label = \"%s\"", edge->label);
        has_prev = 1;
    }
    if (edge->style != NULL) {
        if (has_prev) {
            fprintf(out, ", ");
        }
        fprintf(out, "style = \"%s\"", edge->style);
        has_prev = 1;
    }
    if (edge->font_name != NULL) {
        if (has_prev) {
            fprintf(out, ", ");
        }
        fprintf(out, "fontname = \"%s\"", edge->font_name);
        has_prev = 1;
    }
}

void save_aux(void *key, size_t ksize, uintptr_t value, void *usr) {
    if (usr == NULL) { return; }

    void **data = (void **) usr;
    FILE *out = (FILE *) data[0];
    struct DIGRAPH *graph = (struct DIGRAPH *) data[1];
    struct NODE *node = (struct NODE *) value;

    fprintf(out, "\t\"%s\" [", node->id);
    write_node_attr(node, out);
    fprintf(out, "]\n");

    struct LIST *edges = get_outgoing_from(graph, node->id);
    struct L_NODE *item = edges->first;
    while (item != NULL) {
        struct EDGE *edge = (struct EDGE *) item->value;
        fprintf(out, "\t\"%s\" -> \"%s\" [", edge->from, edge->to);
        write_edge_attr(edge, out);
        fprintf(out, "]\n");
        item = item->next;
    }

    fprintf(out, "\n");
}

void save(FILE *out, struct DIGRAPH *graph, struct NODE *default_node, struct EDGE *default_edge) {
    fprintf(out, "digraph \"%s\" {\n", graph->id);
    
    if (default_node != NULL) {
        fprintf(out, "\tnode [");
        write_node_attr(default_node, out);
        fprintf(out, "]\n");
    }
    if (default_edge != NULL) {
        fprintf(out, "\tedge [");
        write_edge_attr(default_edge, out);
        fprintf(out, "]\n");
    }
    fprintf(out, "\n");

    void **data = malloc(sizeof(void *) * 2);
    data[0] = (void *) out;
    data[1] = (void *) graph;
    hashmap_iterate(graph->nodes, save_aux, (void *) data);
    free(data);

    fprintf(out, "}\n");
    fflush(out);
}