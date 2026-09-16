

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
#include "moveArray.h"
#include "board.h"
#include "attacks.h"

int is_enemy(int piece1, int piece2){
    if(piece1 < BLACK_PAWN && piece2 >= BLACK_PAWN) return 1; //p1 white, p2 black
    if(piece2 < BLACK_PAWN && piece1 >= BLACK_PAWN) return 1; //p2 white, p1 black
    return 0;
}

void generateKnightMoves(int square, MoveArray* arr){
    //Get offsets and piece
    int offsets_array[] = {square+14, square+18, square+31, square+33, square-14, square-18, square-31, square-33};
    int len = 8;
    int piece = board.board[square]->piece_value; //White knight: 2, Black knight: 8
    

    for (int i = 0; i < len; i++){
        if(is_illegal_square(offsets_array[i]) != 0){
            continue;
        } else {
            int to_check = board.board[offsets_array[i]] == NULL ? -1 : board.board[offsets_array[i]]->piece_value;
            if(to_check == -1){//empty square
                Move move = {
                    .from = square, 
                    .to = offsets_array[i], 
                    .piece = piece,
                    .capture = 0,
                    .promotion = 0, 
                    .flags = 00
                };
                insertMove(move, arr);

            } else if((piece == 2 && to_check >= 7) || (piece == 8 && to_check <= 6)){
                //white knight, black piece or black knight, white piece
                Move move = {
                    .from = square, 
                    .to = offsets_array[i], 
                    .piece = piece,
                    .capture = 1,
                    .promotion = 0, 
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
void generate_king_moves(int square, MoveArray* arr){
    int king_offsets[] = {square+1, square-1, square+16, square-16, square+15, square-15, square+17, square-17};
    int len = 8;
    int piece = board.board[square]->piece_value;

    for(int i = 0; i < len; i++){
        if(is_illegal_square(king_offsets[i]) != 0){
            continue;
        } else {
            int to_check = board.board[king_offsets[i]] == NULL ? -1 : board.board[king_offsets[i]]->piece_value;
            if(to_check == -1){//empty square
                Move move = {
                    .from = square, 
                    .to = king_offsets[i], 
                    .piece = piece,
                    .capture = 0,
                    .promotion = 0, 
                    .flags = 00
                };
                insertMove(move, arr);

            } else if((piece == WHITE_KING && to_check >= BLACK_PAWN) || (piece == BLACK_KING && to_check <= WHITE_KING)){
                //white king, black piece or black king, white piece
                Move move = {
                    .from = square, 
                    .to = king_offsets[i], 
                    .piece = piece,
                    .capture = 1,
                    .promotion = 0, 
                    .flags = 00
                };
                insertMove(move, arr);
            } else { //same color
                continue;
            }
        }
    }
}

void movegen_walk(int piece, int square, int direction, MoveArray* arr){
    for(int i = 1; i <= 7; i++){
        int current_square = square + (i * direction);
        if(is_illegal_square(current_square) != 0) break; //Illegal square
        if(board.board[current_square] == NULL){ //Empty square
            Move move = {
                    .from = square, 
                    .to = current_square, 
                    .piece = piece,
                    .capture = 0,
                    .promotion = 0, 
                    .flags = 00
                };
                insertMove(move, arr);
        } else if(is_enemy(piece, board.board[current_square]->piece_value)){ //Enemy piece
            Move move = {
                    .from = square, 
                    .to = current_square, 
                    .piece = piece,
                    .capture = 1,
                    .promotion = 0, 
                    .flags = 00
                };
                insertMove(move, arr);
                break;
        } else break; //friendly piece
    }
}


void generate_bishop_moves(int square, MoveArray* arr){
    int diag_directions[] = {15, 17, -15, -17};
    int len = 4;
    int piece = board.board[square]->piece_value;

    for(int i = 0; i < len; i++){
        movegen_walk(piece, square, diag_directions[i], arr);
    }
}

void generate_rook_moves(int square, MoveArray* arr){
    int straight_directions[] = {16, 1, -16, -1};
    int len = 4;
    int piece = board.board[square]->piece_value;

    for(int i = 0; i < len; i++){
        movegen_walk(piece, square, straight_directions[i], arr);
    }
}

void generate_queen_moves(int square, MoveArray* arr){
    generate_bishop_moves(square, arr);
    generate_rook_moves(square, arr);
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

void generate_pawn_moves(int square, MoveArray* arr){
    int piece = board.board[square]->piece_value;
    Colors color = piece == WHITE_PAWN ? WHITE: BLACK;
    int rank = (square >> 4) + 1; //0x88 rank calculation
    int single_push = color == WHITE ? square+16 : square-16;
    int double_push = color == WHITE ? square+32 : square-32;

    //Case 1&2: single and double pawn push
    if(board.board[single_push] == NULL){
        insert_pawn_move(square, single_push, color, NONE, DEFAULT, arr);
        if(((rank == 2 && color == WHITE) || (rank == 7 && color == BLACK)) && board.board[double_push] == NULL) {
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
        if(board.enpassant_target_square == dest){
            Pieces captured_pawn = color == WHITE ? BLACK_PAWN : WHITE_PAWN;
            insert_pawn_move(square, dest, color, captured_pawn, EN_PASSANT, arr);
        }

        if(is_illegal_square(dest) != 0 || board.board[dest] == NULL) continue;
        Pieces dest_piece = board.board[dest]->piece_value;
        if(is_enemy(piece, dest_piece)){
            insert_pawn_move(square, dest, color, dest_piece, DEFAULT, arr);
        }

    }

}

void generate_all_moves(MoveArray* arr){

}

/*
board state refactor:
List of functions that access the global board state in movegen.c
    generate_knight_moves
    generate_king_moves
    movegen_walk
    generate_bishop_moves
    generate_rook_moves
    generate_queen_moves
    generate_pawn_moves

In attacks.c
    knight_attacks
    king_attacks
    pawn_attacks
    walk
    
*/