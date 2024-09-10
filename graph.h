#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
  unsigned short **adjacencies;
  int num_vertices;
  int *arrays_lengths;
} graph_t;

int has_self_loop(graph_t graph);

#endif

