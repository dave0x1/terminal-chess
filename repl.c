
/*
    print_board(Board* b)

    input - Pointer to a board struct
    output - prints an 8 by 8 board to the terminal
    return type - void

    what prints per square? The occupying piece letter separated by pipes or an empty square
    ranks are separated by hyphens and pluses at intersections

    loop shape:
    for(rank = 7; rank >= 0; rank--){
        printf("%d |", rank+1)
        for(file = 0; file <= 8; file++){
            if(file == 8) print file labels
            if(square is occupied){
                printf(" %c |", piece)
            } else {
                printf("  |")
             }
            
        }
        printf("\n")
    }
        file_to_char will be a simple helper function that converts the file numner to the corresponing char

    per square symbol: upper case letters for white pieces, lower case for black, unoccupied squares are empty spaces
    piece mapping will be done with a lookup table

    Rank and file labels will be printed also

    print_board will only print the board an do nothing else
*/
#include <stdio.h>
#include "board.h"
#include "repl.h"


void print_board(Board* b){
    // indexed by (piece_value - 1); relies on Pieces enum ordering: White P,N,B,R,Q,K then Black P,N,B,R,Q,K
    char piece_table[] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};
    for(int rank = 7; rank >= 0; rank--){
        printf("\n %d  |", rank+1);
        for(int file = 0; file < 8; file++){
            int square = (rank << 4) | file;
            if(b->board[square] == NULL){
                printf("   |");
            } else {
                printf(" %c |", piece_table[b->board[square]->piece_value - 1]);
            }            
        }
            printf("\n    +---+---+---+---+---+---+---+---+\n");

    }
    printf("\n      a   b   c   d   e   f   g   h \n");

}

void print_board_state(Board* b){
    printf("\n-------------Board state--------------\n");
    printf("White used: %d\n", b->white_used);
    printf("Black used: %d\n", b->black_used);
    printf("Turn: %c\n", b->turn == WHITE ? 'w' : 'b');
    printf("Castling: %d\n", b->castling);
    printf("enpassant_target_square: %d\n", b->enpassant_target_square);
    printf("Half moves: %d\n", b->halfmove_counter);
    printf("Full moves: %d\n", b->fullmove_counter);
}