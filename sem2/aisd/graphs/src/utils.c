#include "../include/utils.h"
#include "../include/types.h"

// heap sort reverse
void heapify_reverse(ull* arr, int n, int i, ull* positions)
{
	int smallest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	if(l < n &&
	   (arr[l] < arr[smallest] || (arr[l] == arr[smallest] && positions[l] > positions[smallest])))
	{
		smallest = l;
	}
	if(r < n &&
	   (arr[r] < arr[smallest] || (arr[r] == arr[smallest] && positions[r] > positions[smallest])))
	{
		smallest = r;
	}
	if(smallest != i)
	{
		int temp = arr[i];
		arr[i] = arr[smallest];
		arr[smallest] = temp;
		temp = positions[i];
		positions[i] = positions[smallest];
		positions[smallest] = temp;
		heapify_reverse(arr, n, smallest, positions);
	}
}

void heap_sort_reverse(ull* arr, int n, ull* positions)
{
	for(int i = n / 2 - 1; i >= 0; i--)
	{
		heapify_reverse(arr, n, i, positions);
	}
	for(int i = n - 1; i > 0; i--)
	{
		int temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;
		temp = positions[0];
		positions[0] = positions[i];
		positions[i] = temp;
		heapify_reverse(arr, i, 0, positions);
	}
}
// heap sort
void heapify(int* arr, int n, int i)
{
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	if(l < n && arr[l] > arr[largest])
	{
		largest = l;
	}
	if(r < n && arr[r] > arr[largest])
	{
		largest = r;
	}
	if(largest != i)
	{
		int temp = arr[i];
		arr[i] = arr[largest];
		arr[largest] = temp;
		heapify(arr, n, largest);
	}
}

void heap_sort(int* arr, int n)
{
	for(int i = n / 2 - 1; i >= 0; i--)
	{
		heapify(arr, n, i);
	}
	for(int i = n - 1; i > 0; i--)
	{
		int temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;
		heapify(arr, i, 0);
	}
}