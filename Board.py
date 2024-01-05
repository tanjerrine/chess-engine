import numpy as np
from bitarray import bitarray
import Piece

FEN_TO_PIECE = {"p": "pawn", "n": "knight", "b": "bishop", "r": "rook", "q": "queen", "k": "king"}

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
    def __init__(self, fen=None):
        if fen is None:
            self.turn = "w"
            self.check = False
            self.checkmate = False
            self.can_castle = "KQkq"
            self.en_passant = "-"

            self.w_pieces = {"pawn": bitarray("0" * 8 + "1" * 8 + "0" * 48), 
                            "knight": bitarray("01000010" + "0" * 56),
                            "bishop": bitarray("00100100" + "0" * 56),
                            "rook": bitarray("10000001" + "0" * 56),
                            "queen": bitarray("00010000" + "0" * 56),
                            "king": bitarray("00001000" + "0" * 56)}

            self.b_pieces = {"pawn": bitarray("0" * 48 + "1" * 8 + "0" * 8), 
                            "knight": bitarray("0" * 56 + "01000010"),
                            "bishop": bitarray("0" * 56 + "00100100"),
                            "rook": bitarray("0" * 56 + "10000001"),
                            "queen": bitarray("0" * 56 + "00010000"),
                            "king": bitarray("0" * 56 + "00001000")}
        # TODO: initialize from FEN
        else:
            board_str, self.turn, self.can_castle, self.en_passant, _, _ = fen.split()
            
            self.w_pieces = {"pawn": bitarray("0"*64), 
                            "knight": bitarray("0"*64),
                            "bishop": bitarray("0"*64),
                            "rook": bitarray("0"*64),
                            "queen": bitarray("0"*64),
                            "king": bitarray("0"*64)}

            self.b_pieces = {"pawn": bitarray("0"*64), 
                            "knight": bitarray("0"*64),
                            "bishop": bitarray("0"*64),
                            "rook": bitarray("0"*64),
                            "queen": bitarray("0"*64),
                            "king": bitarray("0"*64)}
            
            rows = board_str.split("/")
            ind = 56
            for row in rows:
                for c in row:
                    if 49 <= ord(c) <= 56:
                        ind += int(c)
                        # print("ind increased from {} to {}".format(ind-int(c), ind))
                    elif 65 <= ord(c) <= 90:
                        self.w_pieces[FEN_TO_PIECE[c.lower()]][ind] = True
                        # print("set {}(ind {}) to {}".format(ind_to_sq(ind), ind, FEN_TO_PIECE[c.lower()]))
                        ind += 1
                    else:
                        self.b_pieces[FEN_TO_PIECE[c]][ind] = True
                        # print("set {}(ind {}) to {}".format(ind_to_sq(ind), ind, FEN_TO_PIECE[c]))
                        ind += 1
                ind -= 16


    # TODO: use bitarray for position (ind) instead of an int
    # given ind between 0 to 63, get piece abbreviation on that square, or Null piece if none. 
    def piece_at_square(self, ind, display_only=False):
        for piece, bb in self.w_pieces.items():
            if bb[ind] == 1:
                return Piece.Piece("w", piece) if not display_only else Piece.display_notation(piece, "w")
            
        for piece, bb in self.b_pieces.items():
            if bb[ind] == 1:
                return Piece.Piece("b", piece) if not display_only else Piece.display_notation(piece, "b")
        
        return Piece.Piece() if not display_only else "."

    def display(self):
        for r in range(7, -1, -1):
            print("{} | ".format(r + 1), end="")
            for c in range(8):
                # TODO: optimize piece_at_square to not instantiate object just to print
                print(self.piece_at_square(8 * r + c, True), end=" ")
            print("")
        
        print(" " * 4 + "- " * 8)
        print(" " * 4 + "a b c d e f g h")
    
    def get_legal_moves(self):
        legal_moves = []
        p_dict = self.w_pieces if self.turn == "w" else self.b_pieces
        for piece_name, piece_board in p_dict.items():
            for i in range(64):
                if piece_board[i] == 1:
                    legal_moves.extend(Piece.Piece(self.turn, piece_name).legal_moves(i, self))
        disp_legal_moves = [m.notation for m in legal_moves]
        print("All legal moves are: ", disp_legal_moves)
    
    # update board should clear en passant square


if __name__ == "__main__":
    b = Board("r2qkbnr/1ppb1p1p/p1np4/4pPp1/3NP3/6P1/PPPP3P/RNBQKB1R b KQkq - 0 7")
    b.display()
    b.get_legal_moves()