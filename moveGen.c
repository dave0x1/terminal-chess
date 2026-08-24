

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
#include "move.h"
#include "board.h"
#include "attacks.h"



void generateKnightMoves(int square, MoveArray* arr){
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
