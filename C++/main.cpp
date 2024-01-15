#include "move.h"
#include "board.h"
#include "strategy.h"
#include <iostream>
#include <algorithm>

using std::cout; using std::endl; using std::vector;
using std::sort; using std::string;

void init_first_rank_atks() {
    for (U64 occ = 0; occ < 128; occ+=2) {
        cout << "{";
        for (U64 rook = 1; rook < 256; rook<<=1) {
            U8 atks = line_atks((occ | rook), rook);
            cout << "0x" << std::hex << (int)(atks & 0xFF); 
            if (rook != 128) cout << ", ";
        }
        cout << "}";
        if (occ != 126) cout << ", ";
        if (occ % 8 == 6) cout << endl;
    }
}

void verify_rook_atks() {
    for (U64 occ = 0; occ < 256; occ++) {
        for (U64 rook = 1; rook < 256; rook<<=1) {
            if (rook & occ) {
                U8 atks = line_atks(occ, rook);
                U8 pre = rook_first_rank_attacks[(occ >> 1) & 63][bit_scan(rook)];
                if (atks != pre) {
                    cout << "ERROR! line_atks: " << "0x" << std::hex << (int)(atks & 0xFF) << " does not equal precomputed: " "0x" << std::hex << (int)(pre & 0xFF) << endl;
                    cout << "rook: " << endl; print_bb(rook);
                    cout << "occ: " << endl; print_bb(occ);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    cout << "SUCCESS! ROOK MOVES MATCH LINE_ATKS FUNCTION" << endl;
}

void test_illegal_moves() {
    // Board b("5rk1/6p1/1b4q1/8/8/4B3/6N1/4R1K1 w - - 0 1");
    Board b("7k/1q4pp/8/8/8/8/6QP/5r1K w - - 0 1");
    b.display();
    vector<Move> legal_moves;
    b.get_pseudo_legal_moves(legal_moves);
    print_moves(legal_moves);

    enum_color turn = b.get_turn();
    for (Move m : legal_moves) {
        Board board_copy(b);
        board_copy.make_move(m); 
        if (board_copy.in_check(turn)) {
            cout << "Illegal move: " << m.get_notation() << endl;
        }
    }
}

void test_eval_function() {
    Board b("6qk/bb4pp/7q/8/8/7r/6NP/7K b - - 0 1");
    b.display();
    cout << "Evaluation: " << b.get_eval() << endl;
}

void print_best_move() {
    NUM_EVALED = 0;
    Board b("4b1q1/6k1/8/5n2/2N5/2K4P/8/3Q4 b - - 0 1"); // Runs in 4.20 seconds, Evaluated 116658 positions
    b.display();
    Move best = best_move(b);
    cout << "Best move: " << best.get_notation() << endl; 
    cout << "Evaluated " << NUM_EVALED << " positions" << endl;
}

void print_sorted_moves(Board &b) {
    b.display();
    vector<Move> legal_moves;
    b.get_legal_moves(legal_moves);
    cout << "Before sorting: " << endl;
    print_moves(legal_moves);
    sort(legal_moves.begin(), legal_moves.end(), std::greater<Move>());
    cout << "Sorted legal moves: " << endl;
    print_moves(legal_moves);
}

Move make_best_move(Board &b) {
    Move best = best_move(b);
    b.make_move(best);
    cout << "after " << best.get_notation() << ":" << endl;
    b.display();
    return best;
}

void make_two_moves(Board &b) {
    // Board b("r1B5/1P3k2/8/8/2K5/8/4p3/5N2 w - - 0 2");
    // Board b("r7/5k2/5p2/1pP5/3K4/4B3/4N3/8 w - b6 0 2");
    b.display();
    Move best = make_best_move(b);

    Move best2 = make_best_move(b);

    b.unmake_move(best2);
    b.unmake_move(best);
    cout << "restored: " << endl;
    b.display();
}

void print_eval_sq_adj(Board &b) {
    b.display();
    short eval = b.get_eval_adj();
    cout << "Eval adjustment: " << eval << endl;
}

void play_itself() {
    Board b;
    vector<Move> all_moves;
    while (!b.game_over()) {
        Move best = make_best_move(b);
        all_moves.push_back(best);
    }
    print_moves(all_moves);
}

int main(int argc, char* argv[]) {
    NUM_EVALED = 0;
    // pawn captures
    // Board b("rn1qkbnr/pp2pppp/8/2pp1b2/3PP3/5N2/PPP2PPP/RNBQKB1R b KQkq - 2 4");
    // en passant
    // Board b("rnbqkb1r/ppp1pppp/5n2/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");
    // knight and pawn captures
    // Board b("1nbqkb1r/rppp1ppp/p4n1B/3Bp3/3PP2P/5N2/PPP2PP1/RN1QK2R b KQk - 6 7");
    // rook moves
    // Board b("1r4k1/5p1p/4rn2/1p6/2n5/B1N1R3/5PP1/1R3K2 w - - 0 1");
    // all psuedo moves
    // Board b("1rq3k1/4bp1p/4rn2/1p6/2n2Q2/B1N1RB2/5PP1/1R3K2 b - - 0 1");
    // test en_passant
    // Board b("1rq3k1/4bp1p/4rn2/1pP5/2n2Q2/B1N1RB2/5PP1/1R3K2 w - b6 0 1");
    // test legal moves
    // Board b("5rk1/6p1/1b4q1/8/8/4B3/6N1/4R1K1 w - - 0 1");
    // test atks_sq
    // Board b("5rk1/6p1/1b4q1/8/8/7n/8/r3R1K1 w - - 0 1");
    // make_best_move();

    // vector<Move> legal_moves = b.get_legal_moves();
    // print_moves(legal_moves);
    // sort(legal_moves.begin(), legal_moves.end(), greater<Move>());
    // cout << "Sorted legal moves: " << endl;
    // print_moves(legal_moves);
    // print_sorted_moves();
    // print_best_move();
    // Board b("5k2/8/8/8/8/8/7P/5K2 w - - 0 1");
    // print_eval_sq_adj(b);
    // Move m = make_best_move(b);
    // print_eval_sq_adj(b);

    // white queenside castle
    // Board b("rnb1kbnr/pppppppp/6q1/8/8/8/PP1PPP1P/R3K2R w KQkq - 0 1");
    // print_eval_sq_adj(b);
    // Move m(5, (U64) 1 << 4, (U64) 1 << 2);
    // b.make_move(m);
    // print_eval_sq_adj(b);
    // b.unmake_move(m);
    // print_eval_sq_adj(b);

    //black kingside castle
    // Board b("rnb1k2r/pppppppp/6q1/8/8/8/PP1PPP1P/R3K2R b KQkq - 0 1");
    // print_eval_sq_adj(b);
    // Move m(5, (U64) 1 << 60, (U64) 1 << 62);
    // b.make_move(m);
    // print_eval_sq_adj(b);
    // b.unmake_move(m);
    // print_eval_sq_adj(b);

    // Board b("rnbqk2r/pppppppp/8/8/8/8/PP1PPP1P/R3K2R b KQkq - 0 1");
    // make_two_moves(b);
    // print_sorted_moves(b);

    play_itself();
    
    // test_illegal_moves();
    // Move test_move = Move(0, (U64) 1 << 34, (U64) 1 << 41, true, true);
    // cout << "After move " << test_move.get_notation() << ", board looks like this:" << endl;
    // b.make_move(test_move);
    // b.display();

    // U64 rook = (U64) 1 << 28;
    // U64 occ = (0x03UL << 24) | rook;
    // U64 rook = 2;
    // U64 occ = 6;
    // cout << "Rook: " << endl; print_bb(rook);
    // cout << "Occ: " << endl; print_bb(occ);
    // U64 rook_atks = line_atks(occ, rook);
    // cout << "rook attacks: " << endl; print_bb(rook_atks);
    // U64 rook_atks_pre = get_rook_atks(occ, rook);
    // cout << "rook attacks precomputed: " << endl; print_bb(rook_atks_pre);

    // U64 rook = (U64) 1 << 63;
    // U64 occ = rook | (rook >> 2) | (rook >> 3) | (rook >> 8);
    // cout << "Rook: " << endl; print_bb(rook);
    // cout << "Occ: " << endl; print_bb(occ);
    // U64 rook_atks_pre = get_rook_hor_atks(occ, rook);
    // cout << "rook attacks precomputed: " << endl; print_bb(rook_atks_pre);
    // U64 rook_atks_line = line_atks(occ, rook);
    // cout << "rook attacks line: " << endl; print_bb(rook_atks_line);

    // U64 bishop = (U64) 1 << 50;
    // U64 occ = bishop | (bishop >> 14);
    // cout << "Bishop: " << endl; print_bb(bishop);
    // cout << "Occ: " << endl; print_bb(occ);
    // U64 atks = diag_atks(occ, bishop, true);
    // cout << "Diagonal atks: " << endl; print_bb(atks);

    // generate knight attacks
    // cout << "Generating knight attacks: " << endl;
    // for (int i = 0; i < 64; i++) {
    //     U64 pos = (U64) 1 << i;
    //     U64 atks = 0;
    //     atks |= n_one(ne_one(pos) | nw_one(pos));
    //     atks |= e_one(ne_one(pos) | se_one(pos));
    //     atks |= s_one(se_one(pos) | sw_one(pos));
    //     atks |= w_one(nw_one(pos) | sw_one(pos));
    //     cout << atks << "UL" << ", ";
    //     if (i % 8 == 7) {
    //         cout << endl;
    //     }
    // }
    // generate king attacks
    // cout << "Generating king attacks: " << endl;
    // for (int i = 0; i < 64; i++) {
    //     U64 pos = (U64) 1 << i;
    //     U64 atks = 0;
    //     atks |= n_one(e_one(pos) | w_one(pos) | pos);
    //     atks |= e_one(pos);
    //     atks |= s_one(e_one(pos) | w_one(pos) | pos);
    //     atks |= w_one(pos);
    //     cout << atks << "UL" << ", ";
    //     if (i % 8 == 7) {
    //         cout << endl;
    //     }
    // }
    // generate and verify rook rank lookup
    // init_first_rank_atks();
    // verify_rook_atks();
    return 0;
}