#define _CRT_DISABLE_PERFCRIT_LOCKS

#include "../include/algorithms.h"
#include "../include/board.h"
#include "../include/commandHandler.h"
#include "../include/types.h"
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char const* argv[])
{
	struct Board b = {.board = {{0}},
					  .board_size = 0,
					  .pawns_number = 0,
					  .red_pawns_number = 0,
					  .blue_pawns_number = 0,
					  .red_pawns_on_top_border_number = 0,
					  .red_pawns_on_bottom_border_number = 0,
					  .blue_pawns_on_left_border_number = 0,
					  .blue_pawns_on_right_border_number = 0,
					  .red_pawns_on_top_border = {0},
					  .red_pawns_on_bottom_border = {0},
					  .blue_pawns_on_left_border = {0},
					  .blue_pawns_on_right_border = {0}};

	bool isBoard = true;

	// true: count board size
	// false: handle commands

	char c;
	char prev = '\0';
	while((c = getchar()) != EOF)
	{
		if(!isBoard)
		{

			if(c == ' ' && prev == ' ')
			{
				isBoard = true;
				b.board_size = 2;
				continue;
			}
			else if(prev == ' ' && c == '-')
			{
				isBoard = true;
				b.board_size = 1;
				continue;
			}
			/* Commands
      + BOARD_SIZE 0
      + PAWNS_NUMBER 1
      + IS_BOARD_CORRECT 3
      + IS_GAME_OVER 2
      + IS_BOARD_POSSIBLE 4
      + CAN_RED_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT 5
      + CAN_BLUE_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT 9
      + CAN_RED_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT 7
      + CAN_BLUE_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT 11
      CAN_RED_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT 6
      CAN_BLUE_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT 10
      CAN_RED_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT 8
      CAN_BLUE_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT 12
      */
			prev = c;
			// print_board(b.board, b.board_size);
			if(c != ' ' && c != '\n')
			{
				switch(getCommand(c))
				{
				case -1:
					break;
				case 0:
					printf("%d\n", b.board_size);
					break;
				case 1:
					printf("%d\n", b.pawns_number);
					break;
				case 2:
					if(isBoardCorrect(&b.red_pawns_number, &b.blue_pawns_number))
						switch(isGameOver(&b))
						{
						case 0:
							printf("NO\n");
							break;
						case 1:
							printf("YES RED\n");
							break;
						case 2:
							printf("YES BLUE\n");
						}
					else
						printf("NO\n");
					break;
				case 3:
					if(isBoardCorrect(&b.red_pawns_number, &b.blue_pawns_number))
					{
						printf("YES\n");
					}
					else
					{
						printf("NO\n");
					}
					break;
				case 4:
					// printf("IS_BOARD_POSSIBLE\n");
					isBoardPossible(&b) ? printf("YES\n") : printf("NO\n");
					break;
				case 5:
					// printf("CAN_RED_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT\n");
					naiveHandler(&b, false, 1) ? printf("YES\n") : printf("NO\n");
					;
					break;
				case 6:
					// printf("CAN_RED_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT\n");
					break;
				case 7:
					naiveHandler(&b, true, 1) ? printf("YES\n") : printf("NO\n");
					resetChecked(&b);
					// printf("CAN_RED_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT\n");
					break;
				case 8:
					// printf("CAN_RED_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT\n");
					break;
				case 9:
					naiveHandler(&b, false, 2) ? printf("YES\n") : printf("NO\n");
					resetChecked(&b);
					// printf("CAN_BLUE_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT\n");
					break;
				case 10:
					// printf("CAN_BLUE_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT\n");
					break;
				case 11:
					naiveHandler(&b, true, 2) ? printf("YES\n") : printf("NO\n");
					resetChecked(&b);
					// printf("CAN_BLUE_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT\n");
					break;
				case 12:
					// printf("CAN_BLUE_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT\n");
					break;
				}
			}
		}
		else if(c == ' ')
		{
			b.board_size++;
		}
		else
		{
			b.board_size = (b.board_size + 2) / 3;
			updateBoard(&b);
			inputBoard(&b);
			// print_board(b.board, b.board_size);
			// printf(checkConnections(&b) ? "YES\n" : "NO\n");
			// return 0;
			isBoard = false;
		}
	}

	return 0;
}