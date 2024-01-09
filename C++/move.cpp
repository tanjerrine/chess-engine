#include "move.h"
#include "utils.h"
#include "board.h"
#include <iostream>
// #include <cbyteswap>

using std::string; using std::cout; using std::endl;
using std::vector;

Move::Move(int piece, U64 start, U64 finish, bool capture, bool en_passant, int promote) : 
    piece(piece), start(start), finish(finish), capture(capture), en_passant(en_passant), promote(promote) {}

string Move::get_notation() {
    string f_pos = pos_to_sq(finish);
    string notation;
    if (!capture) {
        notation = int_to_alg_not(piece) + f_pos;
    }
    else {
        notation = (piece ? int_to_alg_not(piece) : pos_to_sq(start)[0]) + ('x' + f_pos);
    }
    // if (en_passant) notation += 'e';
    return notation;
}

// TODO: make const ref to board?
// TODO: promotion
vector<Move> pawns_legal_moves(U64 pawn_poss, Board* board) {
    vector<Move> moves;
    char color = board->get_turn();
    // Captures
    U64 opp_pieces = (color == 'w' ? board->get_b_pieces() : board->get_w_pieces());
    U64 en_passant = sq_to_pos(board->get_en_passant());
    U64 east_caps = (opp_pieces | en_passant) & (color == 'w' ? w_pawn_east_atks(pawn_poss) : b_pawn_east_atks(pawn_poss));
    U64 west_caps = (opp_pieces | en_passant) & (color == 'w' ? w_pawn_west_atks(pawn_poss) : b_pawn_west_atks(pawn_poss));
    while (east_caps) {
        U64 curr = east_caps & -east_caps;
        U64 start_pos = (color == 'w' ? sw_one(curr) : nw_one(curr));
        Move m(0, start_pos, curr, true, curr & en_passant);
        moves.push_back(m);
        east_caps &= east_caps - 1;
    }
    while (west_caps) {
        U64 curr = west_caps & -west_caps;
        U64 start_pos = (color == 'w' ? se_one(curr) : ne_one(curr));
        Move m(0, start_pos, curr, true, curr & en_passant);
        moves.push_back(m);
        west_caps &= west_caps - 1;
    }

    // Moves
    U64 same_pieces = (color == 'b' ? board->get_b_pieces() : board->get_w_pieces());
    U64 one_moves = (color == 'b' ? s_one(pawn_poss) : n_one(pawn_poss)) & ~(same_pieces | opp_pieces);
    U64 one_move_mask = rank_mask((color == 'b' ? 6 : 3));
    U64 two_moves = ((one_move_mask & one_moves) << 8) & ~(same_pieces | opp_pieces);
    if (color == 'b') two_moves = ((one_move_mask & one_moves) >> 8) & ~(same_pieces | opp_pieces);

    while (one_moves) {
        U64 curr = one_moves & -one_moves;
        U64 start_pos = (color == 'w' ? s_one(curr) : n_one(curr));
        Move m(0, start_pos, curr);
        moves.push_back(m);
        one_moves &= one_moves - 1;
    }
    while (two_moves) {
        U64 curr = two_moves & -two_moves;
        U64 start_pos = (color == 'w' ? s_one(s_one(curr)) : n_one(n_one(curr)));
        Move m(0, start_pos, curr);
        moves.push_back(m);
        two_moves &= two_moves - 1;
    }

    return moves;
}

vector<Move> king_knights_legal_moves(U64 piece_bb, Board* board, int piece) {
    if (piece != 1 && piece != 5) {
        cout << "Incorrect usage of king_knights_legal_moves, piece must be knight or king" << endl;
        exit(EXIT_FAILURE);
    }
    vector<Move> moves;
    char color = board->get_turn();
    U64 opp_pieces = (color == 'w' ? board->get_b_pieces() : board->get_w_pieces());
    U64 same_pieces = (color == 'b' ? board->get_b_pieces() : board->get_w_pieces());
    const U64 (&move_lookup)[64] = (piece == 1 ? knight_attacks : king_attacks);
    while (piece_bb) {
        U64 start_pos = piece_bb & -piece_bb;
        U64 atks = move_lookup[bit_scan(piece_bb)];
        atks &= ~same_pieces;
        while (atks) {
            U64 curr = atks & -atks;
            Move m(piece, start_pos, curr, curr & opp_pieces);
            moves.push_back(m);
            atks &= atks - 1;
        }
        piece_bb &= piece_bb - 1;
    }
    return moves;
}

U64 line_atks(U64 occ, U64 slider) {
    int rank_adj = (bit_scan(slider) / 8) * 8;
    occ >>= rank_adj; slider >>= rank_adj;

    U64 mask = rank_mask(1);
    U64 forward = occ & mask;
    // cout << "o: " << endl; print_bb(forward);
    forward -= slider;
    // cout << "o-s: " << endl; print_bb(forward);
    U64 reverse = reverse_bits(forward);
    // cout << "reverse: " << endl; print_bb(reverse);
    forward -= slider;
    // cout << "o-2s: " << endl; print_bb(forward);
    // cout << "reverse(slider): " << endl; print_bb(reverse_bits(slider));
    reverse -= reverse_bits(slider);
    // cout << "o'-2s': " << endl; print_bb(reverse);

    forward ^= (reverse_bits(reverse));
    // cout << "final: " << endl; print_bb(forward);
    // cout << "mask: " << endl; print_bb(mask);
    return (forward & mask) << rank_adj;
}

U64 get_rook_hor_atks(U64 occ, U64 slider) {
    int sq = bit_scan(slider);
    int rank_adj = (sq / 8) * 8;
    occ >>= rank_adj; slider >>= rank_adj;
    U8 occ_byte = occ & rank_mask(1);
    U8 slider_ind = sq - rank_adj;
    return (U64) rook_first_rank_attacks[(occ_byte >> 1) & 63][slider_ind] << rank_adj;
}

U64 get_rook_vert_atks(U64 occ, U64 slider) {
    int sq = bit_scan(slider);
    int file_adj = sq % 8;
    occ >>= file_adj;
    occ &= column_mask(0);
    occ *= main_diagonal;
    occ >>= 56;
    int slider_ind = scalar_90_ccw_rotate(sq - file_adj);
    U64 atks = (U64) rook_first_rank_attacks[(occ >> 1) & 63][slider_ind];
    atks *= main_diagonal;
    atks = (atks >> 7) & column_mask(0);
    return atks << file_adj;
}

U64 get_rook_atks(U64 occ, U64 slider) {
    return get_rook_hor_atks(occ, slider) | get_rook_vert_atks(occ, slider);
}

vector<Move> rooks_legal_moves(U64 piece_bb, Board* board) {
    vector<Move> moves;
    char color = board->get_turn();
    U64 opp_pieces = (color == 'w' ? board->get_b_pieces() : board->get_w_pieces());
    U64 same_pieces = (color == 'b' ? board->get_b_pieces() : board->get_w_pieces());
    U64 occ = opp_pieces | same_pieces;
    while (piece_bb) {
        U64 start_pos = piece_bb & -piece_bb;
        // cout << "rook at: " << endl; print_bb(start_pos);
        U64 atks = get_rook_atks(occ, start_pos);
        // cout << "atks: " << endl; print_bb(atks);
        atks &= ~same_pieces;
        while (atks) {
            U64 curr = atks & -atks;
            Move m(3, start_pos, curr, curr & opp_pieces);
            moves.push_back(m);
            atks &= atks - 1;
        }
        piece_bb &= piece_bb - 1;
    }
    return moves;
}