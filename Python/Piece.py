from Board import ind_is_valid, ind_to_sq
from Move import Move
PIECE_VALUES = {"pawn": 1, "knight": 3, "bishop": 3, "rook": 5, "queen": 9, "king": 255}
# each piece stores tuple (range, step, sideways adjustment list)
PIECE_MOVES = {"bishop": (7, 1, [1]), "rook": (7, 1, [0]), "queen": (7, 1, [0, 1]), "king": (1, 1, [0, 1]), "knight": (1, 2, [1, -1])}
PROMOTABLES = ["knight", "bishop", "rook", "queen"]

NOT_A_FILE = 0xfefefefefefefefe
NOT_H_FILE = 0x7f7f7f7f7f7f7f7f

def n_one(b): return b << 8
def s_one(b): return b >> 8
def e_one(b): return (b << 1) & NOT_A_FILE
def ne_one(b): return (b << 9) & NOT_A_FILE
def se_one(b): return (b >> 7) & NOT_A_FILE
def w_one(b): return (b >> 1) & NOT_H_FILE
def nw_one(b): return (b << 7) & NOT_H_FILE
def sw_one(b): return (b >> 9) & NOT_H_FILE

def display_notation(type, color):
    letter = "n" if type == "knight" else type[0] 
    return letter if color == "b" else chr(ord(letter)-32)

def rank_of(ind):
    return ind // 8 + 1

def opp_color(color):
    if color == "w": return "b"
    if color == "b": return "w"

class Piece:
    def __init__(self, color=None, type=None):
        self.color = color
        self.type = type
        self.dis_not = (self.get_dis_notation() if type is not None else ".")
        self.alg_not = ("" if self.type == "pawn" else self.dis_not.upper())
        self.value = (PIECE_VALUES[self.type] if self.type is not None else 0)

    def get_dis_notation(self):
        return display_notation(self.type, self.color)
    
    # TODO: add castling
    def legal_moves(self, pos, board):
        if self.type == "pawn": return pawn_legal_moves(pos, board)
        moves = []
        cardinal = [8, -8, 1, -1]
        sideways_unit = [-1, 1, 8, -8]
        scope, step, side_del = PIECE_MOVES[self.type]
        for card, side_dir in zip(cardinal, sideways_unit):
            for side_vel in side_del:
                for i in range(1, scope + 1):
                    move_ind = pos + i * (card * step + side_dir * side_vel)
                    if (not ind_is_valid(move_ind)) or (board.piece_at_square(move_ind).color == self.color):
                        break
                    elif board.piece_at_square(move_ind).color == None:
                        moves.append(Move(self, pos, move_ind, False, None))
                    else:
                        moves.append(Move(self, pos, move_ind, True, None))
                        break
        return moves
        
def check_piece_color(piece, color):
    if color not in {"w", "b"}:
        raise Exception("Piece color must be black or white") 
    extra = 32 if color == "w" else 0
    return 122 >= ord(piece) + extra >= 97

# Legal move helpers. Moves stored as [piece][start_pos][x][end_pos], where the x exists if
# and only if the move is a capture.
# TODO: add promotion
def pawn_legal_moves(pos, board):
    scope = 1
    if board.turn == "w":
        direction = 1 
        if 8 <= pos < 16: scope = 2
    else:
        direction = -1
        if 48 <= pos < 56: scope = 2
    piece = Piece(board.turn, "pawn")
    moves = []
    next = pos
    for i in range(scope):
        next += (8 * direction) 
        if i == 0:
            for diff in [1, -1]:
                c_ind = next + diff
                if ind_is_valid(c_ind):
                    if ind_to_sq(c_ind) == board.en_passant or board.piece_at_square(c_ind).color == opp_color(piece.color):
                        moves.append(Move(piece, pos, c_ind, True, None))
            
        next_square = board.piece_at_square(next)
        if ind_is_valid(next):
            if next_square.type is None:
                # next square open
                moves.append(Move(piece, pos, next, False, None))
            else:
                # path is blocked, no need to look further
                return moves
       
    return moves