#include "graph.h"
#include "common.h"

#include <ctype.h> // for tolower


graph_t make_test_graph(bool should_have_self_loop) {
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

  if (should_have_self_loop) {
    graph.adjacencies[42042][7777] = 42042;
  }

  return graph;
}

bool prompt_boolean_question(const char *question) {
  char input;
  while (true) {
    printf("%s [y, n] ", question);
    scanf(" %c", &input);
    input = tolower(input);
    if (input != 'y' && input != 'n') {
      printf("invalid respond...\n");
    } else {
      return input == 'y';
    }
  }
}

int main(void) {
  bool should_have_self_loop = prompt_boolean_question("Should the graph have a self-loop?");
  bool should_use_pcmpistri = prompt_boolean_question("Should the algorithm use PCMPISTRI?");

  graph_t graph = make_test_graph(should_have_self_loop);

  START_TIMER(Algorithm);
  int algo_result = graph_has_self_loop(graph, should_use_pcmpistri);
  PRINT_TIMER(Algorithm);

  printf("Self-loop(s) %s\n", algo_result ? "DETECTED" : "NOT detected");
  
  free_graph(graph);
  return 0;
}
