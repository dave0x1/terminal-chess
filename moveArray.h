#ifndef MOVEARRAY_H
#define MOVEARRAY_H

#include "board.h"

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

typedef struct _moveArray{
    int capacity;
    int used;
    Move* list;
} MoveArray;

MoveArray* createMoveArray(int initial_capacity);
void insertMove(Move m, MoveArray* arr);

#endif