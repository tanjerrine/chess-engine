#include "strategy.h"
#include "utils.h"
#include "move.h"
#include <limits.h>
#include <algorithm>

using std::vector; 
using std::max; using std::min;
using std::cout; using std::endl;
using std::sort; using std::greater;

Move best_move(Board &position) {
    enum_color turn = position.get_turn();
    short max_eval = -SHRT_MAX;
    vector<Move> all_moves;
    position.get_legal_moves(all_moves);
    sort(all_moves.begin(), all_moves.end(), greater<Move>());
    Move best_move(0,0,0);
    for (Move move : all_moves) {
        position.make_move(move);
        // if (move.get_piece() != 0 || move.get_finish() != ((U64) 1 << 23)) continue;
        short eval = -negamax(position, search_depth - 1, -SHRT_MAX, -max_eval, (turn != white));
        position.unmake_move(move);
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

short negamax(Board &position, int depth, short alpha, short beta, bool max_player) {
    if (depth == 0 || position.game_over()) {
        // float eval = position.get_eval();
        // if (eval == 3 || eval == -5) {cout << max_player << "sym eval: " << ((2 * (int)max_player - 1) * position.get_eval()) << ", position: " << endl; position.display();}
        return (2 * (int)max_player - 1) * position.get_eval();
    }
    short max_eval = -SHRT_MAX;
    vector<Move> p_legal_moves;
    position.get_pseudo_legal_moves(p_legal_moves);
    enum_color moving_color = position.get_turn();
    sort(p_legal_moves.begin(), p_legal_moves.end(), greater<Move>());
    for (Move move : p_legal_moves) {
        position.make_move(move);
        if (!position.in_check(moving_color)) {
            short eval = -negamax(position, depth - 1, -beta, -alpha, !max_player);
            position.unmake_move(move);
            max_eval = max(max_eval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        else {
            position.unmake_move(move);
        }
    }
    return max_eval;
}


Move best_move_minimax(Board position) {
    enum_color turn = position.get_turn();
    if (turn == white) {
        short max_eval = -SHRT_MAX;
        vector<Move> all_moves;
        position.get_legal_moves(all_moves);
        sort(all_moves.begin(), all_moves.end(), greater<Move>());
        Move best_move(0,0,0);
        for (Move move : all_moves) {
            Board board_copy(position);
            board_copy.make_move(move);
            short eval = minimax(board_copy, search_depth - 1, -SHRT_MAX, SHRT_MAX, black);
            if (eval > max_eval) {
                max_eval = eval;
                best_move = move;
                cout << "Move: " << move.get_notation() << " has eval " << eval << endl;
            }
        }
        return best_move;
    }
    else {
        short min_eval = SHRT_MAX;
        vector<Move> all_moves;
        position.get_legal_moves(all_moves);
        sort(all_moves.begin(), all_moves.end(), greater<Move>());
        Move best_move(0,0,0);
        for (Move move : all_moves) {
            Board board_copy(position);
            board_copy.make_move(move);
            short eval = minimax(board_copy, search_depth - 1, -SHRT_MAX, SHRT_MAX, white);
            if (eval < min_eval) {
                min_eval = eval;
                best_move = move;
                cout << "Move: " << move.get_notation() << " has eval " << eval << endl;
            }
        }
        return best_move;
    }
}

short minimax(Board position, int depth, short alpha, short beta, bool max_player) {
    if (depth == 0 || position.game_over()) {
        return position.get_eval();
    }
    enum_color turn = position.get_turn();
    if (max_player) {
        short max_eval = -SHRT_MAX;
        vector<Move> p_legal_moves;
        position.get_pseudo_legal_moves(p_legal_moves);
        sort(p_legal_moves.begin(), p_legal_moves.end(), greater<Move>());
        for (Move cand_move : p_legal_moves) {
            Board board_copy(position);
            board_copy.make_move(cand_move);
            if (!board_copy.in_check(turn)) {
                short eval = minimax(board_copy, depth - 1, alpha, beta, false);
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
        short min_eval = SHRT_MAX;
        vector<Move> p_legal_moves;
        position.get_pseudo_legal_moves(p_legal_moves);
        sort(p_legal_moves.begin(), p_legal_moves.end(), greater<Move>());
        for (Move cand_move : p_legal_moves) {
            Board board_copy(position);
            board_copy.make_move(cand_move);
            if (!board_copy.in_check(turn)) {
                short eval = minimax(board_copy, depth - 1, alpha, beta, true);
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