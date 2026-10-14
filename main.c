#include <assert.h>
#include "board.h"
#include "attacks.h"
#include "repl.h"
#include "moveGen.h"
#include "utility.h"
#include "move.h"
#include <string.h>
#include <stdio.h>

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

void move_gen_test(){
    Board board;
    init_empty_board(&board);
    MoveArray* arr = createMoveArray(20);
    insert_piece(WHITE_KING, 4, &board);
    insert_piece(WHITE_ROOK, 0, &board);
    insert_piece(WHITE_ROOK, 7, &board);
    insert_piece(BLACK_ROOK, 20, &board);

    print_board(&board);

    // for(int i = 0; i < 16; i++){
    //     printf("value: %d \nlocation: %d \n\n", board.white_pieces[i].piece_value, board.white_pieces[i].piece_location);
    // }
    board.turn = BLACK;

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

void move_test(){
    Board board;
    Board_history history;
    history.used = 0;
    init_empty_board(&board);
    // insert_piece(WHITE_ROOK, 16, &board);
    insert_piece(WHITE_KING, 0, &board);
    insert_piece(BLACK_QUEEN, 33, &board);
    MoveArray* arr = createMoveArray(20);
    set_castling(WHITE, &board);
    print_board(&board);
    print_board_state(&board);
    printf("\nHistory.used: %d\n", history.used);

    generate_all_moves(arr, &board);
    printArray(arr);
    generate_legal_moves(arr, &board, &history);
    printArray(arr);
    print_board(&board);
    print_board_state(&board);
    printf("\nHistory.used: %d\n", history.used);
    
}

typedef struct{
    char* start;
    Fen_status expected_status;
} Test_fen;

void fen_test(){
    Test_fen s[] = {{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", FEN_PARSE_SUCCESS},
                    {"rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R", FEN_PARSE_SUCCESS},
                    {"rnbqkbnrr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", FEN_TOO_MANY_FILES},
                    {"9/8/8/8/8/8/8/8", FEN_INVALID_DIGIT},
                    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBN", FEN_TOO_FEW_FILES},
                    {"8/8", FEN_TOO_FEW_RANKS},
                    {"rnbqkznr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", FEN_INVALID_PIECE},
                    {"8/8/8/8/8/8/8/8/8", FEN_TOO_MANY_RANKS},
                    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNRR", FEN_TOO_MANY_FILES},
                    {"PPPPPPPP/PPPPPPPP/PPPPPPPP/PPPPPPPP/8/8/8/8", FEN_PARSE_SUCCESS},
                    {"88/8/8/8/8/8/8/8", FEN_TOO_MANY_FILES},
                    {"8/8/8/8/8/8/8/8/p", FEN_TOO_MANY_RANKS}};

    Test_fen side[] = {{"w", FEN_PARSE_SUCCESS},
                    {"b", FEN_PARSE_SUCCESS},
                    {"wb", FEN_FIELD_TOO_LONG},
                    {"", FEN_EMPTY_SPAN},
                    {"W", FEN_INVALID_SIDE}};

    Test_fen move_c[] = {{"0", FEN_PARSE_SUCCESS},
                    {"100", FEN_PARSE_SUCCESS},
                    {"wb", FEN_NOT_A_NUM},
                    {"", FEN_EMPTY_SPAN},
                    {"-5", FEN_NOT_A_NUM}};

    
    int len = sizeof(s) / sizeof(s[0]);
    int len1 = sizeof(side) / sizeof(side[0]);

    Board b;
    init_empty_board(&b);
    for(int i = 0; i < len; i++){
        init_empty_board(&b);
        Fen_status status = fen_place_pieces(s[i].start, s[i].start + strlen(s[i].start), &b);
        if(status != s[i].expected_status){
            printf("\n%s failed\n returned status: %d", s[i].start, status);
            print_board(&b);
        }
    }
    for(int i = 0; i < len1; i++){
        init_empty_board(&b);
        Fen_status status = fen_set_side(side[i].start, side[i].start + strlen(side[i].start), &b);
        if(status != side[i].expected_status){
            printf("\n%s failed\n returned status: %d", side[i].start, status);
            print_board_state(&b);
        }
    }
    for(int i = 0; i < len1; i++){
        init_empty_board(&b);
        Fen_status status = fen_set_fullmove(move_c[i].start, move_c[i].start + strlen(move_c[i].start), &b);
        if(status != move_c[i].expected_status){
            printf("\n%s failed\n returned status: %d", move_c[i].start, status);
            print_board_state(&b);
        }
    }
    
}

void fen_parse_test(){
    Board b;
    init_empty_board(&b);
    char s[] = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3";
    Fen_status status = parse_fen(s, &b);
    printf("\n%s\n", fen_arr[status]);
    print_board(&b);
    print_board_state(&b);
}
int main(){
    // gen_all_test();
    // move_gen_test();
    // move_test();
    // fen_test();
    fen_parse_test();
}