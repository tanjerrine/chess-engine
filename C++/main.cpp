#include "board.h"
#include "move.h"
#include <iostream>

using std::cout; using std::endl;

int main(int argc, char* argv[]) {
    // pawn captures
    // Board b("rn1qkbnr/pp2pppp/8/2pp1b2/3PP3/5N2/PPP2PPP/RNBQKB1R b KQkq - 2 4");
    // en passant
    // Board b("rnbqkb1r/ppp1pppp/5n2/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");
    // knight and pawn captures
    Board b("1nbqkb1r/rppp1ppp/p4n1B/3Bp3/3PP2P/5N2/PPP2PP1/RN1QK2R b KQk - 6 7");
    b.display();
    // Move m(1, (U64) 2, (U64) 1 << 18, 0);
    // cout << m.get_notation() << endl;
    // Move m2(0, (U64) 1 << 27, (U64) 1 << 36, 0);
    // cout << m2.get_notation() << endl;
    // Move m3(2, (U64) 1 << 27, (U64) 1 << 36);
    // cout << m3.get_notation() << endl;
    b.get_legal_moves();

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
    return 0;
}