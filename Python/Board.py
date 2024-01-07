import numpy as np
from bitarray import bitarray
import Piece

FEN_TO_PIECE = {"p": "pawn", "n": "knight", "b": "bishop", "r": "rook", "q": "queen", "k": "king"}
PIECE_TO_NUM = {"pawn": 0, "knight": 1, "bishop": 2, "rook": 3, "queen": 4, "king": 5}
NUM_TO_PIECE = ["pawn", "knight", "bishop", "rook", "queen", "king"]

def bitset_int64(num, bit):
    return num | (np.int64(1) << bit)

def piece_notation(p_name, is_white):
    letter = "n" if p_name == "knights" else p_name[0] 
    return letter if not is_white else chr(ord(letter)-32)

def ind_to_sq(ind):
    rank = ind // 8 + 1
    file = chr(ind % 8 + 97)
    return file+str(rank)

def ind_is_valid(ind):
    return 0 <= ind < 64

class Board:
    def __init__(self, fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"):
        board_str, self.turn, self.can_castle, self.en_passant, _, _ = fen.split()
        
        self.w_pieces = np.zeros((6,), dtype=np.int64)
        self.b_pieces = np.zeros((6,), dtype=np.int64)
        
        rows = board_str.split("/")
        pos = np.int64(1) << 56
        for row in rows:
            for c in row:
                if 49 <= ord(c) <= 56:
                    pos <<= int(c)
                elif 65 <= ord(c) <= 90:
                    self.w_pieces[PIECE_TO_NUM[FEN_TO_PIECE[c.lower()]]] |= pos
                    pos <<= 1
                else:
                    self.b_pieces[PIECE_TO_NUM[FEN_TO_PIECE[c]]] |= pos
                    pos <<= 1
            pos = pos >> 16 if pos else np.int64(1) << 48

    # given pos as int64, get piece abbreviation on that square, or Null piece if none. 
    def piece_at_square(self, pos, display_only=False):
        for i, bb in enumerate(self.w_pieces):
            if bb & pos:
                return Piece.Piece("w", NUM_TO_PIECE[i]) if not display_only else Piece.display_notation(NUM_TO_PIECE[i], "w")
            
        for i, bb in enumerate(self.b_pieces):
            if bb & pos:
                return Piece.Piece("b", NUM_TO_PIECE[i]) if not display_only else Piece.display_notation(NUM_TO_PIECE[i], "b")
        
        return Piece.Piece() if not display_only else "."

    def display(self):
        # TODO: optimize loops?
        for r in range(7, -1, -1):
            print("{} | ".format(r + 1), end="")
            for c in range(8):
                # TODO: optimize piece_at_square to not instantiate object just to print
                print(self.piece_at_square(np.int64(1) << (8 * r + c), True), end=" ")
            print("")
        
        print(" " * 4 + "- " * 8)
        print(" " * 4 + "a b c d e f g h")
    
    def get_legal_moves(self):
        legal_moves = []
        p_arr = self.w_pieces if self.turn == "w" else self.b_pieces
        for piece_num, piece_board in enumerate(p_arr):
            pos = np.int64(1)
            while pos:
                if piece_board & pos:
                    legal_moves.extend(Piece.Piece(self.turn, NUM_TO_PIECE(piece_num)).legal_moves(pos, self))
                pos <<= 1
        disp_legal_moves = [m.notation for m in legal_moves]
        print("All legal moves are: ", disp_legal_moves)
    
    # update board should clear en passant square


if __name__ == "__main__":
    b = Board("r2qkbnr/1ppb1p1p/p1np4/4pPp1/3NP3/6P1/PPPP3P/RNBQKB1R b KQkq - 0 7")
    b.display()
    b.get_legal_moves()