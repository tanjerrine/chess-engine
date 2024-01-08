#include "utils.h"
#include <sstream>
#include <iostream>

using std::stringstream; using std::cout; using std::endl;

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