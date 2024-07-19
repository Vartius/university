#include "../include/commandHandler.h"
#include "../include/algorithms.h"
#include "../include/board.h"
#include "../include/types.h"
#include <stdbool.h>
#include <stdio.h>

int getCommand(char c)
{
	char c1;
	int result = -1;
	switch(c)
	{
	case 'B': // BOARD_SIZE
		result = 0;
		for(size_t i = 0; i < 9; i++)
		{
			c1 = getchar();
		}
		break;
	case 'P': // PAWNS_NUMBER
		result = 1;
		for(size_t i = 0; i < 11; i++)
		{
			c1 = getchar();
		}
		break;
	case 'I': // IS_BOARD_CORRECT, IS_GAME_OVER, IS_BOARD_POSSIBLE
		getchar();
		getchar();
		c = getchar();
		switch(c)
		{
		case 'G': // IS_GAME_OVER
			result = 2;
			for(size_t i = 0; i < 8; i++)
			{
				c1 = getchar();
			}
			break;
		default:
			for(size_t i = 0; i < 5; i++)
			{
				getchar();
			}
			c = getchar();
			switch(c)
			{
			case 'C': // IS_BOARD_CORRECT
				result = 3;
				for(size_t i = 0; i < 6; i++)
				{
					c1 = getchar();
				}
				break;
			case 'P': // IS_BOARD_POSSIBLE
				result = 4;
				for(size_t i = 0; i < 7; i++)
				{
					c1 = getchar();
				}
				break;
			}
		}
		break;
	case 'C': // CAN_*_WIN_IN_*_MOVE_WITH_*_OPPONENT
		getchar();
		getchar();
		getchar();
		c = getchar();
		switch(c)
		{
		case 'R': // CAN_RED_WIN_IN_*_MOVE_WITH_*_OPPONENT
			for(size_t i = 0; i < 10; i++)
			{
				getchar();
			}
			c = getchar();
			switch(c)
			{
			case '1': // CAN_RED_WIN_IN_1_MOVE_WITH_*_OPPONENT
				for(size_t i = 0; i < 11; i++)
				{
					getchar();
				}
				c = getchar();
				switch(c)
				{
				case 'N': // CAN_RED_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT
					result = 5;
					for(size_t i = 0; i < 13; i++)
					{
						getchar();
					}
					break;
				default: // CAN_RED_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT
					result = 6;
					for(size_t i = 0; i < 15; i++)
					{
						getchar();
					}
				}
				break;
			default: // CAN_RED_WIN_IN_2_MOVES_WITH_*_OPPONENT
				for(size_t i = 0; i < 12; i++)
				{
					getchar();
				}
				c = getchar();
				switch(c)
				{
				case 'N': // CAN_RED_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT
					result = 7;
					for(size_t i = 0; i < 13; i++)
					{
						getchar();
					}
					break;
				default: // CAN_RED_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT
					for(size_t i = 0; i < 15; i++)
					{
						getchar();
					}
					result = 8;
				}
			}
			break;
		default: // CAN_BLUE_WIN_IN_*_MOVE_WITH_*_OPPONENT
			for(size_t i = 0; i < 11; i++)
			{
				getchar();
			}
			c = getchar();
			switch(c)
			{
			case '1': // CAN_BLUE_WIN_IN_1_MOVE_WITH_*_OPPONENT
				for(size_t i = 0; i < 11; i++)
				{
					getchar();
				}
				c = getchar();
				switch(c)
				{
				case 'N': // CAN_BLUE_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT
					result = 9;
					for(size_t i = 0; i < 13; i++)
					{
						getchar();
					}
					break;
				default: // CAN_BLUE_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT
					for(size_t i = 0; i < 15; i++)
					{
						getchar();
					}
					result = 10;
				}
				break;
			default: // CAN_BLUE_WIN_IN_2_MOVES_WITH_*_OPPONENT
				for(size_t i = 0; i < 12; i++)
				{
					getchar();
				}
				c = getchar();
				switch(c)
				{
				case 'N': // CAN_BLUE_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT
					for(size_t i = 0; i < 13; i++)
					{
						getchar();
					}
					result = 11;
					break;
				default: // CAN_BLUE_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT
					for(size_t i = 0; i < 15; i++)
					{
						getchar();
					}
					result = 12;
				}
			}
		}
	}
	return result;
}

int isGameOver(struct Board* b)
{
	unsigned int win = 0; // 0 - no win, 1 - red win, 2 - blue win
	if(b->red_pawns_on_bottom_border_number < b->red_pawns_on_top_border_number)
	{
		for(size_t i = 0; i < b->red_pawns_on_bottom_border_number; i++)
		{
			if(aStar(b->red_pawns_on_bottom_border[i], b->board_size, true))
			{
				win = 1;
				break;
			}
		}
	}
	else
	{
		for(size_t i = 0; i < b->red_pawns_on_top_border_number; i++)
		{
			if(aStar(b->red_pawns_on_top_border[i], b->board_size, false))
			{
				win = 1;
				break;
			}
		}
	}

	if(win != 1)
	{
		if(b->blue_pawns_on_right_border_number < b->blue_pawns_on_left_border_number)
		{
			for(size_t i = 0; i < b->blue_pawns_on_right_border_number; i++)
			{
				if(aStar(b->blue_pawns_on_right_border[i], b->board_size, true))
				{
					win = 2;
					break;
				}
			}
		}
		else
		{
			for(size_t i = 0; i < b->blue_pawns_on_left_border_number; i++)
			{
				if(aStar(b->blue_pawns_on_left_border[i], b->board_size, false))
				{
					win = 2;
					break;
				}
			}
		}
	}
	if(b->red_pawns_number == 1 && b->board_size == 1)
	{
		win = 1;
	}
	return win;
}

bool check(struct Board* b, bool isRed)
{
	if(isRed)
	{
		if(b->red_pawns_on_bottom_border_number < b->red_pawns_on_top_border_number)
		{
			for(size_t i = 0; i < b->red_pawns_on_bottom_border_number; i++)
			{
				if(!b->red_pawns_on_bottom_border[i]->is_checked)
					if(aStar(b->red_pawns_on_bottom_border[i], b->board_size, true))
					{
						return false;
					}
			}
		}
		else
		{
			for(size_t i = 0; i < b->red_pawns_on_top_border_number; i++)
			{
				if(!b->red_pawns_on_top_border[i]->is_checked)
					if(aStar(b->red_pawns_on_top_border[i], b->board_size, false))
					{
						return false;
					}
			}
		}
	}
	else
	{
		if(b->blue_pawns_on_right_border_number < b->blue_pawns_on_left_border_number)
		{
			for(size_t i = 0; i < b->blue_pawns_on_right_border_number; i++)
			{
				if(!b->blue_pawns_on_right_border[i]->is_checked)
					if(aStar(b->blue_pawns_on_right_border[i], b->board_size, true))
					{
						return false;
					}
			}
		}
		else
		{
			for(size_t i = 0; i < b->blue_pawns_on_left_border_number; i++)
			{
				if(!b->blue_pawns_on_left_border[i]->is_checked)
					if(aStar(b->blue_pawns_on_left_border[i], b->board_size, false))
					{
						return false;
					}
			}
		}
	}

	return true;
}

bool isBoardCorrect(const unsigned int* red_pawns, const unsigned int* blue_pawns)
{
	if(*red_pawns == *blue_pawns || *red_pawns == *blue_pawns + 1)
	{
		return true;
	}
	return false;
}

bool isBoardPossible(struct Board* b)
{
	if(isBoardCorrect(&b->red_pawns_number, &b->blue_pawns_number))
	{
		switch(isGameOver(b))
		{
		case 0:
			return true;
			break;
		case 1:
			if(b->red_pawns_number == b->blue_pawns_number + 1)
			{
				if(findNodes(b, true))
					return true;
				else
					return false;
			}
			else
			{
				return false;
			}
			break;
		case 2:
			if(b->red_pawns_number == b->blue_pawns_number)
			{
				if(findNodes(b, false))
					return true;
				else
					return false;
			}
			else
			{
				return false;
			}
			break;
		}
	}
	return false;
}

void addPawn(struct Board* b, int x, int y, int color)
{
	b->board[x][y].color = color;
	b->board[x][y].x = x;
	b->board[x][y].y = y;
	if(color == 1)
	{
		b->red_pawns_number++;
		b->board[x][y].distance = x;

		if(x == 0)
		{
			b->red_pawns_on_top_border[b->red_pawns_on_top_border_number] = &b->board[x][y];
			b->red_pawns_on_top_border_number++;
		}
		else if(x == b->board_size - 1)
		{
			b->red_pawns_on_bottom_border[b->red_pawns_on_bottom_border_number] = &b->board[x][y];
			b->red_pawns_on_bottom_border_number++;
		}
	}
	else
	{
		b->blue_pawns_number++;
		b->board[x][y].distance = y;

		if(y == 0)
		{
			b->blue_pawns_on_left_border[b->blue_pawns_on_left_border_number] = &b->board[x][y];
			b->blue_pawns_on_left_border_number++;
		}
		else if(y == b->board_size - 1)
		{
			b->blue_pawns_on_right_border[b->blue_pawns_on_right_border_number] = &b->board[x][y];
			b->blue_pawns_on_right_border_number++;
		}
	}

	setFullNeighbours(b, x, y);
}

void removePawn(struct Board* b, int x, int y)
{
	if(b->board[x][y].color == 1)
	{
		b->red_pawns_number--;
		if(x == 0)
		{
			b->red_pawns_on_top_border_number--;
			for(size_t i = 0; i < b->red_pawns_on_top_border_number; i++)
			{
				if(b->red_pawns_on_top_border[i]->x == x && b->red_pawns_on_top_border[i]->y == y)
				{
					b->red_pawns_on_top_border[i] = NULL;
					// reorder
					for(size_t j = i; j < b->red_pawns_on_top_border_number; j++)
					{
						b->red_pawns_on_top_border[j] = b->red_pawns_on_top_border[j + 1];
					}
					break;
				}
			}
		}
		else if(x == b->board_size - 1)
		{
			b->red_pawns_on_bottom_border_number--;
			for(size_t i = 0; i < b->red_pawns_on_bottom_border_number; i++)
			{
				if(b->red_pawns_on_bottom_border[i]->x == x &&
				   b->red_pawns_on_bottom_border[i]->y == y)
				{
					b->red_pawns_on_bottom_border[i] = NULL;
					// reorder
					for(size_t j = i; j < b->red_pawns_on_bottom_border_number; j++)
					{
						b->red_pawns_on_bottom_border[j] = b->red_pawns_on_bottom_border[j + 1];
					}
					break;
				}
			}
		}
	}
	else
	{
		b->blue_pawns_number--;
		if(y == 0)
		{
			b->blue_pawns_on_left_border_number--;
			for(size_t i = 0; i < b->blue_pawns_on_left_border_number; i++)
			{
				if(b->blue_pawns_on_left_border[i]->x == x &&
				   b->blue_pawns_on_left_border[i]->y == y)
				{
					b->blue_pawns_on_left_border[i] = NULL;
					// reorder
					for(size_t j = i; j < b->blue_pawns_on_left_border_number; j++)
					{
						b->blue_pawns_on_left_border[j] = b->blue_pawns_on_left_border[j + 1];
					}
					break;
				}
			}
		}
		else if(y == b->board_size - 1)
		{
			b->blue_pawns_on_right_border_number--;
			for(size_t i = 0; i < b->blue_pawns_on_right_border_number; i++)
			{
				if(b->blue_pawns_on_right_border[i]->x == x &&
				   b->blue_pawns_on_right_border[i]->y == y)
				{
					b->blue_pawns_on_right_border[i] = NULL;
					// reorder
					for(size_t j = i; j < b->blue_pawns_on_right_border_number; j++)
					{
						b->blue_pawns_on_right_border[j] = b->blue_pawns_on_right_border[j + 1];
					}
					break;
				}
			}
		}
	}

	b->board[x][y].color = 0;
	deleteNeighbours(&b->board[x][y]);
}

bool checkIfTwoSteps(struct Board* b, int x1, int y1, int x2, int y2, int color)
{
	removePawn(b, x1, y1);
	// print_board(b->board, b->board_size);
	resetChecked(b);
	if(isGameOver(b) != color)
	{
		addPawn(b, x1, y1, color);
		return true;
	}
	addPawn(b, x1, y1, color);
	removePawn(b, x2, y2);
	// print_board(b->board, b->board_size);
	resetChecked(b);
	if(isGameOver(b) != color)
	{
		addPawn(b, x2, y2, color);
		return true;
	}
	addPawn(b, x2, y2, color);
	return false;
}

bool naiveHandler(struct Board* b, bool isTwo, int color)
{
	if(!isBoardPossible(b))
	{
		return false;
	}
	resetChecked(b);
	if(isGameOver(b) != 0)
	{
		return false;
	}
	struct free f = freePlaces(b);
	if(isTwo)
	{
		if((b->blue_pawns_number == b->red_pawns_number ? color + 2 : !(color - 1) + 3) <= f.count)
		{
			for(size_t j = 0; j < f.count; j++)
			{
				addPawn(b, f.pawn[j]->x, f.pawn[j]->y, color);
				for(size_t i = 0; i < f.count; i++)
				{
					if(i == j)
					{
						continue;
					}
					resetChecked(b);
					addPawn(b, f.pawn[i]->x, f.pawn[i]->y, color);
					// print_board(b->board, b->board_size);
					if(isGameOver(b) == color)
					{
						if(checkIfTwoSteps(
							   b, f.pawn[j]->x, f.pawn[j]->y, f.pawn[i]->x, f.pawn[i]->y, color))
						{
							removePawn(b, f.pawn[i]->x, f.pawn[i]->y);
							removePawn(b, f.pawn[j]->x, f.pawn[j]->y);
							return true;
						}
					}
					removePawn(b, f.pawn[i]->x, f.pawn[i]->y);
				}
				removePawn(b, f.pawn[j]->x, f.pawn[j]->y);
			}
		}
	}
	else
	{
		if((b->blue_pawns_number == b->red_pawns_number ? color : !(color - 1) + 1) <= f.count)
		{
			for(size_t i = 0; i < f.count; i++)
			{
				resetChecked(b);
				addPawn(b, f.pawn[i]->x, f.pawn[i]->y, color);
				// print_board(b->board, b->board_size);
				if(isGameOver(b) == color)
				{
					removePawn(b, f.pawn[i]->x, f.pawn[i]->y);
					return true;
				}
				removePawn(b, f.pawn[i]->x, f.pawn[i]->y);
			}
		}
	}
	return false;
}