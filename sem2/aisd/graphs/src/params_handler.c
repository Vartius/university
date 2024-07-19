#include "../include/params_handler.h"
#include "../include/graph.h"
#include "../include/types.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

// the degree sequence;
ull* degree_sequence(struct Graph* g)
{
	ull* degrees = (ull*)malloc(g->n * sizeof(ull));
	ull* degrees_positions = (ull*)malloc(g->n * sizeof(ull));

	for(ull i = 0; i < g->n; i++)
	{
		degrees_positions[i] = i;
	}
	// printf("\n\n degrees: ");
	for(ull i = 0; i < g->n; i++)
	{
		degrees[i] = g->edges_sizes[i];
		// 	printf("%llu ", degrees[i]);
	}
	// printf("\n");
	heap_sort_reverse(degrees, g->n, degrees_positions);
	for(int i = 0; i < g->n; i++)
	{
		printf("%llu ", degrees[i]);
	}
	// printf("\n degrees_positions: ");
	// for(int i = 0; i < g->n; i++)
	// {
	// 	printf("%llu ", degrees_positions[i] + 1);
	// }
	printf("\n");
	free(degrees);
	return degrees_positions;
}

// the number of components;
void number_of_components(struct Graph* g)
{
	int* visited = (int*)malloc(g->n * sizeof(int));
	for(int i = 0; i < g->n; i++)
	{
		visited[i] = 0;
	}
	int components = 0;
	for(int i = 0; i < g->n; i++)
	{
		if(visited[i] == 0)
		{
			dfs(i, g, visited);
			components++;
		}
	}
	printf("%d\n", components);
	free(visited);
}

// bipartiteness;
void bipartiteness_dfs(int v, struct Graph* g, int* visited, int* colors, int* bipartite)
{
	visited[v] = 1;
	for(int i = 0; i < g->edges_sizes[v]; i++)
	{
		if(visited[g->edges[v][i] - 1] == 0)
		{
			colors[g->edges[v][i] - 1] = 1 - colors[v];
			bipartiteness_dfs(g->edges[v][i] - 1, g, visited, colors, bipartite);
		}
		else if(colors[g->edges[v][i] - 1] == colors[v])
		{
			*bipartite = 0;
		}
	}
}

void bipartiteness(struct Graph* g)
{
	int* visited = (int*)malloc(g->n * sizeof(int));
	int* colors = (int*)malloc(g->n * sizeof(int));
	int bipartite = 1;
	for(int i = 0; i < g->n; i++)
	{
		visited[i] = 0;
		colors[i] = -1;
	}
	for(int i = 0; i < g->n; i++)
	{
		if(visited[i] == 0)
		{
			bipartiteness_dfs(i, g, visited, colors, &bipartite);
		}
	}
	printf("%s\n", bipartite ? "T" : "F");
	free(visited);
	free(colors);
}

// the eccentricity of vertices (within the components)
void eccentricity_of_vertices(struct Graph* g)
{
	printf("?\n");
}

// planarity;
void planarity(struct Graph* g)
{
	printf("?\n");
}

void vertices_colors(struct Graph* g, ull* degrees_positions, int use_LF)
{
	int n = g->n;
	int* colors = (int*)malloc(n * sizeof(int));
	char* used = (char*)calloc(n, sizeof(char));

	for(int i = 0; i < n; i++)
	{
		colors[i] = -1;
	}

	for(int i = 0; i < n; i++)
	{
		int index = use_LF ? degrees_positions[i] : i;
		int edges_size = g->edges_sizes[index];
		ull* edges = g->edges[index];

		for(int j = 0; j < edges_size; j++)
		{
			int adj_vertex = edges[j] - 1;
			if(colors[adj_vertex] != -1)
			{
				used[colors[adj_vertex]] = 1;
			}
		}
		int c = 0;
		while(used[c])
		{
			c++;
		}
		colors[index] = c;

		for(int j = 0; j < edges_size; j++)
		{
			int adj_vertex = edges[j] - 1;
			if(colors[adj_vertex] != -1)
			{
				used[colors[adj_vertex]] = 0;
			}
		}
	}

	for(int i = 0; i < n; i++)
	{
		printf("%d ", colors[i] + 1);
	}
	printf("\n");

	free(colors);
	free(used);
}

// vertices colors obtained using SLF method (ties are solved by highest vertex degree, and if there is still a tie choose the vertex with the lowest number)
void vertices_colors_SLF(struct Graph* g, ull* degrees_positions)
{
	int n = g->n;
	int* colors = (int*)malloc(n * sizeof(int));
	int* unique_colors = (int*)malloc(n * sizeof(int));
	char* used = (char*)calloc(n, sizeof(char));
	int* saturation = (int*)calloc(n, sizeof(int));

	for(int i = 0; i < n; i++)
	{
		colors[i] = -1; // Initialize all colors to -1
	}

	for(int i = 0; i < n; i++)
	{
		int max_saturation = -1;
		int max_degree = -1;
		int vertex_to_color = -1;

		// Find the vertex with the highest saturation, and if tied, the highest degree
		for(int j = 0; j < n; j++)
		{
			int vertex = degrees_positions[j];
			if(colors[vertex] == -1)
			{ // Only consider uncolored vertices
				if(saturation[vertex] > max_saturation ||
				   (saturation[vertex] == max_saturation && g->edges_sizes[vertex] > max_degree))
				{
					max_saturation = saturation[vertex];
					max_degree = g->edges_sizes[vertex];
					vertex_to_color = vertex;
				}
			}
		}

		// Color the selected vertex with the smallest available color
		int edges_size = g->edges_sizes[vertex_to_color];
		ull* edges = g->edges[vertex_to_color];

		for(int j = 0; j < edges_size; j++)
		{
			int adj_vertex = edges[j] - 1;
			if(colors[adj_vertex] != -1)
			{
				used[colors[adj_vertex]] = 1;
			}
		}

		int color = 0;
		while(used[color])
		{
			color++;
		}
		colors[vertex_to_color] = color;

		// Reset used array for the next vertex
		for(int j = 0; j < edges_size; j++)
		{
			int adj_vertex = edges[j] - 1;
			if(colors[adj_vertex] != -1)
			{
				used[colors[adj_vertex]] = 0;
			}
		}

		// Update the saturation of adjacent vertices based on unique colors
		for(int j = 0; j < edges_size; j++)
		{
			int adj_vertex = edges[j] - 1;
			if(colors[adj_vertex] == -1)
			{
				int adj_edges_size = g->edges_sizes[adj_vertex];
				ull* adj_edges = g->edges[adj_vertex];

				for(int k = 0; k < n; k++)
				{
					unique_colors[k] = 0;
				}

				for(int k = 0; k < adj_edges_size; k++)
				{
					int adj_adj_vertex = adj_edges[k] - 1;
					if(colors[adj_adj_vertex] != -1)
					{
						unique_colors[colors[adj_adj_vertex]] = 1;
					}
				}

				int unique_saturation = 0;
				for(int k = 0; k < n; k++)
				{
					if(unique_colors[k])
					{
						unique_saturation++;
					}
				}
				saturation[adj_vertex] = unique_saturation;
			}
		}
	}

	// Print the colors
	for(int i = 0; i < n; i++)
	{
		printf("%d ", colors[i] + 1);
	}
	printf("\n");

	// Free allocated memory
	free(colors);
	free(used);
	free(saturation);
	free(unique_colors);
}

// the number of different C4 subgraphs
void number_of_different_C4_subgraphs(struct Graph* g)
{
	printf("?\n");
}
