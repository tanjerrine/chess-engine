#include "board.h"
#include "utils.h"
#include "move.h"
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <utility>

using std::string; using std::cout; using std::endl; using std::stringstream;
using std::map; using std::vector; using std::pair;

Board::Board(std::string fen) {
    stringstream ss;
    ss << fen;
    string board_str;
    ss >> board_str >> turn >> can_castle >> en_passant;

    for (int i = 0; i < 6; i++) {
        w_piece_arr[i] = 0;
        b_piece_arr[i] = 0;
    }

    U64 pos = (U64) 1 << 56;
    for (char c : board_str) {
        if ('1' <= c && c <= '9') {
            pos <<= (c - '0');
        }
        else if ('A' <= c && c <= 'Z') {
            w_piece_arr[PIECE_TO_NUM.at(FEN_TO_PIECE.at(tolower(c)))] |= pos;
            pos <<= 1;
        }
        else if ('a' <= c && c <= 'z') {
            b_piece_arr[PIECE_TO_NUM.at(FEN_TO_PIECE.at(c))] |= pos;
            pos <<= 1;
        }
        else if (c == '/') {
            pos = (pos ? pos >> 16 : (U64) 1 << 48);
        }
    }
    w_pieces = 0;
    b_pieces = 0;
    for (int i = 0; i < 6; i++) {
        w_pieces |= w_piece_arr[i];
        b_pieces |= b_piece_arr[i];
    }
}

char Board::piece_at_square(U64 pos) {
    for (int i = 0; i < sizeof(w_piece_arr) / sizeof(w_piece_arr[0]); i++) {
        if (w_piece_arr[i] & pos) {
            return toupper(NUM_TO_FEN[i]);
        }
    }

    for (int i = 0; i < sizeof(b_piece_arr) / sizeof(b_piece_arr[0]); i++) {
        if (b_piece_arr[i] & pos) {
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

void Board::get_legal_moves() {
    vector<Move> legal_moves;
    U64 (&p_arr)[6] = (turn == 'w' ? w_piece_arr : b_piece_arr);
    for (int i = 0; i < 6; i++) {
        if (i == 0) {
            U64 piece_bb = p_arr[i];
            vector<Move> p_moves = pawns_legal_moves(piece_bb, this);
            legal_moves = p_moves;
        }
        else {
            break;
        }
    }
    cout << legal_moves.size() << endl;
    cout << "All legal moves: ";
    for (auto m : legal_moves) {
        cout << m.get_notation() << ", ";
    }
    cout << endl;
}