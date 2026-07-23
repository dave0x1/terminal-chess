
/*
    piece_attacks(square, color)
    Function to check if a square is under attack by a piece
    square: The board index of the square
    color: The color of pieces being checked for, 0 for white, 1 for black; Black checks for white pieces and vice versa
    Returns: 0 if square is not attacked by enemy piece
            1 if square is attacked by enemy piece

*/

#include <stddef.h>
#include "attacks.h"
#include "board.h"


// static const int knight_jumps[] = {14, 18, 31, 33, -14, -18, -31, -33};
static const int diag_directions[] = {
    15, //Up left
    17, //Up right
    -15, //Down right
    -17 //Down left
    };
static const int straight_directions[] = {16, 1, -16, -1};

/*
    Legal squares return 0 when ANDed with 0x88, illegal squares return a nonzero value
*/
int is_illegal_square(int square){
    return(square & 0x88);
}

int knight_attacks(int square, int color){
    int offsets_array[] = {square+14, square+18, square+31, square+33, square-14, square-18, square-31, square-33};
    int len = 8;

    for (int i = 0; i < len; i++){
        if(is_illegal_square(offsets_array[i]) != 0){
            continue;
        } else {
            int to_check = board.board[offsets_array[i]] == NULL ? -1 : board.board[offsets_array[i]]->piece_value;
            if (color == WHITE && to_check == WHITE_KNIGHT){
                return 1;
            }
            if (color == BLACK && to_check == BLACK_KNIGHT){
                return 1;
            }
        }
    }
    return 0;
}

int king_attacks(int square, int color){
    int offsets_array[] = {square+1, square-1, square+16, square-16, square+15, square-15, square+17, square-17};
    int len = 8;

    for (int i = 0; i < len; i++){
        if(is_illegal_square(offsets_array[i]) != 0){
            continue;
        } else {
            int to_check = board.board[offsets_array[i]] == NULL ? -1 : board.board[offsets_array[i]]->piece_value;
            if (color == WHITE && to_check == WHITE_KING){
                return 1;
            }
            if (color == BLACK && to_check == BLACK_KING){
                return 1;
            }
        }
    }
    return 0;
}

int pawn_attacks(int square, int color){
    int pawn = color == WHITE ? WHITE_PAWN : BLACK_PAWN; //white pawn == 1; black pawn == 7

    int up_left = diag_directions[0];
    if(is_illegal_square(square + up_left) == 0 && board.board[square + up_left] != NULL){
        up_left = board.board[square + up_left]->piece_value;
    } else {
        up_left = -1;
    }

    int up_right = diag_directions[1];
    if(is_illegal_square(square + up_right) == 0 && board.board[square + up_right] != NULL){
        up_right = board.board[square + up_right]->piece_value;
    } else {
        up_right = -1;
    }

    int down_right = diag_directions[2];
    if(is_illegal_square(square + down_right) == 0 && board.board[square + down_right] != NULL){
        down_right = board.board[square + down_right]->piece_value;
    } else {
        down_right = -1;
    }

    int down_left = diag_directions[3];
    if(is_illegal_square(square + down_left) == 0 && board.board[square + down_left] != NULL){
        down_left = board.board[square + down_left]->piece_value;
    } else {
        down_left = -1;
    }

    switch (color){
    case 0: //White pawns attack 'up' so attacking white pawns will be 'down' and vice versa for black
        if (down_left == pawn || down_right == pawn){
            return 1;
        }
        break;
    case 1:
        if (up_left == pawn || up_right == pawn){
            return 1;
        }
        break;
    default:
        return 0;
        break;
    }
    return 0;
}


int walk(int square, int direction, int target_piece1, int target_piece2){
    for(int i = 1; i < 7; i++){
        int current_square = square + (i * direction);
        if(is_illegal_square(current_square) != 0) break;
        if (board.board[current_square] == NULL) continue;
        int piece = board.board[current_square]->piece_value;
        if(piece == target_piece1 || piece == target_piece2){
            return 1;
        } else break;
    }
    return 0;
}

int is_square_attacked(int square, int color){
    if(knight_attacks(square, color) == 1) return 1;
    if(king_attacks(square, color) == 1) return 1;
    if(pawn_attacks(square, color) == 1) return 1;

    int bishop = color == WHITE ? WHITE_BISHOP : BLACK_BISHOP;
    int rook = color == WHITE ? WHITE_ROOK : BLACK_ROOK;
    int queen = color == WHITE ? WHITE_QUEEN : BLACK_QUEEN;

    for(int i = 0; i < 4; i++){
        if(walk(square, diag_directions[i], bishop, queen) == 1) return 1;
    }

    for(int i = 0; i < 4; i++){
        if(walk(square, straight_directions[i], rook, queen) == 1) return 1;
    }

    return 0;
}
