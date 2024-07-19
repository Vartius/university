#ifndef TYPES_H
#define TYPES_H
#include <stdbool.h>

struct Pawn
{
	int color; // 0 - empty, 1 - red, 2 - blue
	int x;
	int y;
	int distance;
	int neighbours_number;
	bool is_checked;
	struct Pawn* neighbours[6];
};

struct Board
{
	struct Pawn board[11][11];
	unsigned int board_size;
	unsigned int pawns_number;
	unsigned int red_pawns_number;
	unsigned int blue_pawns_number;
	unsigned int red_pawns_on_top_border_number;
	unsigned int red_pawns_on_bottom_border_number;
	unsigned int blue_pawns_on_left_border_number;
	unsigned int blue_pawns_on_right_border_number;
	struct Pawn* red_pawns_on_top_border[11];
	struct Pawn* red_pawns_on_bottom_border[11];
	struct Pawn* blue_pawns_on_left_border[11];
	struct Pawn* blue_pawns_on_right_border[11];
};

struct free
{
	unsigned int count;
	struct Pawn* pawn[121];
};

#endif // TYPES_H