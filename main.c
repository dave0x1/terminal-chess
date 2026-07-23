#include <stdio.h>
#include <assert.h>
#include "board.h"
#include "attacks.h"
#include "move.h"

int main(){
    init_board(&board);

    // White starts first
    assert(board.turn == 0);

    // White piece values
    assert(board.white_pieces[0].piece_value == WHITE_ROOK);
    assert(board.white_pieces[1].piece_value == WHITE_KNIGHT);
    assert(board.white_pieces[2].piece_value == WHITE_BISHOP);
    assert(board.white_pieces[3].piece_value == WHITE_QUEEN);
    assert(board.white_pieces[4].piece_value == WHITE_KING);
    assert(board.white_pieces[5].piece_value == WHITE_BISHOP);
    assert(board.white_pieces[6].piece_value == WHITE_KNIGHT);
    assert(board.white_pieces[7].piece_value == WHITE_ROOK);

    // Black piece values
    assert(board.black_pieces[0].piece_value == BLACK_ROOK);
    assert(board.black_pieces[1].piece_value == BLACK_KNIGHT);
    assert(board.black_pieces[2].piece_value == BLACK_BISHOP);
    assert(board.black_pieces[3].piece_value == BLACK_QUEEN);
    assert(board.black_pieces[4].piece_value == BLACK_KING);
    assert(board.black_pieces[5].piece_value == BLACK_BISHOP);
    assert(board.black_pieces[6].piece_value == BLACK_KNIGHT);
    assert(board.black_pieces[7].piece_value == BLACK_ROOK);


    assert(is_illegal_square(25) != 0);
    assert(is_square_attacked(35, 0) == 1);
    assert(is_square_attacked(35, 1) == 1);

}