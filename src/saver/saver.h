#ifndef SAVER_H
#define SAVER_H

#include <stdint.h>
#include <stdio.h>

#include "../list/list.h"
#include "../digraph/digraph.h"

/// Write `node` attributes on `out` file.
void write_node_attr(struct NODE *node, FILE *out);
/// Write `edge` attributes on `out` file.
void write_edge_attr(struct EDGE *edge, FILE *out);
void save_aux(void *key, size_t ksize, uintptr_t value, void *usr);
/// Saves `graph` information (id, nodes, edges) on `out` file.
void save(FILE *out, struct DIGRAPH *graph, struct NODE *default_node, struct EDGE *default_edge);

#endif