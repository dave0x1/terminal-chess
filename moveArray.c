
/*
A utility for move generation
Move generator functions return a list of possible moves, for now these are pseudo-legal moves.
I'll be representing the list with a dynamic array.
An element of the array will be a pointer to a Move struct(defined in move.h)
The array also needs to have other info like the total number of space available, and space taken. 

The list will be realloc-ed when capacity - used == 1, doubling the capacity
A check if fired before every insert into the list, if 1 is returned, the list is realloc-ed before insertion is done

The move generators are separated by piece types (e.g generateKnightMoves(square))
They take a valid square value and return a pointer to the moveArray and appends moves to it(defined above)
*/


#include <stdio.h>
#include <stdlib.h>
#include "moveArray.h"

MoveArray* createMoveArray(int initial_capacity){
    MoveArray* moveArray = malloc(sizeof(MoveArray));
    Move* list = malloc(initial_capacity * sizeof(Move));
    if(moveArray != NULL && list != NULL){
        moveArray->capacity = initial_capacity;
        moveArray->used = 0;
        moveArray->list = list;
    } else {
        perror("malloc failed in createMoveArray");
        exit(1);
    }

    return moveArray;
}

void insertMove(Move m, MoveArray* arr){
    if((arr->capacity - arr->used) <= 1){
        Move* tempList = realloc(arr->list, 2 * arr->capacity * sizeof(Move));
        if(tempList != NULL){
            arr->list = tempList;
            arr->capacity*=2;
        } else {
            perror("malloc failed in insertMove");
            exit(1);
        }
    }

    arr->list[arr->used] = m;
    arr->used++;
}