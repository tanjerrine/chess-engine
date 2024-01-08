#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <map>
#include <vector>
#include <sstream>

typedef unsigned long long U64;

const U64 NOT_A_FILE = 0xfefefefefefefefe;
const U64 NOT_H_FILE = 0x7f7f7f7f7f7f7f7f;

const std::map<char, std::string> FEN_TO_PIECE = {{'p', "pawn"}, {'n', "knight"}, {'b', "bishop"}, {'r', "rook"}, {'q', "queen"}, {'k', "king"}};
const std::map<std::string, int> PIECE_TO_NUM = {{"pawn", 0}, {"knight", 1}, {"bishop", 2}, {"rook", 3}, {"queen", 4}, {"king", 5}};
const std::vector<char> NUM_TO_FEN = {'p', 'n', 'b', 'r', 'q', 'k'};

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

#define n_one(b) (b << 8)
#define s_one(b) (b >> 8)
#define e_one(b) ((b << 1) & NOT_A_FILE)
#define ne_one(b) ((b << 9) & NOT_A_FILE)
#define se_one(b) ((b >> 7) & NOT_A_FILE)
#define w_one(b) ((b >> 1) & NOT_H_FILE)
#define nw_one(b) ((b << 7) & NOT_H_FILE)
#define sw_one(b) ((b >> 9) & NOT_H_FILE)
#define rank_mask(rank) ((U64) 255 << (8 * (rank - 1)))

char int_to_alg_not(int piece);

int bit_scan(U64 bb);

std::string pos_to_sq(U64 pos);

U64 sq_to_pos(std::string sq);

#endif