#ifndef ATTACKS_H
#define ATTACKS_H

#include "board.h"

int knight_attacks(int square, int color, Board* b);
int king_attacks(int square, int color, Board* b);
int pawn_attacks(int square, int color, Board* b);
int walk(int square, int direction, int target_piece1, int target_piece2, Board* b);
int is_square_attacked(int square, int color, Board* b);
int is_illegal_square(int square);


#endif