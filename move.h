#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>
#include "board.h"
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
    DEFAULT,
    CASTLE,
    DOUBLE_PAWN_PUSH,
    EN_PASSANT
} Flags;


typedef struct _move {
    int from;
    int to;
    Pieces piece;
    Pieces capture;
    Pieces promotion;
    Flags flags;
} Move;

typedef enum {
    MAKE_MOVE_ERROR,
    MAKE_MOVE_SUCCESS
} MAKE_MOVE_STATUS;

typedef enum {
    UNMAKE_MOVE_ERROR,
    UNMAKE_MOVE_SUCCESS
} UNMAKE_MOVE_STATUS;

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
UNMAKE_MOVE_STATUS unmake_move(Board* b, Board_history* bh);
void set_castling(Colors color, Board* b);
#endif