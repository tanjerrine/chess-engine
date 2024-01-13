#ifndef STRATEGY_H
#define STRATEGY_H
#include "board.h"

const int search_depth = 5;

Move best_move(Board &position);
Move best_move_minimax(Board position);
float negamax(Board &position, int depth, float alpha, float beta, bool max_player);
float minimax(Board position, int depth, float alpha, float beta, bool max_player);
#endif