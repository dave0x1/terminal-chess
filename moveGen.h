#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "moveArray.h"

void generateKnightMoves(int square, MoveArray* arr);
void generate_bishop_moves(int square, MoveArray* arr);
void generate_rook_moves(int square, MoveArray* arr);
void generate_queen_moves(int square, MoveArray* arr);
void generate_pawn_moves(int square, MoveArray* arr);

#endif