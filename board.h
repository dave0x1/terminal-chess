#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

typedef enum {
    WHITE = 0,
    BLACK
} Colors;

typedef enum {
    NONE = -1,
    WHITE_PAWN = 1,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN, // 7
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING
} Pieces;

typedef enum {
    WHITE_KINGSIDE = (1 << 0),
    WHITE_QUEENSIDE = (1 << 1),
    BLACK_KINGSIDE = (1 << 2),
    BLACK_QUEENSIDE = (1 << 3)
} Castling;

typedef struct _piece_entry {
    Pieces piece_value; //e.g. 5: White queen
    int piece_location; // 52: e4
} Piece_entry;

typedef struct _board {
    Piece_entry white_pieces[16];
    int white_used;
    Piece_entry black_pieces[16];
    int black_used;
    Piece_entry *board[128];
    Colors turn;
    uint8_t castling;
    int enpassant_target_square;
    int halfmove_counter;
    int fullmove_counter;
    Piece_entry *white_king;
    Piece_entry *black_king;
} Board;

typedef enum {
    INSERT_OK,
    INSERT_ERROR_ILLEGAL_SQUARE,
    INSERT_ERROR_INVALID_PIECE,
    INSERT_ERROR_SQUARE_OCCUPIED,
    INSERT_ERROR_DUPLICATE_KING,
} InsertStatus;

typedef enum {
    REMOVE_OK,
    REMOVE_ERROR_ILLEGAL_SQUARE,
    REMOVE_ERROR_EMPTY_SQUARE,
} RemoveStatus;

void init_board(Board *board);
void init_empty_board(Board *b);
InsertStatus insert_piece(int piece, int square, Board *b);
RemoveStatus remove_piece(int square, Board* b);



#endif