#include "graph.h"

#include <assert.h>
#include <smmintrin.h>  // for SSE4.2


bool has_self_loop__naive(unsigned short vertex, int *neighbors, int num_neighbors) {
  for (int i = 0; i < num_neighbors; ++i) {
    if (neighbors[i] == vertex) {
      return true;
    }
  }
  return false;
}

bool has_self_loop__pcmpistri(unsigned short vertex, int *neighbors, int num_neighbors) {
  // For simplicity, assume each vertex's num_neighbors is a multiple of 8
  assert(num_neighbors % 8 == 0);

  // Create a vector with the vertex ID in the least significant 16 bits
  __m128i vertex_vec = _mm_set1_epi16(vertex);

  // We'll use PCMPISTRI to compare 8 16-bit integers at once.
  // _SIDD_UWORD_OPS: treat the data as 16-bit integers
  // _SIDD_CMP_EQUAL_ANY: check for equality with any
  // _SIDD_LEAST_SIGNIFICANT: start comparison from least significant bit
  #define FLAGS _SIDD_UWORD_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_LEAST_SIGNIFICANT

  for (int i = 0; i < num_neighbors; i += 8) {
    __m128i neighbor_vec = _mm_loadu_si128((__m128i *)&neighbors[i]);

    // we can use this (faster) version if we know the ids of the vertices are 1-based
    int result = _mm_cmpistri(vertex_vec, neighbor_vec, FLAGS);

    // we have to use this (slightly slower) version if ids of the vertices might be 0
    // int result = _mm_cmpestri(vertex_vec, 1, neighbor_vec, 8, FLAGS);

    if (result != 8) {
      return true;
    }
  }

  return false;
}


bool graph_has_self_loop(graph_t graph, bool should_use_pcmpistri) {

  bool (*has_self_loop)(unsigned short, int*, int) =
    should_use_pcmpistri ? has_self_loop__pcmpistri : has_self_loop__naive;

  for (int v = 0; v < graph.num_vertices; ++v) {
    int num_neighbors = graph.arrays_lengths[v];

    if (has_self_loop(v, graph.adjacencies[v], num_neighbors)) {
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