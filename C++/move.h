#ifndef MOVE_H
#define MOVE_H
#include "utils.h"
#include "board.h"
#include <string>
#include <vector>

typedef unsigned long long U64;

class Move {
    public:
        Move(int piece, U64 start, U64 finish, bool capture = false, bool en_passant = false, int promote = -1);
        std::string get_notation();

    private:
        int piece;
        U64 start;
        U64 finish;
        bool capture;
        bool en_passant;
        int promote;
};

inline U64 w_pawn_east_atks(U64 w_pawns) {return ne_one(w_pawns);}
inline U64 w_pawn_west_atks(U64 w_pawns) {return nw_one(w_pawns);}
inline U64 b_pawn_east_atks(U64 b_pawns) {return se_one(b_pawns);}
inline U64 b_pawn_west_atks(U64 b_pawns) {return sw_one(b_pawns);}

std::vector<Move> pawns_legal_moves(U64 pawn_poss, Board* board);

#endif
