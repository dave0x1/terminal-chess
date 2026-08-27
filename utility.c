/*
File for storing reused utility functions
*/

#include <stdio.h>
#include "moveArray.h"
#include "utility.h"

int square_to_string(int square, char* sq){
    if((square & 0x88) == 0){
        char file = (square & 7) + 'a';
        char rank = (square >> 4) + '1';
        sq[0] = file;
        sq[1] = rank;
        sq[2] = '\0';
        return RESULT_OK;
    } else{
        sq[0] = 'x';
        sq[1] = 'x';
        sq[2] = '\0';
    }
    return RESULT_ERROR;
}

int piece_to_string(int piece, char* pc){
    char piece_table[] = {'P', 'N', 'B', 'R', 'K', 'Q', 'p', 'n', 'b', 'r', 'k', 'q'};
    if(piece >= 1 && piece <= 12) {
        pc[0] = piece_table[piece-1];
        pc[1] = '\0';
        return RESULT_OK;
    }
    return RESULT_ERROR;
}

void printMove(Move m){
    char from[3];
    char to[3];
    char piece[2];
    square_to_string(m.from, from);
    square_to_string(m.to, to);
    piece_to_string(m.piece, piece);
    printf("{\nfrom: %s,\nto: %s,\npiece: %s, \ncapture: %d, \npromotion: %d,\nflags: %d \n},\n", from, to, piece, m.capture, m.promotion, m.flags);
}

void printArray(MoveArray* arr){
    printf("\n-------------------------Printing moves------------------------------\n");
    for(int i = 0; i < arr->used; i++){
        printMove(arr->list[i]);
    }
}