#include "graph.h"

#include <assert.h>
// #include <smmintrin.h>  // for SSE4.2

bool graph_has_self_loop(graph_t graph) {
  for (int i = 0; i < graph.num_vertices; ++i) {
    int num_neighbors = graph.arrays_lengths[i];

    if (vertex_has_self_loop(i, graph.adjacencies[i], num_neighbors)) {
      return true;
    }
  }

  return false;
}

bool vertex_has_self_loop(unsigned short vertex, unsigned short *neighbors, int num_neighbors) {
  // for simplicity, assume each vertex's num_neighbors is a multiple of 8
  assert(num_neighbors % 8 == 0);

  for (int i = 0; i < num_neighbors; ++i) {
    if (neighbors[i] == vertex) {
      return true;
    }
  }

  return false;
}

void free_graph(graph_t graph) {
  for (int i = 0; i < graph.num_vertices; ++i) {
    free(graph.adjacencies[i]);
  }
  free(graph.adjacencies);
  free(graph.arrays_lengths);
}