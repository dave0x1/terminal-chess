#ifndef BOARD_H
#define BOARD_H



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

typedef struct _piece_entry {
    Pieces piece_value; //e.g. 5: White queen
    int piece_location; // 52: e4 
} Piece_entry;

typedef struct _board {
    Piece_entry white_pieces[16];
    Piece_entry black_pieces[16];
    Piece_entry *board[128];
    Colors turn;
    int castling;
    int enpassant_target_square;
    int halfmove_counter;
    int fullmove_counter;
    Piece_entry *white_king;
    Piece_entry *black_king;
} Board;

void init_board(Board *board);
extern Board board;



#endif