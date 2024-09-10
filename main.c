#include "graph.h"
#include "common.h"

graph_t make_test_graph() {
  const int NUM_VERTICES = 65537;
  const int NUM_NEIGHBORS_EACH = 65536;

  graph_t graph = {
    .adjacencies = malloc(NUM_VERTICES * sizeof(unsigned short *)),
    .num_vertices = NUM_VERTICES,
    .arrays_lengths = malloc(NUM_VERTICES * sizeof(int))
  };

  if (!graph.adjacencies || !graph.arrays_lengths) {
    PANIC("malloc failed\n");
  }

  for (int i = 0; i < NUM_VERTICES; ++i) {
    if (!(graph.adjacencies[i] = malloc(NUM_NEIGHBORS_EACH * sizeof(unsigned short)))) {
      PANIC("malloc failed\n");
    }
    graph.arrays_lengths[i] = NUM_NEIGHBORS_EACH;
    for (int j = 0; j < NUM_NEIGHBORS_EACH; ++j) {
      graph.adjacencies[i][j] = (i + j + 1) % NUM_VERTICES;
    }
  }

  // make last neighbor of last vertex a self loop
  // graph.adjacencies[NUM_VERTICES - 1][NUM_NEIGHBORS_EACH - 1] = NUM_VERTICES - 1;

  return graph;
}

int main(void) {
  graph_t graph = make_test_graph();

  START_TIMER(work);
  int has_self_loop = graph_has_self_loop(graph);
  PRINT_TIMER(work);

  printf("graph has self-loop(s) ? %s\n", has_self_loop ? "yes" : "no");
  
  free_graph(graph);
  return 0;
}
