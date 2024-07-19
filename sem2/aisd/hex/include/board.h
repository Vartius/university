#ifndef BOARD_H
#define BOARD_H

#include "types.h"
#include <stdbool.h>

void print_board(struct Pawn board[11][11], unsigned int board_size);
void print_pawns_tree(struct Pawn* p, int x, int y);
void updateBoard(struct Board* b);
void setNeighbours(struct Board* b, const int x, int y);
void setPawn(struct Board* b, const int x, const int y, const char pawn);
void inputBoard(struct Board* b);
bool checkConnections(struct Board* b);
struct free freePlaces(struct Board* b);
void deleteNeighbours(struct Pawn* p);
void setFullNeighbours(struct Board* b, const int x, int y);

#endif // BOARD_H