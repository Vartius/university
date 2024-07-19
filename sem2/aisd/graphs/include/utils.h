#ifndef UTILS_H
#define UTILS_H

#include "types.h"
void heapify(int* arr, int n, int i);
void heap_sort_reverse(ull* arr, int n, ull* positions);
void heapify_reverse(ull* arr, int n, int i, ull* positions);
void heap_sort(int* arr, int n);

#endif // UTILS_H