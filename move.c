

/*
make_move(Board* b, Move m)
returns:
    status code: MAKE_MOVE_SUCCESS, MAKE_MOVE_ILLEGAL, MAKE_MOVE_ERROR
mutates:
    Board.board[128]-> two squares: Move.from and Move.to
    Board.white/black_pieces
    Board.white_king/Board.black_king
    Board.turn
    Board.castling
    Board.enpassant_target_square
    Board.halfmove_counter
    board.fullmove_counter


for unmake_move, I could have a board state array
typedef struct _board_history{
    Board list[];
    int capacity;
    int used
} board_history
where the current board state is pushed to before make_move runs

*/