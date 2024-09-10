#ifndef GRAPH_H
#define GRAPH_H

#include "common.h"

typedef struct {
  unsigned short **adjacencies;
  int num_vertices;
  int *arrays_lengths;
} graph_t;

void free_graph(graph_t graph);

bool graph_has_self_loop(graph_t graph);

bool vertex_has_self_loop(unsigned short vertex, unsigned short *neighbors, int num_neighbors);

#endif

