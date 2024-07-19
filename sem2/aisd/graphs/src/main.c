#include "../include/graph.h"
#include "../include/params_handler.h"
#include "../include/types.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	ull count = 0;
	ull number_of_graph_complement_edges = 0;
	scanf("%llu", &count);
	for(int i = 0; i < count; i++)
	{
		struct Graph g;
		ull n;
		ull* degrees_positions;
		scanf("%llu", &n);
		number_of_graph_complement_edges = create_graph(n, &g);

		// print_graph(&g);
		degrees_positions = degree_sequence(&g);
		number_of_components(&g);
		bipartiteness(&g);
		eccentricity_of_vertices(&g);
		planarity(&g);
		vertices_colors(&g, degrees_positions, 0);
		vertices_colors(&g, degrees_positions, 1);
		// vertices_colors_greedy(&g);
		// vertices_colors_LF(&g, degrees_positions);
		vertices_colors_SLF(&g, degrees_positions);
		number_of_different_C4_subgraphs(&g);
		printf("%llu\n", number_of_graph_complement_edges);

		free(g.edges_sizes);
		for(int j = 0; j < g.n; j++)
		{
			free(g.edges[j]);
		}
	}
	return 0;
}
