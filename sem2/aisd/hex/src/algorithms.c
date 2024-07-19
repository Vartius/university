#include "../include/algorithms.h"
#include "../include/commandHandler.h"
#include <stdio.h>

void swap(struct Pawn* a, struct Pawn* b)
{
	struct Pawn temp = *a;
	*a = *b;
	*b = temp;
}

int partition(struct Pawn* arr[], int low, int high)
{
	int pivot = arr[high]->distance;
	int i = low - 1;

	for(int j = low; j <= high - 1; j++)
	{
		if(arr[j]->distance < pivot)
		{
			i++;
			swap(arr[i], arr[j]);
		}
	}
	swap(arr[i + 1], arr[high]);
	return i + 1;
}

void quickSort(struct Pawn* arr[], int low, int high)
{
	if(low < high)
	{
		int pi = partition(arr, low, high);
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

bool aStar(struct Pawn* current, unsigned int border_size, bool isRevertDistance)
{
	current->is_checked = true;
	if(current->neighbours_number == 0)
	{
		return false;
	}
	else
	{
		// quickSort(current->neighbours, 0, current->neighbours_number - 1);
		if(isRevertDistance)
		{
			for(size_t i = 0; i < current->neighbours_number; i++)
			{
				if(current->neighbours[i]->is_checked)
				{
					continue;
				}
				if(current->neighbours[i]->distance == 0)
				{
					return true;
				}
				if(aStar(current->neighbours[i], border_size, isRevertDistance))
				{
					return true;
				}
			}
		}
		else
		{
			for(int i = current->neighbours_number - 1; i >= 0; i--)
			{
				if(current->neighbours[i]->is_checked)
				{
					continue;
				}
				if(current->neighbours[i]->distance == border_size - 1)
				{
					return true;
				}
				if(aStar(current->neighbours[i], border_size, isRevertDistance))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void resetChecked(struct Board* b)
{
	for(size_t i = 0; i < b->board_size; i++)
	{
		for(size_t j = 0; j < b->board_size; j++)
		{
			b->board[i][j].is_checked = false;
		}
	}
}

bool findNodes(struct Board* b, bool isRed)
{
	if(isRed)
	{
		for(size_t i = 0; i < b->board_size; i++)
		{
			for(size_t j = 0; j < b->board_size; j++)
			{
				if(b->board[i][j].color == 1)
				{
					resetChecked(b);
					b->board[i][j].is_checked = true;
					if(check(b, 1))
						return true;
					// b->board[i][j].is_checked = false;
				}
			}
		}
	}
	else
	{
		for(size_t i = 0; i < b->board_size; i++)
		{
			for(size_t j = 0; j < b->board_size; j++)
			{
				if(b->board[i][j].color == 2)
				{
					resetChecked(b);
					b->board[i][j].is_checked = true;
					if(check(b, 0))
					{
						return true;
					}
					// b->board[i][j].is_checked = false;
				}
			}
		}
	}
	return false;
}