#ifndef UTILITY_H
#define UTILITY_H

#include "moveArray.h"

typedef enum {
    RESULT_ERROR = -1,
    RESULT_OK = 0,
}Result;

typedef enum {
    FEN_PARSE_SUCCESS,
    FEN_PARSE_ERROR,
    FEN_INVALID_PIECE,
    FEN_INVALID_DIGIT,
    FEN_TOO_MANY_FILES,
    FEN_TOO_MANY_RANKS,
    FEN_TOO_FEW_FILES,
    FEN_TOO_FEW_RANKS,
    FEN_INSERT_ERROR,
    FEN_DUPLICATE_KING,
    FEN_INVALID_SIDE,
    FEN_EMPTY_SPAN,
    FEN_FIELD_TOO_LONG,
    FEN_FIELD_TOO_SHORT,
    FEN_NOT_A_NUM,
    FEN_INVALID_CASTLING,
    FEN_INVALID_SQUARE,
    FEN_MISSING_FIELD,
    FEN_NOT_ENPASSANT_SQUARE
}Fen_status;


static char fen_arr[19][26] = {
    "FEN_PARSE_SUCCESS",
    "FEN_PARSE_ERROR",
    "FEN_INVALID_PIECE",
    "FEN_INVALID_DIGIT",
    "FEN_TOO_MANY_FILES",
    "FEN_TOO_MANY_RANKS",
    "FEN_TOO_FEW_FILES",
    "FEN_TOO_FEW_RANKS",
    "FEN_INSERT_ERROR",
    "FEN_DUPLICATE_KING",
    "FEN_INVALID_SIDE",
    "FEN_EMPTY_SPAN",
    "FEN_FIELD_TOO_LONG",
    "FEN_FIELD_TOO_SHORT",
    "FEN_NOT_A_NUM",
    "FEN_INVALID_CASTLING",
    "FEN_INVALID_SQUARE",
    "FEN_MISSING_FIELD",
    "FEN_NOT_ENPASSANT_SQUARE"
};

void printArray(MoveArray* arr);
int char_to_piece(char p);
int piece_to_string(int piece, char* pc);
Fen_status fen_place_pieces(char* start, char* end, Board* b);
Fen_status fen_set_side(char* start, char* end, Board* b);
Fen_status fen_set_halfmove(char* start, char* end, Board* b);
Fen_status fen_set_fullmove(char* start, char* end, Board* b);
Fen_status parse_fen(char* fen, Board* b);


static char piece_table[] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};
static int piece_table_len = 12;
#endif