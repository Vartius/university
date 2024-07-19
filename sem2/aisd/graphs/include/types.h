#ifndef TYPES_H
#define TYPES_H

typedef unsigned long long ull;

struct Graph
{
	ull n;
	ull** edges;
	ull* edges_sizes;
};

#endif // TYPES_H