#ifndef PARAMS_HANDLER_H
#define PARAMS_HANDLER_H

#include "graph.h"

// the degree sequence;
ull* degree_sequence(struct Graph* g);

// the number of components;
void number_of_components(struct Graph* g);

// bipartiteness;
void bipartiteness(struct Graph* g);

// the eccentricity of vertices (within the components)
void eccentricity_of_vertices(struct Graph* g);

// planarity;
void planarity(struct Graph* g);

void vertices_colors(struct Graph* g, ull* degrees_positions, int use_LF);

// vertices colors obtained using SLF method (ties are solved by highest vertex degree, and if there is still a tie choose the vertex with the lowest number)
void vertices_colors_SLF(struct Graph* g, ull* degrees_positions);

// the number of different C4 subgraphs
void number_of_different_C4_subgraphs(struct Graph* g);

#endif // PARAMS_HANDLER_H