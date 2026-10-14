/*
File for storing reused utility functions
*/

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "moveArray.h"
#include "utility.h"
#include <stddef.h>
#include <stdint.h>

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

/*
    void parse_fen(char* fen, Board* b)
        init_empty_board(b)
    1. Piece placement:
        rank = 7        :rank counter
        file = 0        : file counter
        c = 0          : char pointer

        while rank >= 0
            square = (rank << 4) + file
            if fen[c] = '/'
                decrement rank
                file = 0
            else if fen[c] is a number
                add c to file
            else if fen[c] is a piece:
                convert piece char to integer
                insert_piece(piece, square, b)
                file++
            c++
    2. side to move
        int state = 0  : 0-side to move, 1-castling, 2-en passant 3-halfmove clock, 4-full move clock
        char* enpassant[3];
        enpassant[2] = '\0'
        while state < 5:
            if c == ' '
                c++
                continue
            switch state:
                case 0:
                    b->turn = WHITE if c == 'w' else BLACK
                    state++
                    break;
                case 1:
                    switch c:
                        case 'K':
                            b->castling |= WHITE_KINGSIDE
                            break;
                        case 'Q':
                            b->castling |= WHITE_QUEENSIDE
                            break;
                        case 'k':
                            b->castling |= BLACK_KINGSIDE
                            break;
                        case 'Q':
                            b->castling |= BLACK_QUEENSIDE
                            break;
                        default:
                            state++
                    break;
                case 2:
                    if c == '-':
                    state++
                        break;
                    else:
                        enpassant[0] = fen[c]
                        enpassant[1] = fen[c+1]
                        b->enpassant_target_square = string_to_square(enpassant)
                        c++
                    state++
                    break;
                case 3:
                    b->halfmove_counter = c - '0'
                    break;
                case 4:
                    b->fullmove_counter = c - '0'
                    break;
            c++
                


*/

/*
    - How to know if the fen has ended? maybe a Newline character
    - The fen fields are separated by spaces(" ")
    1. The function reads piece placement until a space(" ") is encountered
    2. Then turn
    3. Then castling
    4. Enpassant target square
    5. halfmove
    6. fullmove

    I could have separate functions for each field
    The functions take a start and end pointer to the substring being read
    The span starts at the first character and ends at the one past the last character
    e.g FEN_STATUS fen_place_pieces(char* start, char* end, Board* b)
    FEN_STATUS fen_set_side(char* start, char* end, Board* b)
    FEN_STATUS fen_set_castling(char* start, char* end, Board* b)
    FEN_STATUS fen_set_enpassant(char* start, char* end, Board* b)
    FEN_STATUS fen_set_halfmove(char* start, char* end, Board* b)
    FEN_STATUS fen_set_fullmove(char* start, char* end, Board* b)

    I could create an fen status enum(e.gFEN_PARSE_SUCCESS, FEN_PARSE_ERROR) (I feel like I have a lot of status enums at this point)
    The board is reset on failure
    Invalid ranks can be checked using 0x88 calculations
    Unavailable halfmove and fullmove fields will default to 0 and 1 respectively
*/

int char_to_piece(char p){
    for(int i = 0; i < piece_table_len; i++){
        if(p == piece_table[i]){
            return (i + WHITE_PAWN);
        }
    }
    return NONE;
}




Fen_status fen_place_pieces(char* start, char* end, Board* b){
    if(end - start == 0){
        return FEN_EMPTY_SPAN;
    }
    int rank = 7;
    int file = 0;
    int square;
    for(int i = 0; i < (end - start); i++){
        square = (rank << 4) + file;
        Pieces pc = char_to_piece(start[i]);
        if(start[i] == '/'){
            if(file == 8){
                rank--;
                file = 0;
            } else {
                return FEN_TOO_FEW_FILES;
            }
            
        } else if(pc != NONE){
            if(file > 7){
                return FEN_TOO_MANY_FILES;
            }
            InsertStatus status = insert_piece(pc, square, b);
            if(status == INSERT_OK){
                file++;
            } else if(status == INSERT_ERROR_DUPLICATE_KING){
                return FEN_DUPLICATE_KING;
            } else {
                return FEN_INSERT_ERROR; //Can only be reached through a bug
            }
        } else if(isdigit((unsigned char)start[i])){
            int digit = start[i] - '0';
            if(digit < 1 || digit > 8){
                return FEN_INVALID_DIGIT;
            }
            file+=digit;
        } else {
            return FEN_INVALID_PIECE;
        }
        if(file > 8){
            return FEN_TOO_MANY_FILES;
        }
        if(rank < 0){
            return FEN_TOO_MANY_RANKS;
        }
    }
    if(rank > 0){
        return FEN_TOO_FEW_RANKS;
    }
    if(file < 8){
        return FEN_TOO_FEW_FILES;
    }
    return FEN_PARSE_SUCCESS;
}
Fen_status fen_set_side(char* start, char* end, Board* b){
    /*
        A valid side span should simply be a character ('w' or 'b')
        Possible statuses(in order of check):
            FEN_EMPTY_SPAN, FEN_FIELD_TOO_LONG, FEN_INVALID_SIDE, FEN_PARSE_SUCCESS
        sample input        status
        "w"                FEN_PARSE_SUCCESS
        "b"                FEN_PARSE_SUCCESS
        "W"                FEN_INVALID_SIDE
        ""                 FEN_EMPTY_SPAN
        "x"                FEN_INVALID_SIDE
        "wb"               FEN_FIELD_TOO_LONG
        "w "               FEN_FIELD_TOO_LONG

        What changes: b->turn, left unchanged on failure
    */
    ptrdiff_t len = end - start;
    if(len == 0){
        return FEN_EMPTY_SPAN;
    }
    if(len > 1){
        return FEN_FIELD_TOO_LONG;
    }
    char side = start[0];
    if(side != 'w' && side != 'b'){
        return FEN_INVALID_SIDE;
    }
    b->turn = side == 'w' ? WHITE : BLACK;
    return FEN_PARSE_SUCCESS;
}
Fen_status fen_set_castling(char* start, char* end, Board* b){
    /*
        There are 4 castling values
            White kingside - K
            White queenside - Q
            Black kingside - k
            Black queenside - q
        The castling field contains either a '-'(indicating no castling) or a combination of the values
        The function is lenient on values order and repetition
        The main parsing function syncs up castling state if position doesn't match castling rights
        Possible statuses(in order of check):
            FEN_EMPTY_SPAN, FEN_FIELD_TOO_LONG, FEN_INVALID_CASTLING, FEN_PARSE_SUCCESS
        sample input               status
        "k"                       FEN_PARSE_SUCCESS
        "KQkq"                    FEN_PARSE_SUCCESS
        "qkQK"                    FEN_PARSE_SUCCESS
        "KKQQ"                    FEN_PARSE_SUCCESS
        "-"                       FEN_PARSE_SUCCESS
        "-K"                      FEN_INVALID_CASTLING
        "w"                       FEN_INVALID_CASTLING
        ""                        FEN_EMPTY_SPAN
        "KQkq "                   FEN_FIELD_TOO_LONG
        "Kw"                      FEN_INVALID_CASTLING

    */
    assert(end >= start);
    uint8_t castling = 0;
    ptrdiff_t len = end - start;
    int max_len = 4;
    if(len == 0){
        return FEN_EMPTY_SPAN;
    }
    if(len > max_len){
        return FEN_FIELD_TOO_LONG;
    }
    if(len == 1 && start[0] == '-'){
        b->castling = 0;
        return FEN_PARSE_SUCCESS;
    }
    for(int i = 0; i < len; i++){
        switch(start[i]){
            case 'K':
                castling |= WHITE_KINGSIDE;
                break;
            case 'Q':
                castling |= WHITE_QUEENSIDE;
                break;
            case 'k':
                castling |= BLACK_KINGSIDE;
                break;
            case 'q':
                castling |= BLACK_QUEENSIDE;
                break;
            default:
                return FEN_INVALID_CASTLING;
        }
    }
    b->castling = castling;
    return FEN_PARSE_SUCCESS;
    
}
Fen_status fen_set_enpassant(char* start, char* end, Board* b){
    /*
        The enpassant field contains either a '-', denoting no enpassant square; or a square on the third or 6th rank
        A maximum of two chars
        Possible statuses:
            FEN_EMPTY_SPAN, FEN_FIELD_TOO_SHORT, FEN_FIELD_TOO_LONG, FEN_INVALID_SQUARE, FEN_PARSE_SUCCESS
        sample input               status
        "-"                       FEN_PARSE_SUCCESS
        "e3"                      FEN_PARSE_SUCCESS
        "h6"                      FEN_PARSE_SUCCESS
        "w"                       FEN_FIELD_TOO_SHORT
        ""                        FEN_EMPTY_SPAN
        "a3 "                     FEN_FIELD_TOO_LONG
        "e4"                      FEN_INVALID_SQUARE

        0x88 square = 16 * rank + file
    */
    assert(end >= start);
    ptrdiff_t len = end - start;
    if(len == 0){
        return FEN_EMPTY_SPAN;
    }
    if(len == 1){
        if(start[0] == '-'){
            b->enpassant_target_square = -1;
            return FEN_PARSE_SUCCESS;
        } else {
            return FEN_FIELD_TOO_SHORT;
        }
        
    }
    if(len > 2){
        return FEN_FIELD_TOO_LONG;
    }
    if(start[0] < 'a' || start[0] > 'h'){
        return FEN_INVALID_SQUARE;
    }
    if(start[1] != '3' && start[1] != '6'){
        return FEN_INVALID_SQUARE;
    }
    int file = start[0] - 'a';
    int rank = start[1] - '1';
    int square = 16 * rank + file;
    b->enpassant_target_square = square;
    return FEN_PARSE_SUCCESS;

}
Fen_status fen_set_halfmove(char* start, char* end, Board* b){
    /*
        fen_set_halfmove expects at most 3 chars, all valid numbers: max 999
        Possible statuses:
            FEN_EMPTY_SPAN, FEN_FIELD_TOO_LONG, FEN_NOT_A_NUM, FEN_PARSE_SUCCESS
        sample input               status
        "0"                       FEN_PARSE_SUCCESS
        "45"                      FEN_PARSE_SUCCESS
        "100"                     FEN_PARSE_SUCCESS
        "w"                       FEN_NOT_A_NUM
        ""                        FEN_EMPTY_SPAN
        "10X"                     FEN_NOT_A_NUM
        "1000"                    FEN_FIELD_TOO_LONG
        "00001"                   FEN_FIELD_TOO_LONG
    */

    ptrdiff_t len = end - start;
    if(len <= 0){
        return FEN_EMPTY_SPAN;
    }
    if(len > 3){
        return FEN_FIELD_TOO_LONG;
    }
    int sum = 0;
    for(int i = 0; i < len; i++){
        if(start[i] == ' '){
            break;
        }
        if(start[i] < '0' || start[i] > '9'){
            return FEN_NOT_A_NUM;
        }
        int digit = start[i] - '0';
        sum = (sum * 10) + digit;
    }

    b->halfmove_counter = sum;
    return FEN_PARSE_SUCCESS;
}
Fen_status fen_set_fullmove(char* start, char* end, Board* b){
    /*
        fen_set_fullmove expects at most 4 chars, all numbers: max 9999
        Possible statuses:
            FEN_EMPTY_SPAN, FEN_FIELD_TOO_LONG, FEN_NOT_A_NUM, FEN_PARSE_SUCCESS
        sample input               status
        "0"                       FEN_PARSE_SUCCESS
        "45"                      FEN_PARSE_SUCCESS
        "100"                     FEN_PARSE_SUCCESS
        "w"                       FEN_NOT_A_NUM
        ""                        FEN_EMPTY_SPAN
        "10X"                     FEN_NOT_A_NUM
        "1000"                    FEN_PARSE_SUCCESS
        "00001"                   FEN_FIELD_TOO_LONG
    */
    ptrdiff_t len = end - start;
    if(len <= 0){
        return FEN_EMPTY_SPAN;
    }
    if(len > 4){
        return FEN_FIELD_TOO_LONG;
    }
    int sum = 0;
    for(int i = 0; i < len; i++){
        if(start[i] >= '0' && start[i] <= '9'){
            int digit = start[i] - '0';
            sum = (sum * 10) + digit;
        } else {
            return FEN_NOT_A_NUM;
        }
        
    }

    b->fullmove_counter = sum;
    return FEN_PARSE_SUCCESS;
}

Fen_status parse_fen(char* fen, Board* b){
    /*
        Fen parser main function
        Input: a string terminated by a Nul, newline or carriage return
        Splits the fen string and calls the function for each field in order:
            fen_place_pieces, 
            fen_set_side, 
            fen_set_castling, 
            fen_set_enpassant, 
            fen_set_halfmove, 
            fen_set_fullmove
        The first 4 fields are required, the last two (halfmoves and fullmoves) optional
        Parsing stops immediately the last field is set, extra text is ignored
        The caller's board is untouched on error, and replaced entirely on success
        Tasks after leaves succeed:
            - Missing optional fields are set to default
            - Sync castling
            - Copy board and set king pointers
        Possible statuses:
            All helper function errors, FEN_MISSING_FIELD, FEN_PARSE_SUCCESS
        sample input                                                                        status
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"                       FEN_PARSE_SUCCESS
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3"                        FEN_PARSE_SUCCESS
        "8/8/8/8/8/8/8/8 w KQkq -"                                                       FEN_PARSE_SUCCESS
        "8/8/8/8/8/8/8/8 w KQkq"                                                         FEN_MISSING_FIELD
        ""                                                                               FEN_MISSING_FIELD
    */
    typedef Fen_status (*parser)(char*, char*, Board*);
    parser parsers[6] = {fen_place_pieces, 
                        fen_set_side, 
                        fen_set_castling, 
                        fen_set_enpassant, 
                        fen_set_halfmove, 
                        fen_set_fullmove
    };

    int len = sizeof(parsers) / sizeof(parsers[0]);

    Board tempBoard;
    init_empty_board(&tempBoard);
    char* start = fen;
    char* end = fen;
    int slen = strlen(fen);
    char* final = fen + slen;

    for(int i = 0; i < len; i++){
        if(final - end <= 0){
            if(i < 4){
                return FEN_MISSING_FIELD;
            } else if(i == 4){
                tempBoard.halfmove_counter = 0;
                continue;
            } else if(i == 5){
                tempBoard.fullmove_counter = 1;
                break;
            }
        }
        while(end[0] != ' ' && end[0] != '\0' && end[0] != '\n' && end[0] != '\r'){
            end++;
        }
        Fen_status status = parsers[i](start, end, &tempBoard);

        if(status == FEN_PARSE_SUCCESS){
            printf("\nStep %d done\n", i);
        } else if(status == FEN_EMPTY_SPAN){
            if(i == 4){
                tempBoard.halfmove_counter = 0;
            } else if(i == 5){
                tempBoard.fullmove_counter = 1;
            } else {
                return FEN_MISSING_FIELD;
            }
            
        } else {
            return status;
        }

        start = ++end;
        
    }


    set_castling(WHITE, &tempBoard);
    set_castling(BLACK, &tempBoard);

    copy_board(&tempBoard, b);
    return FEN_PARSE_SUCCESS;
}