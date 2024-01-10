#ifndef BOARD_H
#define BOARD_H
#include "move.h"
#include <string>
#include <vector>

typedef unsigned long long U64;

enum enum_color {white, black};

class Board {
    public:
        Board(std::string fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        U64 w_piece_arr[6];
        U64 b_piece_arr[6];
        enum_color get_turn() const {return turn;}
        U64 get_w_pieces() const {return w_pieces;}
        U64 get_b_pieces() const {return b_pieces;}
        U64 get_en_passant() const {return en_passant;}
        char piece_at_square(U64 pos);
        void display();
        std::vector<Move> get_legal_moves();
        void make_move(const Move &move);
        bool in_check(enum_color color);

    private:
        enum_color turn;
        std::string can_castle;
        U64 en_passant;
        U64 w_pieces;
        U64 b_pieces;
        int half_move_clock;
        int move_number;
};

#endif