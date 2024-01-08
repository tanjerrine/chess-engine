#include "board.h"
#include "move.h"
#include <iostream>

using std::cout; using std::endl;

int main(int argc, char* argv[]) {
    // Board b("rn1qkbnr/pp2pppp/8/2pp1b2/3PP3/5N2/PPP2PPP/RNBQKB1R b KQkq - 2 4");
    Board b("rnbqkb1r/ppp1pppp/5n2/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");
    b.display();
    Move m(1, (U64) 2, (U64) 1 << 18, 0);
    cout << m.get_notation() << endl;
    Move m2(0, (U64) 1 << 27, (U64) 1 << 36, 0);
    cout << m2.get_notation() << endl;
    Move m3(2, (U64) 1 << 27, (U64) 1 << 36);
    cout << m3.get_notation() << endl;
    b.get_legal_moves();
    return 0;
}