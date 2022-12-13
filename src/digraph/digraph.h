#ifndef DIGRAPH_H
#define DIGRAPH_H

#include "../map/map.h"

struct NODE {
    char *id;
    short is_final;

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
    char *starting_node;
    hashmap *nodes;
    hashmap *edges;
    hashmap *symbols;
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

/// Destroys `node` and all its attributes.
void destroy_node(struct NODE *node);
/// Destroy `edge` attributes. `edge->from` and `edge->to` aren't touched
/// because they might be invalid pointers.
void destroy_edge(struct EDGE *edge);
/// Destroys `graph` and all its nodes and edges.
void destroy_digraph(struct DIGRAPH *graph);

/// Retrieves the node with id `id` from the graph.
/// Returns `NULL` if there's no such node.
struct NODE *get_node(struct DIGRAPH *graph, char *id);
/// Add `node` to the graph.
void add_node(struct DIGRAPH *graph, struct NODE *node);

/// Returns a list of the edges that come out for node `source`.
/// Return `NULL` if there's no such edge.
struct LIST *get_outgoing_from(struct DIGRAPH *graph, char *source);

/// Adds `edge` to the list of edges coming out of `edge->from`.
void add_edge(struct DIGRAPH *graph, struct EDGE *edge);

#endif