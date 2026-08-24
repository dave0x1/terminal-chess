#ifndef MOVEARRAY_H
#define MOVEARRAY_H

typedef struct _move {
    int from;
    int to;
    int piece;
    int capture;
    int promotion;
    int flags;
} Move;

typedef struct _moveArray{
    int capacity;
    int used;
    Move* list;
} MoveArray;

MoveArray* createMoveArray(int initial_capacity);
void insertMove(Move m, MoveArray* arr);

#endif