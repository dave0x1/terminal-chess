/*
Representation of the chessboard: 0x88
An array of 128 values

 112 113 114 115 116 117 118 119 | 120 121 122 123 124 125 126 127
  96  97  98  99 100 101 102 103 | 104 105 106 107 108 109 110 111
  80  81  82  83  84  85  86  87 |  88  89  90  91  92  93  94  95
  64  65  66  67  68  69  70  71 |  72  73  74  75  76  77  78  79
  48  49  50  51  52  53  54  55 |  56  57  58  59  60  61  62  63
  32  33  34  35  36  37  38  39 |  40  41  42  43  44  45  46  47
  16  17  18  19  20  21  22  23 |  24  25  26  27  28  29  30  31
   0   1   2   3   4   5   6   7 |   8   9  10  11  12  13  14  15

In base 16: 
 70 71 72 73 74 75 76 77 | 78 79 7a 7b 7c 7d 7e 7f
 60 61 62 63 64 65 66 67 | 68 69 6a 6b 6c 6d 6e 6f
 50 51 52 53 54 55 56 57 | 58 59 5a 5b 5c 5d 5e 5f
 40 41 42 43 44 45 46 47 | 48 49 4a 4b 4c 4d 4e 4f
 30 31 32 33 34 35 36 37 | 38 39 3a 3b 3c 3d 3e 3f
 20 21 22 23 24 25 26 27 | 28 29 2a 2b 2c 2d 2e 2f
 10 11 12 13 14 15 16 17 | 18 19 1a 1b 1c 1d 1e 1f
  0  1  2  3  4  5  6  7 |  8  9  a  b  c  d  e  f

The squares on the left board are legal, the ones on the right are not
Legality can easily be checked by AND-ing the square's value with 136(0x88 in hex, 10001000 in binary);
a non-zero result means the square is illegal. 

Pieces are stored as ints
White pawn = 1
White knight = 2
White bishop = 3
White rook = 4
White queen = 5
White king = 6

Black pawn = 7
Black knight = 8
Black bishop = 9
Black rook = 10
Black queen = 11
Black king = 12

Empty squares are stored as 0

- Player turn will be represented as a single bit; 0 for white's turn, 1 for black

- Castling rights will be represented as a 4-bit flag with ((White kingside) (White queenside) (Black kingside) (Black queenside))
for example: 1000 means white can castle kingside

- A halfmove counter that resets after every pawn push or capture

- A fullmove counter that increments by 1 every two moves


Two arrays[16] will be used to store all of white's and black's pieces
Board elements will store the address of the piece in each element, NULL for an empty square
*/

#include "board.h"
#include <stddef.h>
#include "attacks.h"

void init_board(Board *b){

    for(int i = 0; i < 128; i++) {
        b->board[i] = NULL;
    }

    // PAWNS
    for(int i = 0; i < 8; i++){
        b->white_pieces[i+8].piece_value = WHITE_PAWN;
        b->white_pieces[i+8].piece_location = i+16;

        b->black_pieces[i+8].piece_value = BLACK_PAWN;
        b->black_pieces[i+8].piece_location = i+96;
    }
    
    //WHITE PIECES
    b->white_pieces[0].piece_value = WHITE_ROOK;
    b->white_pieces[0].piece_location = 0;

    b->white_pieces[1].piece_value = WHITE_KNIGHT;
    b->white_pieces[1].piece_location = 1;
        
    b->white_pieces[2].piece_value = WHITE_BISHOP;
    b->white_pieces[2].piece_location = 2;
        
    b->white_pieces[3].piece_value = WHITE_QUEEN;
    b->white_pieces[3].piece_location = 3;
        
    b->white_pieces[4].piece_value = WHITE_KING;
    b->white_pieces[4].piece_location = 4;
        
    b->white_pieces[5].piece_value = WHITE_BISHOP;
    b->white_pieces[5].piece_location = 5;
        
    b->white_pieces[6].piece_value = WHITE_KNIGHT;
    b->white_pieces[6].piece_location = 6;
        
    b->white_pieces[7].piece_value = WHITE_ROOK;
    b->white_pieces[7].piece_location = 7;
    
    //BLACK PIECES
    b->black_pieces[0].piece_value = BLACK_ROOK;
    b->black_pieces[0].piece_location = 112;

    b->black_pieces[1].piece_value = BLACK_KNIGHT;
    b->black_pieces[1].piece_location = 113;
        
    b->black_pieces[2].piece_value = BLACK_BISHOP;
    b->black_pieces[2].piece_location = 114;
        
    b->black_pieces[3].piece_value = BLACK_QUEEN;
    b->black_pieces[3].piece_location = 115;
        
    b->black_pieces[4].piece_value = BLACK_KING;
    b->black_pieces[4].piece_location = 116;
        
    b->black_pieces[5].piece_value = BLACK_BISHOP;
    b->black_pieces[5].piece_location = 117;
        
    b->black_pieces[6].piece_value = BLACK_KNIGHT;
    b->black_pieces[6].piece_location = 118;
        
    b->black_pieces[7].piece_value = BLACK_ROOK;
    b->black_pieces[7].piece_location = 119;

    //BOARD ARRAY
    for(int i = 0; i <= 7; i++){
        b->board[i] = &b->white_pieces[i];
        b->board[i+112] = &b->black_pieces[i];
    }
    for(int i = 0; i <= 7; i++){
        b->board[i+16] = &b->white_pieces[i+8];
        b->board[i+96] = &b->black_pieces[i+8];
    }

    for(int i = 0; i <= 7; i++){
        if(b->white_pieces[i].piece_value == WHITE_KING){
            b->white_king = &b->white_pieces[i];
        }
        if(b->black_pieces[i].piece_value == BLACK_KING){
            b->black_king = &b->black_pieces[i];
        }
    }

    b->white_used = 16;
    b->black_used = 16;

    //Other board fields
    b->turn = WHITE;
    b->castling = 0b1111;
    b->enpassant_target_square = NONE;
    b->halfmove_counter = 0;
    b->fullmove_counter = 0;
}

void init_empty_board(Board *b){
    //Initialize all board pointers to NULL
    for(int i = 0; i < 128; i++) {
        b->board[i] = NULL;
    }

    b->white_pieces[0].piece_value = WHITE_KING;
    b->white_pieces[0].piece_location = NONE;

    b->black_pieces[0].piece_value = BLACK_KING;
    b->black_pieces[0].piece_location = NONE;

    //Set the king pointers
    b->white_king = &b->white_pieces[0];
    b->black_king = &b->black_pieces[0];

    //Set used indices to 1
    b->white_used = 1;
    b->black_used = 1;

    //Other board fields
    b->turn = WHITE;
    b->castling = 0b1111;
    b->enpassant_target_square = NONE;
    b->halfmove_counter = 0;
    b->fullmove_counter = 0;
}

/*
    What happens when a piece is captured?
    - Change the board pointer of the captured piece to the capturing one
    - Check if the index of the captured piece == piece_entry[used-1], if true, simply NULL that index and return
        - if false, store the piece_location of piece_entry[used-1] in a temp variable
        - copy the fields of piece_entry[used-1] into the captured piece's fields
        - NULL piece_entry[used-1]
        - Decrement used
        - Update board[temp] to the new address
    - Kings are never captured
*/

/*
insert_piece(piece, square)
    -Piece color can be derived from piece value
    -return error if square is occupied
    -Writes to _pieces[0] if piece is a king
        -Returns error if the king location is already valid
    -Writes to _pieces[used] for other pieces(piece_value & piece_location), used is incremented
    -board[square] will now contain a pointer to the newly inserted element &_piece[used] before increment
*/

InsertStatus insert_piece(int piece, int square, Board *b){

    //1. validate square
    if(is_illegal_square(square)) return INSERT_ERROR_ILLEGAL_SQUARE;

    //2. Validate piece
    if(piece < WHITE_PAWN || piece > BLACK_KING) return INSERT_ERROR_INVALID_PIECE;
    
    //3. check if square is occupied
    if(b->board[square] != NULL) return INSERT_ERROR_SQUARE_OCCUPIED;

    //4. get piece array
    Piece_entry *arr = piece < BLACK_PAWN ? b->white_pieces : b->black_pieces;
    int *used = piece < BLACK_PAWN ? &b->white_used : &b->black_used;
    //5. If king piece
    if(piece == WHITE_KING || piece == BLACK_KING){
        if(arr[0].piece_location != NONE) return INSERT_ERROR_DUPLICATE_KING;
        arr[0].piece_location = square;
        b->board[square] = &arr[0];
        return INSERT_OK;
    }

    //6. Other pieces
    arr[*used].piece_value = piece;
    arr[*used].piece_location = square;
    b->board[square] = &arr[*used];
    (*used)++;

    return INSERT_OK;

}

RemoveStatus remove_piece(int square, Board* b){
    //1. validate square
    if(is_illegal_square(square)) return REMOVE_ERROR_ILLEGAL_SQUARE;

    //2. check if square is empty
    if(b->board[square] == NULL) return REMOVE_ERROR_EMPTY_SQUARE;

    //remove from: board.square[], board.piece_entry[],  decrement used, if king just remove piece location;

    Piece_entry* entry = b->board[square];
    int color = entry->piece_value < BLACK_PAWN ? WHITE : BLACK;
    Piece_entry* array = color == WHITE ? b->white_pieces : b->black_pieces;
    int* used = color == WHITE ? &b->white_used : &b->black_used;

    if(entry->piece_value == WHITE_KING || entry->piece_value == BLACK_KING){
        //simply remove piece location for the king
        array[0].piece_location = NONE;
        b->board[square] = NULL;
        return REMOVE_OK;
    }
    
    int index = (int)(entry - array);
    int last = *used - 1;

   if (index != last) {
        int moved_square = array[last].piece_location;
        array[index] = array[last];
        b->board[moved_square] = &array[index];
    }

    array[last].piece_location = NONE;
    array[last].piece_value = NONE;

    b->board[square] = NULL;
    (*used)--;

    return REMOVE_OK;
}