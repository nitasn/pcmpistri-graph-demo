#include "graph.h"
#include "common.h"

#include <ctype.h> // for tolower

// initialize a dummy big graph that doesn't have self-loops
graph_t make_test_graph() {
  const int NUM_VERTICES = 65000;
  const int NUM_NEIGHBORS_EACH = 64000;

  graph_t graph = {
    .adjacencies = malloc(NUM_VERTICES * sizeof(unsigned short *)),
    .num_vertices = NUM_VERTICES,
    .arrays_lengths = malloc(NUM_VERTICES * sizeof(int))
  };

  if (!graph.adjacencies || !graph.arrays_lengths) {
    PANIC("malloc failed\n");
  }

  for (int v = 0; v < NUM_VERTICES; ++v) {
    if (!(graph.adjacencies[v] = malloc(NUM_NEIGHBORS_EACH * sizeof(unsigned short)))) {
      PANIC("malloc failed\n");
    }
    graph.arrays_lengths[v] = NUM_NEIGHBORS_EACH;
    for (int n = 0; n < NUM_NEIGHBORS_EACH; ++n) {
      graph.adjacencies[v][n] = ((v + n) % NUM_VERTICES) + 1;
    }
  }

  return graph;
}

bool prompt_boolean_question(const char *question) {
  char input;
  while (true) {
    printf("%s\n", question);
    printf("[y, n] > ");
    scanf(" %c", &input);
    printf("\n");
    input = tolower(input);
    if (input != 'y' && input != 'n') {
      printf("invalid respond...\n");
    } else {
      return input == 'y';
    }
  }
}

int main(void) {
  printf("Initializing graph without self-loops... \n\n");
  graph_t graph = make_test_graph();

  while (true) {
    printf(
      "Choose: \n"
      "[1] Make a new self-loop in the graph \n"
      "[2] Run a self-loop detection algorithm \n"
      "[3] Exit \n"
      " > "
    );

    char input;
    scanf(" %c", &input);
    printf("\n");

    if (input == '1') {
      printf("Insert node-id and neighbor-index: \n");

      int node_id, neighbor_index;
      scanf(" %d %d", &node_id, &neighbor_index);
      printf("\n");

      graph.adjacencies[node_id][neighbor_index] = node_id;
      printf("Done. Now node %d's %d'th neighbor is node %d itself. \n\n", node_id, neighbor_index, node_id);
    }

    else if (input == '2') {
      bool should_use_pcmpistri = prompt_boolean_question("Should the algorithm use PCMPISTRI?");

      START_TIMER(Algorithm);
      bool algo_result = graph_has_self_loop(graph, should_use_pcmpistri);
      PRINT_TIMER(Algorithm);

      printf("Self-loop(s) %s\n\n", algo_result ? "DETECTED" : "NOT detected");
    }

    else if (input == '3') {
      printf("Existing... \n");
      break;
    }

    else {
      printf("Invalid option. \n\n");
    }
  }

  free_graph(graph);
  return 0;
}
