#define init_pieceboard(game) {\
  game.boards[WHITE][TOP] = 0;\
  game.boards[WHITE][MIDDLE] = 0xffff;\
  game.boards[WHITE][BOTTOM] = 0;\
  game.boards[BLACK][TOP] = 0;\
  game.boards[BLACK][MIDDLE] = 0xffff000000000000;\
  game.boards[BLACK][BOTTOM] = 0;\
  game.piece_boards[WHITE][BISHOP][TOP] = 0;\
  game.piece_boards[WHITE][BISHOP][MIDDLE] = 0x24;\
  game.piece_boards[WHITE][BISHOP][BOTTOM] = 0;\
  game.piece_boards[BLACK][BISHOP][TOP] = 0;\
  game.piece_boards[BLACK][BISHOP][MIDDLE] = 0x2400000000000000;\
  game.piece_boards[BLACK][BISHOP][BOTTOM] = 0;\
  game.piece_boards[WHITE][ROOK][TOP] = 0;\
  game.piece_boards[WHITE][ROOK][MIDDLE] = 0x81;\
  game.piece_boards[WHITE][ROOK][BOTTOM] = 0;\
  game.piece_boards[BLACK][ROOK][TOP] =0;\
  game.piece_boards[BLACK][ROOK][MIDDLE]= 0x8100000000000000;\
  game.piece_boards[BLACK][ROOK][BOTTOM]= 0;\
  game.piece_boards[WHITE][QUEEN][TOP] = 0;\
  game.piece_boards[WHITE][QUEEN][MIDDLE]= 0x10;\
  game.piece_boards[WHITE][QUEEN][BOTTOM]= 0;\
  game.piece_boards[BLACK][QUEEN][TOP] = 0;\
  game.piece_boards[BLACK][QUEEN][MIDDLE] = 0x1000000000000000;\
  game.piece_boards[BLACK][QUEEN][BOTTOM] = 0;\
  game.piece_boards[WHITE][PAWN][TOP] = 0;\
  game.piece_boards[WHITE][PAWN][MIDDLE]= 0xff00;\
  game.piece_boards[WHITE][PAWN][BOTTOM]= 0;\
  game.piece_boards[BLACK][PAWN][TOP] = 0;\
  game.piece_boards[BLACK][PAWN][MIDDLE] = 0xff000000000000;\
  game.piece_boards[BLACK][PAWN][BOTTOM]= 0;\
  game.piece_boards[WHITE][KNIGHT][TOP] = 0;\
  game.piece_boards[WHITE][KNIGHT][MIDDLE]= 0x42;\
  game.piece_boards[WHITE][KNIGHT][BOTTOM]= 0;\
  game.piece_boards[BLACK][KNIGHT][TOP] = 0;\
  game.piece_boards[BLACK][KNIGHT][MIDDLE]= 0x4200000000000000;\
  game.piece_boards[BLACK][KNIGHT][BOTTOM]= 0;\
  game.piece_boards[WHITE][KING][TOP] = 0;\
  game.piece_boards[WHITE][KING][MIDDLE]= 0x8;\
  game.piece_boards[WHITE][KING][BOTTOM]=0;\
  game.piece_boards[BLACK][KING][TOP] = 0;\
  game.piece_boards[BLACK][KING][MIDDLE]= 0x800000000000000;\
  game.piece_boards[BLACK][KING][BOTTOM]= 0;\
  game.hv_sliders[WHITE][TOP] = 0;\
  game.hv_sliders[WHITE][MIDDLE]= game.piece_boards[WHITE][ROOK][MIDDLE] |\
  game.piece_boards[WHITE][QUEEN][MIDDLE];\
  game.hv_sliders[WHITE][BOTTOM]= 0;\
  game.hv_sliders[BLACK][TOP] = 0;\
  game.hv_sliders[BLACK][MIDDLE]= game.piece_boards[BLACK][ROOK][MIDDLE] |\
  game.piece_boards[BLACK][QUEEN][MIDDLE];\
  game.hv_sliders[BLACK][BOTTOM]= 0;\
  game.d_sliders[WHITE][TOP] = 0;\
  game.d_sliders[WHITE][MIDDLE] = game.piece_boards[WHITE][BISHOP][MIDDLE] |\
  game.piece_boards[WHITE][QUEEN][MIDDLE];\
  game.d_sliders[WHITE][BOTTOM] = 0;\
  game.d_sliders[BLACK][TOP] = 0;\
  game.d_sliders[BLACK][MIDDLE]= game.piece_boards[BLACK][BISHOP][MIDDLE] |\
  game.piece_boards[BLACK][QUEEN][MIDDLE];\
  game.d_sliders[BLACK][BOTTOM]= 0;\
  game.k_pos[WHITE][MIDDLE] = 3;\
  game.k_pos[WHITE][TOP] = MIDDLE;\
  game.k_pos[BLACK][MIDDLE] = 59;\
  game.k_pos[BLACK][TOP] = MIDDLE;\
}

