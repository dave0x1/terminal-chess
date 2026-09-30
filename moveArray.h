#ifndef MOVEARRAY_H
#define MOVEARRAY_H

#include "board.h"
#include "move.h"

typedef struct _moveArray{
    int capacity;
    int used;
    Move* list;
} MoveArray;

MoveArray* createMoveArray(int initial_capacity);
void insertMove(Move m, MoveArray* arr);
void generate_legal_moves(MoveArray* arr, Board* b, Board_history* bh);

#endif