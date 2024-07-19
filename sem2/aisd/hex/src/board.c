#include "../include/board.h"
#include <stdbool.h>
#include <stdio.h>

void print_board(struct Pawn board[11][11], unsigned int board_size)
{
	putchar('\n');
	for(size_t i = 0; i < board_size; i++)
	{
		for(size_t j = 0; j < board_size; j++)
		{
			switch(board[i][j].color)
			{
			case 0:
				putchar('.');
				break;
			case 1:
				putchar('r');
				break;
			case 2:
				putchar('b');
				break;
			}
			putchar(' ');
		}
		putchar('\n');
	}
}

void print_pawns_tree(struct Pawn* p, int x, int y)
{
	if(p == NULL)
	{
		return;
	}
	printf("x: %d, y: %d, color: %d\n", x, y, p->color);
	// tree
	for(size_t i = 0; i < 6; i++)
	{
		if(p->neighbours[i] != NULL)
		{
			printf("%d\n", p->neighbours[i]->distance);
		}
	}
}

void updateBoard(struct Board* b)
{
	b->pawns_number = 0;
	b->red_pawns_number = 0;
	b->blue_pawns_number = 0;
	b->red_pawns_on_top_border_number = 0;
	b->red_pawns_on_bottom_border_number = 0;
	b->blue_pawns_on_left_border_number = 0;
	b->blue_pawns_on_right_border_number = 0;
	for(int i = 0; i < 11; i++)
	{
		b->red_pawns_on_top_border[i] = NULL;
		b->red_pawns_on_bottom_border[i] = NULL;
		b->blue_pawns_on_left_border[i] = NULL;
		b->blue_pawns_on_right_border[i] = NULL;
	}
}

void deleteNeighbours(struct Pawn* p)
{
	for(size_t i = 0; i < p->neighbours_number; i++)
	{
		for(size_t j = 0; j < 6; j++)
		{
			if(p->neighbours[i]->neighbours[j] == p)
			{
				p->neighbours[i]->neighbours[j] = NULL;
				p->neighbours[i]->neighbours_number--;
				// reorder
				for(size_t k = j; k < 5; k++)
				{
					p->neighbours[i]->neighbours[k] = p->neighbours[i]->neighbours[k + 1];
				}
			}
		}
		p->neighbours[i] = NULL;
	}
	p->neighbours_number = 0;
}

void setNeighbours(struct Board* b, const int x, int y)
{
	if(x - 1 >= 0 && b->board[x - 1][y].color == b->board[x][y].color) // up right
	{
		b->board[x][y].neighbours[b->board[x][y].neighbours_number] = &b->board[x - 1][y];
		b->board[x - 1][y].neighbours[b->board[x - 1][y].neighbours_number] = &b->board[x][y];
		b->board[x][y].neighbours_number++;
		b->board[x - 1][y].neighbours_number++;
	}
	if(y - 1 >= 0 && b->board[x][y - 1].color == b->board[x][y].color) // up left
	{
		b->board[x][y].neighbours[b->board[x][y].neighbours_number] = &b->board[x][y - 1];
		b->board[x][y - 1].neighbours[b->board[x][y - 1].neighbours_number] = &b->board[x][y];
		b->board[x][y].neighbours_number++;
		b->board[x][y - 1].neighbours_number++;
	}
	if(y - 1 >= 0 && x - 1 >= 0 && b->board[x - 1][y - 1].color == b->board[x][y].color) // up
	{
		b->board[x][y].neighbours[b->board[x][y].neighbours_number] = &b->board[x - 1][y - 1];
		b->board[x - 1][y - 1].neighbours[b->board[x - 1][y - 1].neighbours_number] =
			&b->board[x][y];
		b->board[x][y].neighbours_number++;
		b->board[x - 1][y - 1].neighbours_number++;
	}
}

void setFullNeighbours(struct Board* b, const int x, int y)
{
	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			if(i == 0 && j == 0)
			{
				continue;
			}
			if(x + i < 0 || x + i >= b->board_size || y + j < 0 || y + j >= b->board_size ||
			   (i == -1 && j == 1) || (i == 1 && j == -1))
			{
				continue;
			}
			if(b->board[x + i][y + j].color == b->board[x][y].color)
			{
				b->board[x][y].neighbours[b->board[x][y].neighbours_number] =
					&b->board[x + i][y + j];
				b->board[x + i][y + j].neighbours[b->board[x + i][y + j].neighbours_number] =
					&b->board[x][y];
				b->board[x][y].neighbours_number++;
				b->board[x + i][y + j].neighbours_number++;
			}
		}
	}
}

void setPawn(struct Board* b, const int x, const int y, const char pawn)
{
	b->board[x][y].color = 0;
	b->board[x][y].distance = 0;
	b->board[x][y].neighbours_number = 0;
	b->board[x][y].is_checked = false;
	b->board[x][y].x = x;
	b->board[x][y].y = y;
	for(size_t i = 0; i < 6; i++)
	{
		b->board[x][y].neighbours[i] = NULL;
	}
	switch(pawn)
	{
	case 'r':
		b->board[x][y].color = 1;
		b->board[x][y].distance = x;
		setNeighbours(b, x, y);
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
		b->red_pawns_number++;
		break;
	case 'b':
		b->board[x][y].color = 2;
		b->board[x][y].distance = y;
		setNeighbours(b, x, y);
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
		b->blue_pawns_number++;
		break;
	default:
		b->board[x][y].color = 0;
		break;
	}
}

void inputBoard(struct Board* b)
{
	int x = 0;
	int y = 0;
	int k = 0;
	char c;
	for(size_t i = 0; i < b->board_size; i++)
	{
		y = k;
		x = 0;
		while(y >= 0)
		{
			while(1)
			{
				c = getchar();
				if('<' == c)
				{
					getchar(); // ignore space
					break;
				}
			}
			const char pawn = getchar();
			setPawn(b, x, y, pawn);
			x++;
			y--;
		}
		k++;
	}

	k = 1;
	for(size_t i = 0; i < b->board_size; i++)
	{
		y = b->board_size - 1;
		x = k;
		while(y >= 0 && x < b->board_size)
		{
			while(1)
			{
				c = getchar();
				if('<' == c)
				{
					getchar(); // ignore space
					break;
				}
			}
			const char pawn = getchar();
			setPawn(b, x, y, pawn);
			x++;
			y--;
		}
		k++;
	}
	while(getchar() != '\n')
		;
	while(getchar() != '\n')
		;
	b->pawns_number = b->red_pawns_number + b->blue_pawns_number;
}

bool checkConnections(struct Board* b)
{
	for(int i = 0; i < b->board_size; i++)
	{
		for(int j = 0; j < b->board_size; j++)
		{
			struct Pawn* shouldBe[6] = {0};
			int shouldBeSize = 0;
			if(b->board[i][j].color == 1)
			{
				for(int x = -1; x <= 1; x++)
				{
					for(int y = -1; y <= 1; y++)
					{
						if((x == 0 && y == 0) || (x == -1 && y == 1) || (x == 1 && y == -1))
						{
							continue;
						}
						// printf("i: %d, j: %d, x: %d, y: %d\n", i, j, x, y);
						if(i + x < 0 || i + x >= b->board_size || j + y < 0 ||
						   j + y >= b->board_size)
						{
							continue;
						}
						if(b->board[i + x][j + y].color == 1)
						{
							shouldBe[shouldBeSize] = &b->board[i + x][j + y];
							shouldBeSize++;
						}
					}
				}
			}
			else if(b->board[i][j].color == 2)
			{
				for(int x = -1; x <= 1; x++)
				{
					for(int y = -1; y <= 1; y++)
					{
						if((x == 0 && y == 0) || (x == -1 && y == 1) || (x == 1 && y == -1))
						{
							continue;
						}
						if(i + x < 0 || i + x >= b->board_size || j + y < 0 ||
						   j + y >= b->board_size)
						{
							continue;
						}
						if(b->board[i + x][j + y].color == 2)
						{
							shouldBe[shouldBeSize] = &b->board[i + x][j + y];
							shouldBeSize++;
						}
					}
				}
			}
			for(int k = 0; k < 6; k++)
			{
				for(int kk = 0; kk < 6; kk++)
				{
					if(b->board[i][j].neighbours[k] == shouldBe[kk])
					{
						break;
					}
					if(kk == 5)
					{
						printf("Pawn at (%d, %d) has wrong neighbours\n", i, j);
						printf("Expected neighbours:\n");
						for(int l = 0; l < 6; l++)
						{
							if(shouldBe[l] != NULL)
							{
								printf("(%d, %d)\n", shouldBe[l]->distance, shouldBe[l]->color);
							}
						}
						printf("Actual neighbours:\n");
						for(int l = 0; l < 6; l++)
						{
							if(b->board[i][j].neighbours[l] != NULL)
							{
								printf("(%d, %d)\n",
									   b->board[i][j].neighbours[l]->distance,
									   b->board[i][j].neighbours[l]->color);
							}
						}
						return false;
					}
				}
			}
		}
	}
	return true;
}

struct free freePlaces(struct Board* b)
{
	struct free f;
	f.count = 0;
	for(int i = 0; i < b->board_size; i++)
	{
		for(int j = 0; j < b->board_size; j++)
		{
			if(b->board[i][j].color == 0)
			{
				f.pawn[f.count] = &b->board[i][j];
				f.count++;
			}
		}
	}
	return f;
}