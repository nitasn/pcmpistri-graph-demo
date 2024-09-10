#include "graph.h"

#include <assert.h>
#include <smmintrin.h>  // for SSE4.2


bool has_self_loop__naive(uint16_t vertex, uint16_t *neighbors, uint16_t num_neighbors) {
  for (uint16_t i = 0; i < num_neighbors; ++i) {
    if (neighbors[i] == vertex) {
      return true;
    }
  }
  return false;
}

bool has_self_loop__pcmpistri(uint16_t vertex, uint16_t *neighbors, uint16_t num_neighbors) {
  // for simplicity, assume each vertex's num_neighbors is a multiple of 8
  assert(num_neighbors % 8 == 0);

  // Create a vector with the vertex ID in the least significant 16 bits
  __m128i vertex_vec = _mm_set1_epi16(vertex);

  for (uint16_t i = 0; i < num_neighbors; i += 8) {
    __m128i neighbor_vec = _mm_loadu_si128((__m128i *)&neighbors[i]);

    // Use PCMPISTRI to compare 8 16-bit integers at once
    // _SIDD_UWORD_OPS: treat the data as 16-bit integers
    // _SIDD_CMP_EQUAL_ANY: check for equality with any
    // _SIDD_LEAST_SIGNIFICANT: start comparison from least significant bit
    int result = _mm_cmpistri(vertex_vec, neighbor_vec, 
                              _SIDD_UWORD_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_LEAST_SIGNIFICANT);

    if (result != 8) {
      // Double-check the potential match
      if (neighbors[i + result] == vertex) {
        return true;  // Confirmed match
      }
    }
  }

  return false;  // No match found
}


bool graph_has_self_loop(graph_t graph, bool should_use_pcmpistri) {

  bool (*has_self_loop)(uint16_t, uint16_t*, uint16_t) =
    should_use_pcmpistri ? has_self_loop__pcmpistri : has_self_loop__naive;

  for (uint16_t v = 0; v < graph.num_vertices; ++v) {
    uint16_t num_neighbors = graph.arrays_lengths[v];

    if (has_self_loop(v, graph.adjacencies[v], num_neighbors)) {
      return true;
    }
  }

  return false;
}

// bool has_self_loop(uint16_t vertex, uint16_t *neighbors, uint16_t num_neighbors) {
//   for (uint16_t i = 0; i < num_neighbors; ++i) {
//     if (neighbors[i] == vertex) {
//       return true;
//     }
//   }
//   return false;
// }

// bool has_self_loop(uint16_t vertex, uint16_t *neighbors, uint16_t num_neighbors) {
//   // for simplicity, assume each vertex's num_neighbors is a multiple of 8
//   assert(num_neighbors % 8 == 0);

//   // Create a vector with 8 copies of the vertex ID
//   __m128i vertex_vec = _mm_set1_epi16(vertex);

//   for (uint16_t i = 0; i < num_neighbors; i += 8) {
//     __m128i neighbors_vec = _mm_loadu_si128((__m128i *)&neighbors[i]);

//     // Compare each 16-bit integer in neighbor_vec with vertex_vec
//     __m128i cmp_result = _mm_cmpeq_epi16(neighbors_vec, vertex_vec);

//     // Convert the comparison result to a bitmask
//     uint16_t mask = _mm_movemask_epi8(cmp_result);

//     if (mask != 0) {
//       return true;
//     }
//   }

//   return false;
// }

void free_graph(graph_t graph) {
  for (uint16_t i = 0; i < graph.num_vertices; ++i) {
    free(graph.adjacencies[i]);
  }
  free(graph.adjacencies);
  free(graph.arrays_lengths);
}