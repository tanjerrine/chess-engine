#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <map>
#include <vector>
#include <sstream>

typedef unsigned long long U64;
typedef unsigned char U8;

const U64 NOT_A_FILE = 0xfefefefefefefefe;
const U64 NOT_H_FILE = 0x7f7f7f7f7f7f7f7f;
const U64 main_diagonal = 0x8040201008040201;
const U64 anti_diagonal = 0x102040810204080;

const std::map<char, std::string> FEN_TO_PIECE = {{'p', "pawn"}, {'n', "knight"}, {'b', "bishop"}, {'r', "rook"}, {'q', "queen"}, {'k', "king"}};
const std::map<std::string, int> PIECE_TO_NUM = {{"pawn", 0}, {"knight", 1}, {"bishop", 2}, {"rook", 3}, {"queen", 4}, {"king", 5}};
const std::vector<char> NUM_TO_FEN = {'p', 'n', 'b', 'r', 'q', 'k'};
const short PIECE_VALUES[6] = {100, 320, 330, 500, 900, 15000};

const int index64[64] = {
    63, 0,  58, 1,  59, 47, 53, 2, 
    60, 39, 48, 27, 54, 33, 42, 3, 
    61, 51, 37, 40, 49, 18, 28, 20, 
    55, 30, 34, 11, 43, 14, 22, 4, 
    62, 57, 46, 52, 38, 26, 32, 41, 
    50, 36, 17, 19, 29, 10, 13, 21, 
    56, 45, 25, 31, 35, 16, 9, 12, 
    44, 24, 15, 8,  23, 7,  6, 5
};

const short PAWN_SQ_TB[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10,-20,-20, 10, 10,  5,
    5, -5,-10,  0,  0,-10, -5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5,  5, 10, 25, 25, 10,  5,  5,
    10, 10, 20, 30, 30, 20, 10, 10,
    50, 50, 50, 50, 50, 50, 50, 50,
    0,  0,  0,  0,  0,  0,  0,  0
};

const short KNIGHT_SQ_TB[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

const short BISHOP_SQ_TB[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

const short ROOK_SQ_TB[64] = {
     0,  0,  0,  5,  5,  0,  0,  0,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     5, 10, 10, 10, 10, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
};

const short KINGSIDE_ROOK_ADJ = ROOK_SQ_TB[5] - ROOK_SQ_TB[7];
const short QUEENSIDE_ROOK_ADJ = ROOK_SQ_TB[3] - ROOK_SQ_TB[0];
const U64 WK_CASTLE_SQ = (U64) 1 << 7;
const U64 WQ_CASTLE_SQ = (U64) 1;
const U64 BK_CASTLE_SQ = (U64) 1 << 63;
const U64 BQ_CASTLE_SQ = (U64) 1 << 56;

const short QUEEN_SQ_TB[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -10,  5,  5,  5,  5,  5,  0,-10,
      0,  0,  5,  5,  5,  5,  0, -5,
     -5,  0,  5,  5,  5,  5,  0, -5,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

const short KING_SQ_TB[64] = {
     20, 30, 10,  0,  0, 10, 30, 20,
     20, 20,  0,  0,  0,  0, 20, 20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30
};

extern const short *SQ_TBS[6];

#define n_one(b) ((b) << 8)
#define s_one(b) ((b) >> 8)
#define e_one(b) (((b) << 1) & NOT_A_FILE)
#define ne_one(b) (((b) << 9) & NOT_A_FILE)
#define se_one(b) (((b) >> 7) & NOT_A_FILE)
#define w_one(b) (((b) >> 1) & NOT_H_FILE)
#define nw_one(b) (((b) << 7) & NOT_H_FILE)
#define sw_one(b) (((b) >> 9) & NOT_H_FILE)
#define rank_mask(rank) ((U64) 255 << (8 * ((rank) - 1)))
#define rank_of_sq(sq) ((sq) / 8 + 1)
#define column_mask(col) (~NOT_A_FILE << (col))
#define first_n_ranks_mask(n) (((U64) 1 << (8 * (n))) - 1)
#define flip_sq(n) ((n) ^ 56)

char int_to_alg_not(int piece);

int bit_scan(U64 bb);

std::string pos_to_sq(U64 pos);

U64 sq_to_pos(std::string sq);

template<class T>
inline void vector_extend(T &old, T &append) {
    old.reserve(old.size() + distance(append.begin(), append.end()));
    old.insert(old.end(), append.begin(), append.end());
}

void print_bb(U64 bb);

U8 reverse_bits(U8 b);

inline int scalar_90_ccw_rotate(int sq) {return (((sq >> 3) | (sq << 3)) & 63) ^ 7;}

U64 diagonal_mask(int sq);
inline U64 anti_diagonal_mask(int sq) {return __builtin_bswap64(diagonal_mask(sq ^ 56));}

inline short get_sq_adj(int piece, int sq, bool is_black) {
    if (is_black) {sq = flip_sq(sq);}
    return SQ_TBS[piece][sq];
}

#endif