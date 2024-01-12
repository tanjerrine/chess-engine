#ifndef STRATEGY_H
#define STRATEGY_H
#include "board.h"

const int search_depth = 2;

Move best_move(Board position);
float negamax(Board position, int depth, float alpha, float beta, bool max_player);
float minimax(Board position, int depth, float alpha, float beta, bool max_player);
#endif