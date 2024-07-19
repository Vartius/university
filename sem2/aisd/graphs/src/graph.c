#include "../include/graph.h"
#include <stdio.h>
#include <stdlib.h>

ull create_graph(ull n, struct Graph* g)
{
	// create a graph with n vertices and return the number of the graph complement's edges
	ull count = 0;
	g->n = n;
	g->edges = (ull**)malloc((g->n * sizeof(ull*)));
	g->edges_sizes = (ull*)malloc(g->n * sizeof(ull));

	for(ull i = 0; i < g->n; i++)
	{
		scanf("%llu", &g->edges_sizes[i]);
		g->edges[i] = (ull*)malloc(g->edges_sizes[i] * sizeof(ull));
		for(ull j = 0; j < g->edges_sizes[i]; j++)
		{
			scanf("%llu", &g->edges[i][j]);
			if(i < g->edges[i][j] - 1)
			{
				count++;
			}
		}
	}
	count = n * (n - 1) / 2 - count;
	return count;
}

void print_graph(struct Graph* g)
{
	for(ull i = 0; i < g->n; i++)
	{
		printf("%llu: ", i);
		for(int j = 0; j < g->edges_sizes[i]; j++)
		{
			printf("%llu ", g->edges[i][j]);
		}
		printf("\n");
	}
}

void dfs(int v, struct Graph* g, int* visited)
{
	visited[v] = 1;
	for(int i = 0; i < g->edges_sizes[v]; i++)
	{
		if(visited[g->edges[v][i] - 1] == 0)
		{
			dfs(g->edges[v][i] - 1, g, visited);
		}
	}
}

// 20
// 12 2 3 4 5 6 7 8 10 11 14 15 16
// 11 1 4 5 9 11 12 13 15 16 18 19
// 14 1 4 6 7 8 9 10 12 13 14 15 16 17 19
// 11 1 2 3 6 7 8 12 13 15 17 20
// 10 1 2 6 9 11 15 16 17 18 20
// 15 1 3 4 5 7 8 9 10 11 13 14 15 16 18 20
// 11 1 3 4 6 8 10 13 14 16 17 20
// 15 1 3 4 6 7 9 10 12 13 14 15 16 17 19 20
// 12 2 3 5 6 8 10 11 12 13 14 15 16
// 12 1 3 6 7 8 9 12 15 16 17 18 20
// 12 1 2 5 6 9 12 13 14 15 17 19 20
// 11 2 3 4 8 9 10 11 13 16 18 20
// 15 2 3 4 6 7 8 9 11 12 14 16 17 18 19 20
// 13 1 3 6 7 8 9 11 13 15 16 17 18 20
// 14 1 2 3 4 5 6 8 9 10 11 14 16 17 19
// 16 1 2 3 5 6 7 8 9 10 12 13 14 15 17 18 19
// 13 3 4 5 7 8 10 11 13 14 15 16 19 20
// 9 2 5 6 10 12 13 14 16 20
// 9 2 3 8 11 13 15 16 17 20
// 13 4 5 6 7 8 10 11 12 13 14 17 18 19