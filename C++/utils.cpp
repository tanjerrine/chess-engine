#include "utils.h"
#include <sstream>
#include <iostream>

using std::stringstream; using std::cout; using std::endl;

const short *SQ_TBS[6] = {PAWN_SQ_TB, KNIGHT_SQ_TB, BISHOP_SQ_TB, ROOK_SQ_TB, QUEEN_SQ_TB, KING_SQ_TB};

char int_to_alg_not(int piece) {
    char fen = NUM_TO_FEN[piece];
    return (fen == 'p' ? '\0' : toupper(fen));
}

int bit_scan(U64 bb) {
    U64 debrujin64 = 0x07EDD5E59A4E28C2;
    return index64[((bb & (-bb)) * debrujin64) >> 58];
}

std::string pos_to_sq(U64 pos) {
    int ind = bit_scan(pos);
    int rank = ind / 8 + 1;
    char file = ind % 8 + 'a';
    stringstream ss;
    ss << file << rank;
    return ss.str();
}

U64 sq_to_pos(std::string sq) {
    if (sq == "-") {
        return 0;
    }
    if (sq.size() != 2) {
        cout << "Square does not have exactly two characters, nor is -" << endl;
        exit(EXIT_FAILURE);
    }
    int file = sq[0] - 'a';
    int rank = sq[1] - '1';
    return (U64) 1 << (8 * rank + file);
}

void print_bb(U64 bb) {
    for (int r = 7; r >= 0; r--) {
        cout << r+1 << " | ";
        for (int c = 0; c < 8; c++) {
            char p = ((bb & ((U64) 1 << (8 * r + c))) ? '1' : '.');
            cout << p << " ";
        }
        cout << endl;
    }
    cout << "    " << "- - - - - - - - " << endl;
    cout << "    " << "a b c d e f g h" << endl;
}

U8 reverse_bits(U8 b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

U64 diagonal_mask(int sq) {
    int rem = sq % 9; 
    if (sq / 8 != sq / 9) {
        int shift = 9 - rem;
        return (main_diagonal >> shift) & ~first_n_ranks_mask(shift); 
    }
    else {
        if (rem == 0) return main_diagonal;
        return (main_diagonal << rem) & first_n_ranks_mask(8 - rem);
    }
}