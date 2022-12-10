#ifndef DIGRAPH_H
#define DIGRAPH_H

#include "map/map.h"
struct NODE {
    char *id;

    char *font_name;
    char *shape;
    char *color_scheme;
    char *label;
    char *fill_color;
    char *style;
};

struct EDGE {
    /// Identifier of the originating node
    char *from;
    /// Identifier of the destination node
    char *to;
    char *font_name;
    char *label;
    char *style;
};

struct DIGRAPH {
    char *id;
    /// State `0` should be in `nodes[0]`.
    hashmap *nodes;
    /// `edges[0]` holds every edge from state `0` to other states.
    struct EDGE **edges;
    /// Maximum number of nodes that can be stored.
    int nodes_count;
    /// Number of nodes currently stored.
    int nodes_inserted;
    // `edges_count[0]` tell how many edges goes out of state `0`.
    long *edges_count;
};

struct NODE* empty_node();
struct EDGE* empty_edge();
struct DIGRAPH* empty_digraph();

/// Sets the `NODE` attribute corresponding to `attr_name` to `attr_value`.
/// If `attr_name` doesn't match any attribute, `1` is returned, `0` otherwise.
short set_node_attr(struct NODE *node, char *attr_name, char *attr_value);

/// Sets `node` `NULL` attributes to the corresponding valus in
/// `default_node`.
void set_default_node_attr(struct NODE *node, struct NODE *default_node);

/// Sets the `EDGE` attribute corresponding to `attr_name` to `attr_value`.
/// If `attr_name` doesn't match any attribute, `1` is returned, `0` otherwise.
short set_edge_attr(struct EDGE *edge, char *attr_name, char *attr_value);

/// Sets `edge` `NULL` attributes to the corresponding valus in
/// `default_edge`.
void set_default_edge_attr(struct EDGE *edge, struct EDGE *default_edge);

/// Destroys `graph` and sets `graph` to `NULL`.
void destroy_digraph(struct DIGRAPH *graph);

/// Retrieves the node with id `id` from the graph.
struct NODE *get_node(struct DIGRAPH *graph, char *id);
/// Add `node` to the graph.
void add_node(struct DIGRAPH *graph, struct NODE *node);

#endif