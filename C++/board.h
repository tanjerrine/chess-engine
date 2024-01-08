#ifndef BOARD_H
#define BOARD_H
#include <string>

typedef unsigned long long U64;

class Board {
    public:
        Board(std::string fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        char piece_at_square(U64 pos);
        void display();
        void get_legal_moves();
        U64 w_piece_arr[6];
        U64 b_piece_arr[6];
        char get_turn() {return turn;}
        U64 get_w_pieces() {return w_pieces;}
        U64 get_b_pieces() {return b_pieces;}
        std::string get_en_passant() {return en_passant;}

    private:
        char turn;
        std::string can_castle;
        std::string en_passant;
        U64 w_pieces;
        U64 b_pieces;
};

#endif