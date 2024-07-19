#ifndef GRAPH_H
#define GRAPH_H

#include "types.h"

ull create_graph(ull n, struct Graph* g);
void print_graph(struct Graph* g);
void dfs(int v, struct Graph* g, int* visited);

#endif // GRAPH_H