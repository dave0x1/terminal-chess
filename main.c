#include <assert.h>
#include "board.h"
#include "attacks.h"
#include "repl.h"
#include "moveGen.h"
#include "utility.h"

void pieces_test(Board* b){
    // White starts first
    assert(b->turn == 0);

    // White piece values
    assert(b->white_pieces[0].piece_value == WHITE_ROOK);
    assert(b->white_pieces[1].piece_value == WHITE_KNIGHT);
    assert(b->white_pieces[2].piece_value == WHITE_BISHOP);
    assert(b->white_pieces[3].piece_value == WHITE_QUEEN);
    assert(b->white_pieces[4].piece_value == WHITE_KING);
    assert(b->white_pieces[5].piece_value == WHITE_BISHOP);
    assert(b->white_pieces[6].piece_value == WHITE_KNIGHT);
    assert(b->white_pieces[7].piece_value == WHITE_ROOK);

    // Black piece values
    assert(b->black_pieces[0].piece_value == BLACK_ROOK);
    assert(b->black_pieces[1].piece_value == BLACK_KNIGHT);
    assert(b->black_pieces[2].piece_value == BLACK_BISHOP);
    assert(b->black_pieces[3].piece_value == BLACK_QUEEN);
    assert(b->black_pieces[4].piece_value == BLACK_KING);
    assert(b->black_pieces[5].piece_value == BLACK_BISHOP);
    assert(b->black_pieces[6].piece_value == BLACK_KNIGHT);
    assert(b->black_pieces[7].piece_value == BLACK_ROOK);


    assert(is_illegal_square(25) != 0);
    assert(is_square_attacked(35, 0, b) == 1);
    assert(is_square_attacked(35, 1, b) == 1);
}

void move_test(){

}

void move_gen_test(){
    Board board;
    init_empty_board(&board);
    MoveArray* arr = createMoveArray(20);
    insert_piece(WHITE_KING, 4, &board);
    insert_piece(WHITE_ROOK, 0, &board);
    insert_piece(WHITE_ROOK, 7, &board);

    print_board(&board);

    // for(int i = 0; i < 16; i++){
    //     printf("value: %d \nlocation: %d \n\n", board.white_pieces[i].piece_value, board.white_pieces[i].piece_location);
    // }

    generate_all_moves(arr, &board);
    printArray(arr);
}

void gen_all_test(){
    Board test_board;
    init_board(&test_board);
    MoveArray* test_arr = createMoveArray(20);
    print_board(&test_board);
    generate_all_moves(test_arr, &test_board);
    printArray(test_arr);
}

int main(){
    // gen_all_test();
    move_gen_test();
}