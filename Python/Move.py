from Board import ind_to_sq

class Move:
    def __init__(self, piece, start, finish, capture, promote):
        self.piece = piece
        self.start = start
        self.finish = finish
        self.capture = capture
        self.promote = promote
        self.notation = self.get_notation()

    # TODO: add promotion
    # TODO: resolve if multi piece can go to same square
    def get_notation(self):
        f_sq = ind_to_sq(self.finish)
        if not self.capture:
            notation = self.piece.alg_not + f_sq
        else:
            notation = (self.piece.alg_not if self.piece.type != "pawn" else ind_to_sq(self.start)[0]) + "x" + f_sq
        return notation