#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "board.h"

int getCommand(char c);
int isGameOver(struct Board* b);
bool check(struct Board* b, bool isRed);
bool isBoardPossible(struct Board* b);
bool isBoardCorrect(const unsigned int* red_pawns, const unsigned int* blue_pawns);
bool naiveHandler(struct Board* b, bool isTwo, int color);
#endif // COMMAND_HANDLER_H