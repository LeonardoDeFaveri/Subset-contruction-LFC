#ifndef DIGRAPH_H
#define DIGRAPH_H

struct NODE {
    int id;
    char *font_name;
    char *shape;
    char *color_scheme;
    char *label;
    char *fill_color;
    char *style;
};

struct EDGE {
    struct NODE *to;
    char *font_name;
    char *label;
    char *style;
};

struct DIGRAPH {
    char *id;
    /// State `0` should be in `nodes[0]`.
    struct NODE *nodes;
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

/// Sets the `EDGE` attribute corresponding to `attr_name` to `attr_value`.
/// If `attr_name` doesn't match any attribute, `1` is returned, `0` otherwise.
short set_edge_attr(struct EDGE *edge, char *attr_name, char *attr_value);

/// Destroys `graph` and sets `graph` to `NULL`.
void destroy_digraph(struct DIGRAPH *graph);

/// Constructs a `digraph` from `raw`: a string holding a `.dot` representation of
// it. Returns a number grater than `0` if something went wrong, `0` otherwise.
unsigned short load(char *raw, struct DIGRAPH *graph);

/// Constructs a `.dot` representation of `graph`. Returns a number greater than `0`
/// if something went wrong, `0` otherwise.
unsigned short save(char *raw, struct DIGRAPH *graph);

#endif