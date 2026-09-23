

/*
Move generator should return a list of possible moves, for now these are pseudo-legal moves.
I'll be representing the list with a dynamic array.
An element of the array will be a pointer to a Move struct(defined in move.h)
The array also needs to have other info like the total number of space available, and space taken. 

The list will be realloc-ed when capacity - used == 1, doubling the capacity
A check if fired before every insert into the list, if 1 is returned, the list is realloc-ed before insertion is done

The move generators are separated by piece types (e.g generateKnightMoves(square))
They take a valid square value and return a pointer to the moveArray and appends moves to it(defined above)
*/

#include <stddef.h>
#include "moveGen.h"
#include "moveArray.h"
#include "board.h"
#include "attacks.h"

int is_enemy(int piece1, int piece2){
    if(piece1 < BLACK_PAWN && piece2 >= BLACK_PAWN) return 1; //p1 white, p2 black
    if(piece2 < BLACK_PAWN && piece1 >= BLACK_PAWN) return 1; //p2 white, p1 black
    return 0;
}

void generate_knight_moves(int square, MoveArray* arr, Board* b){
    //Get offsets and piece
    int offsets_array[] = {square+14, square+18, square+31, square+33, square-14, square-18, square-31, square-33};
    int len = 8;
    int piece = b->board[square]->piece_value; //White knight: 2, Black knight: 8
    

    for (int i = 0; i < len; i++){
        if(is_illegal_square(offsets_array[i]) != 0){
            continue;
        } else {
            int to_check = b->board[offsets_array[i]] == NULL ? -1 : b->board[offsets_array[i]]->piece_value;
            if(to_check == -1){//empty square
                Move move = {
                    .from = square, 
                    .to = offsets_array[i], 
                    .piece = piece,
                    .capture = NONE,
                    .promotion = NONE, 
                    .flags = 00
                };
                insertMove(move, arr);

            } else if((piece == 2 && to_check >= 7) || (piece == 8 && to_check <= 6)){
                //white knight, black piece or black knight, white piece
                Move move = {
                    .from = square, 
                    .to = offsets_array[i], 
                    .piece = piece,
                    .capture = to_check,
                    .promotion = NONE, 
                    .flags = 00
                };
                insertMove(move, arr);
            } else { //same color
                continue;
            }
        }
    }
}

// Castling not included
void generate_king_moves(int square, MoveArray* arr, Board* b){
    int king_offsets[] = {square+1, square-1, square+16, square-16, square+15, square-15, square+17, square-17};
    int len = 8;
    int piece = b->board[square]->piece_value;

    for(int i = 0; i < len; i++){
        if(is_illegal_square(king_offsets[i]) != 0){
            continue;
        } else {
            int to_check = b->board[king_offsets[i]] == NULL ? -1 : b->board[king_offsets[i]]->piece_value;
            if(to_check == -1){//empty square
                Move move = {
                    .from = square, 
                    .to = king_offsets[i], 
                    .piece = piece,
                    .capture = NONE,
                    .promotion = NONE, 
                    .flags = 00
                };
                insertMove(move, arr);

            } else if((piece == WHITE_KING && to_check >= BLACK_PAWN) || (piece == BLACK_KING && to_check <= WHITE_KING)){
                //white king, black piece or black king, white piece
                Move move = {
                    .from = square, 
                    .to = king_offsets[i], 
                    .piece = piece,
                    .capture = to_check,
                    .promotion = NONE, 
                    .flags = 00
                };
                insertMove(move, arr);
            } else { //same color
                continue;
            }
        }
    }
}

enum {
    KINGSIDE,
    QUEENSIDE
};

int check_castling(int square, Board* b, int color, int side){
    int enemy_color = color == WHITE ? BLACK : WHITE;
    if(is_square_attacked(square, enemy_color, b)) return 0;

    int squares[3];
    if(side == KINGSIDE){
        squares[0] = square+1;
        squares[1] = square+2;
    } else {
        squares[0] = square-1;
        squares[1] = square-2;
        squares[2] = square-3;
    }
    int len = side == KINGSIDE ? 2 : 3;

    for(int i = 0; i < len; i++){
        if(b->board[squares[i]] != NULL){
            return 0;
        }
    }
    for(int i = 0; i < 2; i++){
        if(is_square_attacked(squares[i], enemy_color, b)){
            return 0;
        }
    }
    return 1;
}

void generate_castling_moves(int square, MoveArray* arr, Board* b){
    int piece = b->board[square]->piece_value;
    int color = piece == WHITE_KING ? WHITE : BLACK;
    int kingside = piece == WHITE_KING ? WHITE_KINGSIDE : BLACK_KINGSIDE;
    int queenside = piece == WHITE_KING ? WHITE_QUEENSIDE : BLACK_QUEENSIDE;

    if(b->castling & kingside) {
        if(check_castling(square, b, color, KINGSIDE)) {
            Move move = {
                .from = square, 
                .to = square + 2, 
                .piece = piece,
                .capture = NONE,
                .promotion = NONE, 
                .flags = CASTLE
            };
            insertMove(move, arr);
        }
    }

    if(b->castling & queenside){
        if(check_castling(square, b, color, QUEENSIDE)) {
            Move move = {
                .from = square, 
                .to = square - 2, 
                .piece = piece,
                .capture = NONE,
                .promotion = NONE, 
                .flags = CASTLE
            };
            insertMove(move, arr);
        }
    }
}

void movegen_walk(int piece, int square, int direction, MoveArray* arr, Board* b){
    for(int i = 1; i <= 7; i++){
        int current_square = square + (i * direction);
        if(is_illegal_square(current_square) != 0) break; //Illegal square
        if(b->board[current_square] == NULL){ //Empty square
            Move move = {
                    .from = square, 
                    .to = current_square, 
                    .piece = piece,
                    .capture = NONE,
                    .promotion = NONE, 
                    .flags = 00
                };
                insertMove(move, arr);
        } else if(is_enemy(piece, b->board[current_square]->piece_value)){ //Enemy piece
            Move move = {
                    .from = square, 
                    .to = current_square, 
                    .piece = piece,
                    .capture = b->board[current_square]->piece_value,
                    .promotion = NONE, 
                    .flags = 00
                };
                insertMove(move, arr);
                break;
        } else break; //friendly piece
    }
}


void generate_bishop_moves(int square, MoveArray* arr, Board* b){
    int diag_directions[] = {15, 17, -15, -17};
    int len = 4;
    int piece = b->board[square]->piece_value;

    for(int i = 0; i < len; i++){
        movegen_walk(piece, square, diag_directions[i], arr, b);
    }
}

void generate_rook_moves(int square, MoveArray* arr, Board* b){
    int straight_directions[] = {16, 1, -16, -1};
    int len = 4;
    int piece = b->board[square]->piece_value;

    for(int i = 0; i < len; i++){
        movegen_walk(piece, square, straight_directions[i], arr, b);
    }
}

void generate_queen_moves(int square, MoveArray* arr, Board* b){
    generate_bishop_moves(square, arr, b);
    generate_rook_moves(square, arr, b);
}

/*
Pawn move generation

Case 1: Single pawn push: Check if front square is empty (+16 for white, -16 for black); move if it is
Case 2: Double pawn push: Check if the next two front squares are empty(+16 and +32 for white, -16 and -32);
        And if the pawn is on the home rank (2 for white, 7 for black); move to the second square, set enpassant square
        on the +16(-16 for black) square
Case 3: Capture: Check if the left or right front diag square(+15 or +17 for white, -15 or -17 for black)
        are occupied by enemy pieces. Capture if it is.
Case 4: En passant: The enpassant square is set after every double pawn push. 
        Check if either front diag square is the enpassant square, the move is possible if it is
Case 5: Promotion: After every pawn move, check if it lands on the last rank (8 for white, 1 for black),
        promote if it does
*/

void insert_pawn_move(int from, int to, Colors color, Pieces capture, Flags flag, MoveArray* arr){
    int piece = color == WHITE ? WHITE_PAWN : BLACK_PAWN;
    int next_rank = (to >> 4) + 1;
    Pieces promotion_list[4];
    int plist_len = 4;
    if(color == WHITE){
        promotion_list[0] = WHITE_QUEEN;
        promotion_list[1] = WHITE_ROOK;
        promotion_list[2] = WHITE_BISHOP;
        promotion_list[3] = WHITE_KNIGHT;
    } else {
        promotion_list[0] = BLACK_QUEEN;
        promotion_list[1] = BLACK_ROOK;
        promotion_list[2] = BLACK_BISHOP;
        promotion_list[3] = BLACK_KNIGHT;
    }
    if(next_rank == 8 || next_rank == 1){ //Last rank: promotion
        for(int i = 0; i < plist_len; i++){
            Move move = {
                .from = from,
                .to = to,
                .piece = piece,
                .capture = capture,
                .promotion = promotion_list[i],
                .flags = flag
            };
            insertMove(move, arr);
        }
    } else {
        Move move = {
            .from = from,
            .to = to,
            .piece = piece,
            .capture = capture,
            .promotion = NONE,
            .flags = flag
        };
        insertMove(move, arr);
    }
}

void generate_pawn_moves(int square, MoveArray* arr, Board* b){
    int piece = b->board[square]->piece_value;
    Colors color = piece == WHITE_PAWN ? WHITE: BLACK;
    int rank = (square >> 4) + 1; //0x88 rank calculation
    int single_push = color == WHITE ? square+16 : square-16;
    int double_push = color == WHITE ? square+32 : square-32;

    //Case 1&2: single and double pawn push
    if(b->board[single_push] == NULL){
        insert_pawn_move(square, single_push, color, NONE, DEFAULT, arr);
        if(((rank == 2 && color == WHITE) || (rank == 7 && color == BLACK)) && b->board[double_push] == NULL) {
            insert_pawn_move(square, double_push, color, NONE, DOUBLE_PAWN_PUSH, arr);
        }
    }

    //case 3: diagonal capture
    int offsets[2];
    if(color == WHITE){
        offsets[0] = 15;
        offsets[1] = 17;
    } else {
        offsets[0] = -15;
        offsets[1] = -17;
    }
    int len_offsets = 2;

    for(int i = 0; i < len_offsets; i++){
        int dest = square + offsets[i];

        //case 4: enpassant
        if(b->enpassant_target_square == dest){
            Pieces captured_pawn = color == WHITE ? BLACK_PAWN : WHITE_PAWN;
            insert_pawn_move(square, dest, color, captured_pawn, EN_PASSANT, arr);
        }

        if(is_illegal_square(dest) != 0 || b->board[dest] == NULL) continue;
        Pieces dest_piece = b->board[dest]->piece_value;
        if(is_enemy(piece, dest_piece)){
            insert_pawn_move(square, dest, color, dest_piece, DEFAULT, arr);
        }

    }

}

//Generate pseudo-legal moves
void generate_all_moves(MoveArray* arr, Board* b){
    Piece_entry* pieces_arr = b->turn == WHITE ? b->white_pieces : b->black_pieces;
    int used = b->turn == WHITE ? b->white_used : b->black_used;
    int max_len = 16;
    for(int i = 0; i < used; i++){
        Piece_entry p = pieces_arr[i];
        if(p.piece_location == NONE) continue;
        switch (p.piece_value) {
            case NONE:
                break;
            case WHITE_PAWN:
            case BLACK_PAWN:
                generate_pawn_moves(p.piece_location, arr, b);
                break;
            case WHITE_KNIGHT:
            case BLACK_KNIGHT:
                generate_knight_moves(p.piece_location, arr, b);
                break;
            case WHITE_BISHOP:
            case BLACK_BISHOP:
                generate_bishop_moves(p.piece_location, arr, b);
                break;
            case WHITE_ROOK:
            case BLACK_ROOK:
                generate_rook_moves(p.piece_location, arr, b);
                break;
            case WHITE_QUEEN:
            case BLACK_QUEEN:
                generate_queen_moves(p.piece_location, arr, b);
                break;
            case WHITE_KING:
            case BLACK_KING:
                generate_king_moves(p.piece_location, arr, b);
                generate_castling_moves(p.piece_location, arr, b);
                break;
            default: break;
        }
    }
}