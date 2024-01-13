#include "move.h"
#include "board.h"
#include "utils.h"
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
    char turn_char;
    string en_passant_str;
    string castle_str;
    ss >> board_str >> turn_char >> castle_str >> en_passant_str >> half_move_clock >> move_number;
    if (turn_char == 'w') turn = white;
    else if (turn_char == 'b') turn = black;
    else {cout << "Invalid turn character from fen!" << endl; exit(EXIT_FAILURE);}
    en_passant = sq_to_pos(en_passant_str);
    can_castle = 0;
    for (char cc : castle_str) {
        if (cc == 'K') {can_castle |= WK_CASTLE;}
        else if (cc == 'Q') {can_castle |= WQ_CASTLE;}
        else if (cc == 'k') {can_castle |= BK_CASTLE;}
        else {can_castle |= BQ_CASTLE;}
    }

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

int Board::piece_ind_at_square(U64 pos, enum_color color) {
    U64 (&piece_arr)[6] = (color == white ? w_piece_arr : b_piece_arr);
    for (int i = 0; i < 6; i++) {
        if (piece_arr[i] & pos) {
            return i;
        }
    } 
    return -1;
}

char Board::piece_at_square(U64 pos) {
    for (int i = 0; i < 6; i++) {
        if (w_piece_arr[i] & pos) {
            return toupper(NUM_TO_FEN[i]);
        }
    }

    for (int i = 0; i < 6; i++) {
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

void Board::get_pseudo_legal_moves(vector<Move> &legal_moves) {
    U64 (&p_arr)[6] = (turn == white ? w_piece_arr : b_piece_arr);
    for (int i = 0; i < 6; i++) {
        U64 piece_bb = p_arr[i];
        if (i == 0) {
            pawns_legal_moves(legal_moves, piece_bb, this);
        }
        else if (i == 1 || i == 5) {
            king_knights_legal_moves(legal_moves, piece_bb, this, i);
        }
        else {
            sliders_legal_moves(legal_moves, piece_bb, this, i);
        }
    }

    return;
}

void Board::get_legal_moves(std::vector<Move> &legal_moves) {
    vector<Move> psuedo_legal;
    get_pseudo_legal_moves(psuedo_legal);
    enum_color moving_color = turn;
    for (Move m : psuedo_legal) {
        make_move(m); 
        if (!in_check(moving_color)) {
            legal_moves.push_back(m);
        }
        unmake_move(m);
    }
    return;
}

void Board::make_move(const Move &move) {
    U64 start = move.get_start();
    U64 finish = move.get_finish();
    U64 moved_piece = move.get_piece();
    int promote = move.get_promote();
    // update piece_bb
    U64 (&pieces_arr)[6] = (turn == white ? w_piece_arr : b_piece_arr);
    U64 &piece_bb = pieces_arr[moved_piece];
    piece_bb &= ~start;
    if (promote > -1) {
        pieces_arr[promote] |= finish;
    }
    else {
        piece_bb |= finish;
    }
    // update bb for this color
    U64 &my_color_bb = (turn == white ? w_pieces : b_pieces);
    my_color_bb &= ~start;
    my_color_bb |= finish;

    int old_hmc = half_move_clock;
    int u_captured = 0;
    // delete piece for capture, reset 50 move rule
    if (move.get_capture()) {
        U64 erase = finish;
        if (move.get_en_passant()) {erase = (turn == white ? finish >> 8 : finish << 8);}
        u_captured = piece_ind_at_square(erase, (turn == white ? black : white));
        if (u_captured == -1) {cout << "captured piece not found" << endl; exit(EXIT_FAILURE);}
        U64 (&opp_pieces_arr)[6] = (turn == black ? w_piece_arr : b_piece_arr);
        U64 &opp_color_bb = (turn == black ? w_pieces : b_pieces);
        for (int i = 0; i < 6; i++) {
            opp_pieces_arr[i] &= ~erase;
        }
        opp_color_bb &= ~erase;
        half_move_clock = 0;
    }
    else if (moved_piece == 0) {
        half_move_clock = 0;
    }
    else half_move_clock++;
    
    // push into stack
    move_stk.push_back(Unmove(u_captured, en_passant, can_castle, old_hmc));

    // update en_passant
    if (moved_piece == 0 && (((start >> 16) & finish) | ((start << 16) & finish))) {
        en_passant = (turn == white ? start << 8 : start >> 8);
    }
    else {
        en_passant = 0;
    }
    // update turn, move clocks
    if (turn == black) {
        turn = white;
        move_number++;
    }
    else {
        turn = black;
    }
}

void Board::unmake_move(const Move &move) {
    U64 start = move.get_start();
    U64 finish = move.get_finish();
    U64 moved_piece = move.get_piece();
    int promote = move.get_promote();
    // update piece_bb
    U64 (&pieces_arr)[6] = (turn == black ? w_piece_arr : b_piece_arr);
    U64 &piece_bb = pieces_arr[moved_piece];
    piece_bb |= start;
    if (promote > -1) {
        pieces_arr[promote] &= ~finish;
    }
    else {
        piece_bb &= ~finish;
    }
    // update bb for this color
    U64 &my_color_bb = (turn == black ? w_pieces : b_pieces);
    my_color_bb |= start;
    my_color_bb &= ~finish;

    // pop irreversibles
    Unmove unmove = move_stk.back();
    move_stk.pop_back();
    // spawn piece for capture, reset 50 move rule
    if (move.get_capture()) {
        U64 erase = finish;
        if (move.get_en_passant()) {erase = (turn == black ? finish >> 8 : finish << 8);}
        U64 (&opp_pieces_arr)[6] = (turn == white ? w_piece_arr : b_piece_arr);
        U64 &opp_color_bb = (turn == white ? w_pieces : b_pieces);
        opp_pieces_arr[unmove.get_captured()] |= erase;

        opp_color_bb |= erase;
    }
    half_move_clock = unmove.get_hmc();
    can_castle = unmove.get_can_castle();

    // update en_passant
    en_passant = unmove.get_en_passant();
    // update turn, move clocks
    if (turn == black) {
        turn = white;
    }
    else {
        turn = black;
        move_number--;
    }
}

bool Board::in_check(enum_color color) {
    U64 (&opp_bb_arr)[6] = (color == white ? b_piece_arr : w_piece_arr);
    U64 sq = (color == white ? w_piece_arr[5] : b_piece_arr[5]);
    U64 atks = attacks_to_sq(opp_bb_arr, w_pieces | b_pieces, sq, color);
    // cout << "atks bb: " << endl;
    // print_bb(atks);
    return atks;
}

// TODO: potential optimization only count legal moves
bool Board::is_checkmated() {
    if (in_check(turn)) {
        vector<Move> moves;
        get_legal_moves(moves);
        return (moves.size() == 0);
    }
    return false;
}

bool Board::is_stalemated() {
    if (!in_check(turn)) {
        vector<Move> moves;
        get_legal_moves(moves);
        return (moves.size() == 0);
    }
    return false;
}

bool Board::game_over() {
    return is_checkmated() || is_stalemated();
}

int Board::count_pieces(int piece, enum_color color) {
    U64 (&piece_bb_arr)[6] = (color == white ? w_piece_arr : b_piece_arr);
    U64 piece_bb = piece_bb_arr[piece];
    int count = 0;
    while (piece_bb) {
        piece_bb &= piece_bb - 1;
        count++;
    }
    return count;
}

float Board::get_eval() {
    if (is_checkmated()) return (turn == white ? B_CHECKMATE_SCORE : W_CHECKMATE_SCORE);
    if (is_stalemated() || half_move_clock == 100) return 0;
    float eval = 0;
    for (int i = 0; i < 6; i++) {
        eval += (count_pieces(i, white) - count_pieces(i, black)) * PIECE_VALUES[i];
    }
    return eval;
}