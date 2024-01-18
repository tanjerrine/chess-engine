#ifndef BOARD_H
#define BOARD_H
#include <string>
#include <vector>

typedef unsigned long long U64;

enum enum_color {white, black};

#include "move.h"
#include <limits.h>

const short VALUE_MATED = -SHRT_MAX/2;

const U8 WK_CASTLE = 0x08;
const U8 WQ_CASTLE = 0x04;
const U8 BK_CASTLE = 0x02;
const U8 BQ_CASTLE = 0x01;

extern U64 NUM_EVALED;

class Board {
    public:
        Board(std::string fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        U64 w_piece_arr[6];
        U64 b_piece_arr[6];
        enum_color get_turn() const {return turn;}
        U64 get_w_pieces() const {return w_pieces;}
        U64 get_b_pieces() const {return b_pieces;}
        U64 get_en_passant() const {return en_passant;}
        short get_eval_adj() const {return eval_sq_adj;}
        U8 get_castle_rights() const {return can_castle;}
        int get_move_number() const {return move_number;}
        int piece_ind_at_square(U64 pos, enum_color color);
        char piece_at_square(U64 pos);
        void display();
        void get_pseudo_legal_moves(std::vector<Move> &legal_moves);
        void get_legal_moves(std::vector<Move> &legal_moves);
        void make_move(const Move &move);
        void unmake_move(const Move &move);
        bool in_check(enum_color color);
        bool is_checkmated();
        bool is_stalemated();
        bool game_over();
        int count_pieces(int piece, enum_color color);
        short get_eval(); 

    private:
        enum_color turn;
        U8 can_castle;
        U64 en_passant;
        U64 w_pieces;
        U64 b_pieces;
        int half_move_clock;
        int move_number;
        std::vector<Unmove> move_stk;
        short eval_sq_adj;
};

#endif