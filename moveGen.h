#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "moveArray.h"

void generate_knight_moves(int square, MoveArray* arr, Board* b);
void generate_bishop_moves(int square, MoveArray* arr, Board* b);
void generate_rook_moves(int square, MoveArray* arr, Board* b);
void generate_queen_moves(int square, MoveArray* arr, Board* b);
void generate_pawn_moves(int square, MoveArray* arr, Board* b);
void generate_all_moves(MoveArray* arr, Board* b);

#endif