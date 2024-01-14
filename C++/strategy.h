#ifndef STRATEGY_H
#define STRATEGY_H
#include "board.h"

const int search_depth = 5;

Move best_move(Board &position);
Move best_move_minimax(Board position);
short negamax(Board &position, int depth, short alpha, short beta, bool max_player);
short minimax(Board position, int depth, short alpha, short beta, bool max_player);
#endif