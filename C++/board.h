#include <string>

typedef unsigned long long U64;

class Board {
    public:
        Board(std::string fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        char piece_at_square(U64 pos);
        void display();

    private:
        char turn;
        std::string can_castle;
        std::string en_passant;
        U64 w_pieces[6];
        U64 b_pieces[6];
};