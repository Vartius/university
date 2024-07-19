#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "types.h"
void quickSort(struct Pawn* arr[], int low, int high);
bool aStar(struct Pawn* current, unsigned int border_size, bool isRevertDistance);
bool findNodes(struct Board* b, bool isRed);
void resetChecked(struct Board* b);
#endif // ALGORITHMS_H