

#include "board.h"
#include "moveArray.h"
#include "move.h"
#include <stddef.h>
#include <stdint.h>
// #include <stdlib.h>

/*
make_move(Move m, Board* b)
returns:
    status code: MAKE_MOVE_SUCCESS, MAKE_MOVE_ERROR
mutates:
    Board.board[128]-> two squares: Move.from and Move.to
    Board.white/black_pieces
    Board.white_king/Board.black_king
    Board.turn
    Board.castling
    Board.enpassant_target_square
    Board.halfmove_counter
    board.fullmove_counter


for unmake_move

typedef struct _prev_move{
    Move move_made;
    uint8_t prev_castling_rights;
    int prev_enpassant_square;
    int prev_halfmove_counter
} Prev_move
typedef struct _board_history{
    Prev_move list[];
    int capacity;
    int used
} Board_history

*/
enum {
    KINGSIDE,
    QUEENSIDE
};

void set_castling(Colors color, Board* b){
    int left_rook_home_square;
    int right_rook_home_square;
    int king_home_square;
    if(color == WHITE){
        left_rook_home_square = 0;
        right_rook_home_square = 7;
        king_home_square = 4;
    
        if(b->board[king_home_square] == NULL || b->board[king_home_square]->piece_value != WHITE_KING){
            b->castling &= ~WHITE_KINGSIDE;
            b->castling &= ~WHITE_QUEENSIDE;
        }
        if(b->board[left_rook_home_square] == NULL || b->board[left_rook_home_square]->piece_value != WHITE_ROOK){
            b->castling &= ~WHITE_QUEENSIDE;
        } 
        if(b->board[right_rook_home_square] == NULL || b->board[right_rook_home_square]->piece_value != WHITE_ROOK){
            b->castling &= ~WHITE_KINGSIDE;
        }
    } else {
        left_rook_home_square = 112;
        right_rook_home_square = 119;
        king_home_square = 116;

        if(b->board[king_home_square] == NULL || b->board[king_home_square]->piece_value != BLACK_KING){
            b->castling &= ~BLACK_KINGSIDE;
            b->castling &= ~BLACK_QUEENSIDE;
        }
        if(b->board[left_rook_home_square] == NULL || b->board[left_rook_home_square]->piece_value != BLACK_ROOK){
            b->castling &= ~BLACK_QUEENSIDE;
        } 
        if(b->board[right_rook_home_square] == NULL || b->board[right_rook_home_square]->piece_value != BLACK_ROOK){
            b->castling &= ~BLACK_KINGSIDE;
        }
    }
}


int insert_history(Prev_move prev, Board_history* bh){
    if(bh->used < MOVE_LIMIT){
        bh->list[bh->used] = prev;
        bh->used++;
        return 1;
    }
    return 0;
}

MAKE_MOVE_STATUS make_move(Move m, Board* b, Board_history* bh){
    Prev_move previous = {
        .move_made = m,
        .prev_castling_rights = b->castling,
        .prev_enpassant_square = b->enpassant_target_square,
        .prev_halfmove_counter = b->halfmove_counter
    };
    if(insert_history(previous, bh) == 0){
        return MAKE_MOVE_ERROR;
    }
    Piece_entry* piece = b->board[m.from];
    Colors color = m.piece < BLACK_PAWN ? WHITE : BLACK;
    int enpassant_square = NONE;
    switch (m.flags) {
        case CASTLE:
            Piece_entry* rook;
            int side = m.from < m.to ? KINGSIDE : QUEENSIDE;
            if(side == KINGSIDE){
                if(color == WHITE){
                    rook = b->board[7];
                } else {
                    rook = b->board[119];
                }
            } else {
                if(color == WHITE){
                    rook = b->board[0];
                } else {
                    rook = b->board[112];
                }
            }
            
            int prev_rook_location = rook->piece_location;
            rook->piece_location = side == KINGSIDE ? m.to - 1 : m.to + 1;
            b->board[rook->piece_location] = rook;
            b->board[prev_rook_location] = NULL;
            break;
        case DEFAULT:
            if(m.capture != -1){
                remove_piece(m.to, b);
            }
            break;
        case EN_PASSANT:
            int captured_square = color == WHITE ? m.to - 16 : m.to + 16;
            remove_piece(captured_square, b);
            break;
        case DOUBLE_PAWN_PUSH:
            enpassant_square = color == WHITE ? m.to - 16 : m.to + 16;
            break;
        default: return MAKE_MOVE_ERROR;
    }
    if(m.promotion != NONE){
        piece->piece_value = m.promotion;
    }
    piece->piece_location = m.to;
    b->board[m.from] = NULL;
    b->board[m.to] = piece;
    set_castling(color, b);
    b->turn = color == WHITE ? BLACK : WHITE;
    b->enpassant_target_square = enpassant_square;
    if(m.capture != -1 || m.piece == WHITE_PAWN || m.piece == BLACK_PAWN){
        b->halfmove_counter = 0;
    } else {
        b->halfmove_counter++;
    }
    if(color == BLACK){
        b->fullmove_counter++;
    }

    return MAKE_MOVE_SUCCESS;
}