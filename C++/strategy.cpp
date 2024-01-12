#include "strategy.h"
#include "utils.h"
#include "move.h"
#include <float.h>
#include <algorithm>

using std::vector; 
using std::max; using std::min;
using std::cout; using std::endl;

Move best_move(Board position) {
    enum_color turn = position.get_turn();
    float max_eval = -FLT_MAX;
    vector<Move> all_moves = position.get_legal_moves();
    Move best_move(0,0,0);
    for (Move move : all_moves) {
        Board board_copy(position);
        board_copy.make_move(move);
        // if (move.get_piece() != 0 || move.get_finish() != ((U64) 1 << 23)) continue;
        float eval = negamax(board_copy, search_depth - 1, -FLT_MAX, FLT_MAX, (turn != white));
        if (eval > max_eval) {
            max_eval = eval;
            best_move = move;
            cout << "Move: " << move.get_notation() << " has eval " << eval << endl;
        }
        // alpha = max(alpha, eval);
        // if (beta <= alpha) {
        //     break;
        // }
    }
    return best_move;
}

float negamax(Board position, int depth, float alpha, float beta, bool max_player) {
    if (depth == 0 || position.game_over()) {
        float eval = position.get_eval();
        if (eval == 3 || eval == -5) {cout << max_player << "sym eval: " << ((2 * (int)max_player - 1) * position.get_eval()) << ", position: " << endl; position.display();}
        return (2 * (int)max_player - 1) * position.get_eval();
    }
    enum_color turn = position.get_turn();
    float max_eval = -FLT_MAX;
    for (Move cand_move : position.get_pseudo_legal_moves()) {
        Board board_copy(position);
        board_copy.make_move(cand_move);
        if (!board_copy.in_check(turn)) {
            float eval = negamax(board_copy, depth - 1, -beta, -alpha, !max_player);
            max_eval = max(max_eval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
    }
    return max_eval;
}

float minimax(Board position, int depth, float alpha, float beta, bool max_player) {
    if (depth == 0 || position.game_over()) {
        return position.get_eval();
    }
    enum_color turn = position.get_turn();
    if (max_player) {
        float max_eval = -FLT_MAX;
        for (Move cand_move : position.get_pseudo_legal_moves()) {
            Board board_copy(position);
            board_copy.make_move(cand_move);
            if (!board_copy.in_check(turn)) {
                float eval = minimax(board_copy, depth - 1, alpha, beta, false);
                max_eval = max(max_eval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return max_eval;
    }
    else {
        float min_eval = FLT_MAX;
        for (Move cand_move : position.get_pseudo_legal_moves()) {
            Board board_copy(position);
            board_copy.make_move(cand_move);
            if (!board_copy.in_check(turn)) {
                float eval = minimax(board_copy, depth - 1, alpha, beta, true);
                min_eval = min(min_eval, eval);
                beta = min(beta, eval);
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return min_eval;
    }
}