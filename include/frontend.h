#define init_pieceboard(game) {\
  game.boards[WHITE][0] = 0;\
  game.boards[WHITE][1] = 0xffff;\
  game.boards[WHITE][2] = 0;\
  game.boards[BLACK][0] = 0;\
  game.boards[BLACK][1] = 0xffff000000000000;\
  game.boards[BLACK][2] = 0;\
  game.piece_boards[WHITE][BISHOP][0] = 0;\
  game.piece_boards[WHITE][BISHOP][1] = 0x24;\
  game.piece_boards[WHITE][BISHOP][2] = 0;\
  game.piece_boards[BLACK][BISHOP][0] = 0;\
  game.piece_boards[BLACK][BISHOP][1] = 0x2400000000000000;\
  game.piece_boards[BLACK][BISHOP][2] = 0;\
  game.piece_boards[WHITE][ROOK][0] = 0;\
  game.piece_boards[WHITE][ROOK][1] = 0x81;\
  game.piece_boards[WHITE][ROOK][2] = 0;\
  game.piece_boards[BLACK][ROOK][0] =0;\
  game.piece_boards[BLACK][ROOK][1]= 0x8100000000000000;\
  game.piece_boards[BLACK][ROOK][2]= 0;\
  game.piece_boards[WHITE][QUEEN][0] = 0;\
  game.piece_boards[WHITE][QUEEN][1]= 0x10;\
  game.piece_boards[WHITE][QUEEN][2]= 0;\
  game.piece_boards[BLACK][QUEEN][0] = 0;\
  game.piece_boards[BLACK][QUEEN][1] = 0x1000000000000000;\
  game.piece_boards[BLACK][QUEEN][2] = 0;\
  game.piece_boards[WHITE][PAWN][0] = 0;\
  game.piece_boards[WHITE][PAWN][1]= 0xff00;\
  game.piece_boards[WHITE][PAWN][2]= 0;\
  game.piece_boards[BLACK][PAWN][0] = 0;\
  game.piece_boards[BLACK][PAWN][1] = 0xff000000000000;\
  game.piece_boards[BLACK][PAWN][2]= 0;\
  game.piece_boards[WHITE][KNIGHT][0] = 0;\
  game.piece_boards[WHITE][KNIGHT][1]= 0x42;\
  game.piece_boards[WHITE][KNIGHT][2]= 0;\
  game.piece_boards[BLACK][KNIGHT][0] = 0;\
  game.piece_boards[BLACK][KNIGHT][1]= 0x4200000000000000;\
  game.piece_boards[BLACK][KNIGHT][2]= 0;\
  game.piece_boards[WHITE][KING][0] = 0;\
  game.piece_boards[WHITE][KING][1]= 0x8;\
  game.piece_boards[WHITE][KING][2]=0;\
  game.piece_boards[BLACK][KING][0] = 0;\
  game.piece_boards[BLACK][KING][1]= 0x800000000000000;\
  game.piece_boards[BLACK][KING][2]= 0;\
  game.hv_sliders[WHITE][0] = 0;\
  game.hv_sliders[WHITE][1]= game.piece_boards[WHITE][ROOK][MIDDLE] |\
  game.piece_boards[WHITE][QUEEN][MIDDLE];\
  game.hv_sliders[WHITE][2]= 0;\
  game.hv_sliders[BLACK][0] = 0;\
  game.hv_sliders[BLACK][1]= game.piece_boards[BLACK][ROOK][MIDDLE] |\
  game.piece_boards[BLACK][QUEEN][MIDDLE];\
  game.hv_sliders[BLACK][2]= 0;\
  game.d_sliders[WHITE][0] = 0;\
  game.d_sliders[WHITE][1] = game.piece_boards[WHITE][BISHOP][MIDDLE] |\
  game.piece_boards[WHITE][QUEEN][MIDDLE];\
  game.d_sliders[WHITE][2] = 0;\
  game.d_sliders[BLACK][0] = 0;\
  game.d_sliders[BLACK][1]= game.piece_boards[BLACK][BISHOP][MIDDLE] |\
  game.piece_boards[BLACK][QUEEN][MIDDLE];\
  game.d_sliders[BLACK][2]= 0;\
  game.k_pos[WHITE][1] = 3;\
  game.k_pos[WHITE][0] = MIDDLE;\
  game.k_pos[BLACK][1] = 59;\
  game.k_pos[BLACK][0] = MIDDLE;\
}

