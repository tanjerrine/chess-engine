#include "board.h"
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <utility>

using std::string; using std::cout; using std::endl; using std::stringstream;
using std::map; using std::vector; using std::pair;

map<char, string> FEN_TO_PIECE = {{'p', "pawn"}, {'n', "knight"}, {'b', "bishop"}, {'r', "rook"}, {'q', "queen"}, {'k', "king"}};
map<string, int> PIECE_TO_NUM = {{"pawn", 0}, {"knight", 1}, {"bishop", 2}, {"rook", 3}, {"queen", 4}, {"king", 5}};
vector<char> NUM_TO_FEN = {'p', 'n', 'b', 'r', 'q', 'k'};

Board::Board(std::string fen) {
    stringstream ss;
    ss << fen;
    string board_str;
    ss >> board_str >> turn >> can_castle >> en_passant;

    for (int i = 0; i < 6; i++) {
        w_pieces[i] = 0;
        b_pieces[i] = 0;
    }

    U64 pos = (U64) 1 << 56;
    for (char c : board_str) {
        if ('1' <= c && c <= '9') {
            pos <<= (c - '0');
        }
        else if ('A' <= c && c <= 'Z') {
            w_pieces[PIECE_TO_NUM[FEN_TO_PIECE[tolower(c)]]] |= pos;
            pos <<= 1;
        }
        else if ('a' <= c && c <= 'z') {
            b_pieces[PIECE_TO_NUM[FEN_TO_PIECE[c]]] |= pos;
            pos <<= 1;
        }
        else if (c == '/') {
            pos = (pos ? pos >> 16 : (U64) 1 << 48);
        }
    }
    cout << board_str << turn << can_castle << en_passant << endl;
}

char Board::piece_at_square(U64 pos) {
    for (int i = 0; i < sizeof(w_pieces) / sizeof(w_pieces[0]); i++) {
        if (w_pieces[i] & pos) {
            return toupper(NUM_TO_FEN[i]);
        }
    }

    for (int i = 0; i < sizeof(b_pieces) / sizeof(b_pieces[0]); i++) {
        if (b_pieces[i] & pos) {
            return (NUM_TO_FEN[i]);
        }
    }
    
    return '.';
}

void Board::display() {
    for (int r = 7; r >= 0; r--) {
        cout << r+1 << " | ";
        for (int c = 0; c < 8; c++) {
            // TODO: optimize piece_at_square to not instantiate object just to print
            cout << piece_at_square((U64) 1 << (8 * r + c)) << " ";
        }
        cout << endl;
    }
    cout << "    " << "- - - - - - - - " << endl;
    cout << "    " << "a b c d e f g h" << endl;
}