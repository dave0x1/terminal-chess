#ifndef MOVE_H
#define MOVE_H

/*
    The move data structure
    from, to: square indexes. There will be a parser to convert algebraic notation into 0x88 index
    piece: what piece moved
    capture: returns 0 if 'to' square is empty, returns the enemy piece captured if not
    promotion: 0 or the piece type promoted to
    flags: flags to represent special moves: default(00) castle(01), double pawn push(10), en passant(11)
*/


typedef struct _move {
    int from;
    int to;
    int piece;
    int capture;
    int promotion;
    int flags;
} Move;

#endif


