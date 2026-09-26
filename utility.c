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
    char piece_table[] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};
    if(piece >= 1 && piece <= 12) {
        pc[0] = piece_table[piece-1];
        pc[1] = '\0';
        return RESULT_OK;
    } else if(piece == -1){
        pc[0] = 'x';
        pc[1] = '\0';
    }
    return RESULT_ERROR;
}

int flag_to_string(int flag, char* f, size_t max_len){
    switch (flag) {
        case 0:
            snprintf(f, max_len, "%s", "default");
            return RESULT_OK;
        case 1:
            snprintf(f, max_len, "%s", "castle");
            return RESULT_OK;
        case 2:
            snprintf(f, max_len, "%s", "double pawn push");
            return RESULT_OK;
        case 3:
            snprintf(f, max_len, "%s", "en passant");
            return RESULT_OK;
        default: return RESULT_ERROR;
    }
    return RESULT_ERROR;
}

void printMove(Move m){
    char from[3];
    char to[3];
    char piece[2];
    char capture[2];
    char promotion[2];
    char flag[20];
    square_to_string(m.from, from);
    square_to_string(m.to, to);
    piece_to_string(m.piece, piece);
    piece_to_string(m.capture, capture);
    piece_to_string(m.promotion, promotion);
    flag_to_string(m.flags, flag, 20);
    printf("{\nfrom: %s,\nto: %s,\npiece: %s, \ncapture: %s, \npromotion: %s,\nflags: %s \n},\n", from, to, piece, capture, promotion, flag);
}

void printArray(MoveArray* arr){
    printf("\n-------------------------Printing moves------------------------------\n");
    for(int i = 0; i < arr->used; i++){
        printf("%d", i);
        printMove(arr->list[i]);
    }
}