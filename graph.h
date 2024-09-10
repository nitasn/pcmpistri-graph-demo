#ifndef GRAPH_H
#define GRAPH_H

#include "common.h"

typedef struct {
  uint16_t **adjacencies;
  uint16_t num_vertices;
  uint16_t *arrays_lengths;
} graph_t;

void free_graph(graph_t graph);

bool graph_has_self_loop(graph_t graph, bool should_use_pcmpistri);

#endif

