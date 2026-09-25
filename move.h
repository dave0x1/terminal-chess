#ifndef MOVE_H
#define MOVE_H

#include "moveArray.h"
#include <stdint.h>
/*
    The move data structure
    from, to: square indexes. There will be a parser to convert algebraic notation into 0x88 index
    piece: what piece moved
    capture: returns 0 if 'to' square is empty, returns the enemy piece captured if not
    promotion: 0 or the piece type promoted to
    flags: flags to represent special moves: default(00) castle(01), double pawn push(10), en passant(11)
*/


// typedef struct _move {
//     int from;
//     int to;
//     int piece;
//     int capture;
//     int promotion;
//     int flags;
// } Move;

#define MOVE_LIMIT 32768
typedef enum {
    MAKE_MOVE_ERROR,
    MAKE_MOVE_SUCCESS
} MAKE_MOVE_STATUS;

typedef struct _prev_move{
    Move move_made;
    uint8_t prev_castling_rights;
    int prev_enpassant_square;
    int prev_halfmove_counter;
} Prev_move;

typedef struct _board_history{
    Prev_move list[MOVE_LIMIT];
    int used;
} Board_history;


MAKE_MOVE_STATUS make_move(Move m, Board* b, Board_history* bh);
#endif