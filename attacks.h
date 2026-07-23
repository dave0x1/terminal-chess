#ifndef ATTACKS_H
#define ATTACKS_H


int knight_attacks(int square, int color);
int king_attacks(int square, int color);
int pawn_attacks(int square, int color);
int walk(int square, int direction, int target_piece1, int target_piece2);
int is_square_attacked(int square, int color);
int is_illegal_square(int square);


#endif