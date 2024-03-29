#include <evaluate.h>

/*
* Title: evalute()
* Params:
* -> BOARD_ARGS *args
*   -> Array of board args which houses
*      bitboards to represent the current
*      board position
* -> SIDE to_move
*   -> Enum to represent which side's turn
*      it is now
* -> uint32_t p_flags
*   -> bitstring representation of if the
*      player is in check, double check,
*      or checkmate
* -> uint32_t e_flags
*   -> Same as above, but for the enemy
* -> unsigned int turn
*   -> Represents what turn number the game
*      is currently at
* Description:
* Determines which player currently has the advantage
* in the current board position. Also, incorporates
* the rating system for current position to evaluate
* king safety, piece position, amongst other things
*/

MOVE evaluate(BOARD_ARGS *args, SIDE to_move,
              uint32_t p_flags, uint32_t e_flags, unsigned int turn) {
  MOVE move;
  move.rating = get_weighted_rating(turn, to_move, args);
  if ((p_flags & MATE) && to_move == WHITE) {
    move.score = INT_MIN;
    return move;
  } else if ((p_flags & MATE)) {
    move.score = INT_MAX;
    return move;
  }
  if ((e_flags & MATE) && to_move == WHITE) {
    move.score = INT_MAX;
    return move;
  } else if (e_flags & MATE) {
    move.score = INT_MIN;
    return move;
  }

  int b_num[2] = {
    get_piece_num(args->piece_boards[WHITE][BISHOP]),
    get_piece_num(args->piece_boards[BLACK][BISHOP])
  };
  int r_num[2] = {
    get_piece_num(args->piece_boards[WHITE][ROOK]),
    get_piece_num(args->piece_boards[BLACK][ROOK])
  };
  int q_num[2] = {
    get_piece_num(args->piece_boards[WHITE][QUEEN]),
    get_piece_num(args->piece_boards[BLACK][QUEEN])
  };
  int p_num[2] = {
    get_piece_num(args->piece_boards[WHITE][PAWN]),
    get_piece_num(args->piece_boards[BLACK][PAWN])
  };
  int n_num[2] = {
    get_piece_num(args->piece_boards[WHITE][KNIGHT]),
    get_piece_num(args->piece_boards[BLACK][KNIGHT])
  };

  int material_score = (B_VAL * (b_num[0] - b_num[1])) +
                       (R_VAL * (r_num[0] - r_num[1])) +
                       (Q_VAL * (q_num[0] - q_num[1])) +
                       (P_VAL * (p_num[0] - p_num[1])) +
                       (K_VAL * (n_num[0] - n_num[1]));

  if ((p_flags & D_CHECK) && to_move == WHITE) {
    material_score -= (2 * Q_VAL);
  } else if (p_flags & D_CHECK) {
    material_score += (2 * Q_VAL);
  } else if ((p_flags & CHECK) && to_move == WHITE) {
    material_score -= Q_VAL;
  } else if (p_flags & CHECK) {
    material_score += Q_VAL;
  }

  if ((e_flags & D_CHECK) && to_move == WHITE) {
    material_score += (2 * Q_VAL);
  } else if (e_flags & D_CHECK) {
    material_score -= (2 * Q_VAL);
  } else if ((e_flags & CHECK) && to_move == WHITE) {
    material_score += Q_VAL;
  } else if (e_flags & CHECK) {
    material_score -= Q_VAL;
  }
  move.score = material_score;
  return move;
}

/*
* Title: get_piece_num()
* Params:
* -> uint64_t *board
*   -> Set of three bitboards which are
*      to be analyzed
* Description:
* Simple helper to find out how many pieces
* are on the passed in board
*/

unsigned int get_piece_num(uint64_t *board) {
  unsigned int sum = 0;
  uint64_t ONE = 1;
  for (int i = 0; i < 3; i++) {
    while (board[i]) {
      board[i] ^= (ONE << log2_lookup(board[i]));
      sum++;
    }
  }
  return sum;
}

/*
* Title: get_piece_num_sboard()
* Params:
* -> uint64_t board
*   -> Single bitboard which is to
*      be analyzed
* Description:
* Simple helper to find out how many pieces
* are on the passed in bitboard
*/

unsigned int get_piece_num_sboard(uint64_t board) {
  unsigned int sum = 0;
  uint64_t ONE = 1;
  while (board) {
    board ^= (ONE << log2_lookup(board));
    sum++;
  }
  return sum;
}

/*
* Title: get_weighted_rating()
* Params:
* -> unsigned int turn
*   -> represents what the current turn number the
*      game is on
* -> SIDE player
*   -> Enum to represent who's turn is up next
* -> BOARD_ARGS *args
*   -> struct of bitboards which represents the current
*      board position
* Description:
* Helper function to figure out the rating of the current
* position, not taking into account, necessarily, how many
* pieces are on the board. Primarly, it focuses on king
* positioning and pawn positioning. The weighting seen at
* the top of the function changes based on the current
* move and is applied to pieces differently based on how
* active they need to be later in the game
*/

unsigned int get_weighted_rating(unsigned int turn,
                                 SIDE player, BOARD_ARGS *args) {
  unsigned int pos_multiplier = (unsigned int)(0.022 * (turn * turn));
  unsigned int neg_multiplier = (unsigned int)(40/turn);
  //int neg_multiplier = (unsigned int)(-0.0001 * (turn * turn * turn) + 4);
  neg_multiplier = 1;
  unsigned int pn_multiplier = (unsigned int) neg_multiplier;
  unsigned int pr = 0;
  unsigned int nr = 0;
  unsigned int br = 0;
  unsigned int rr = 0;
  unsigned int qr = 0;
  unsigned int kr = 0;
  unsigned int temp = 0;
  int rating_order[6];
  if (neg_multiplier < 0) {
    neg_multiplier = 0;
  }
  if (player == WHITE) {
    rating_order[0] = 0;
    rating_order[1] = 2;
    rating_order[2] = 4;
    rating_order[3] = 5;
    rating_order[4] = 7;
    rating_order[5] = 8;
  } else {
    rating_order[0] = 1;
    rating_order[1] = 3;
    rating_order[2] = 4;
    rating_order[3] = 6;
    rating_order[4] = 7;
    rating_order[5] = 9;
  }
  temp = get_piece_num_sboard((ratings[rating_order[0]][0]
                               & args->piece_boards[player][BISHOP][BOTTOM]));
  br += temp;
  temp = get_piece_num_sboard((ratings[rating_order[0]][0]
                               & args->piece_boards[player][BISHOP][MIDDLE]));
  br += temp;
  temp = get_piece_num_sboard((ratings[rating_order[0]][0]
                               & args->piece_boards[player][BISHOP][TOP]));
  br += temp;
  temp = get_piece_num_sboard((ratings[rating_order[0]][1]
                               & args->piece_boards[player][BISHOP][BOTTOM]));
  br += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[0]][1]
                               & args->piece_boards[player][BISHOP][MIDDLE]));
  br += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[0]][1]
                               & args->piece_boards[player][BISHOP][TOP]));
  br += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[0]][2]
                               & args->piece_boards[player][BISHOP][BOTTOM]));
  br += (temp * 3);
  temp = get_piece_num_sboard((ratings[rating_order[0]][2]
                               & args->piece_boards[player][BISHOP][MIDDLE]));
  br += (temp * 3);
  temp = get_piece_num_sboard((ratings[rating_order[0]][2]
                               & args->piece_boards[player][BISHOP][TOP]));
  br += (temp * 3);
  temp = get_piece_num_sboard((ratings[rating_order[1]][0]
                               & args->piece_boards[player][ROOK][BOTTOM]));
  rr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[1]][0]
                               & args->piece_boards[player][ROOK][MIDDLE]));
  rr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[1]][0]
                               & args->piece_boards[player][ROOK][TOP]));
  rr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[1]][1]
                               & args->piece_boards[player][ROOK][BOTTOM]));
  rr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[1]][1]
                               & args->piece_boards[player][ROOK][MIDDLE]));
  rr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[1]][1]
                               & args->piece_boards[player][ROOK][TOP]));
  rr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[1]][2]
                               & args->piece_boards[player][ROOK][BOTTOM]));
  rr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[1]][2]
                               & args->piece_boards[player][ROOK][MIDDLE]));
  rr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[1]][2]
                               & args->piece_boards[player][ROOK][TOP]));
  rr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[2]][0]
                               & args->piece_boards[player][QUEEN][BOTTOM]));
  qr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[2]][0]
                               & args->piece_boards[player][QUEEN][MIDDLE]));
  qr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[2]][0]
                               & args->piece_boards[player][QUEEN][TOP]));
  qr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[2]][1]
                               & args->piece_boards[player][QUEEN][BOTTOM]));
  qr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[2]][1]
                               & args->piece_boards[player][QUEEN][MIDDLE]));
  qr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[2]][1]
                               & args->piece_boards[player][QUEEN][TOP]));
  qr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[2]][2]
                               & args->piece_boards[player][QUEEN][BOTTOM]));
  qr += (temp * 4);
  temp = get_piece_num_sboard((ratings[rating_order[2]][2]
                               & args->piece_boards[player][QUEEN][MIDDLE]));
  qr += (temp * 4);
  temp = get_piece_num_sboard((ratings[rating_order[2]][2]
                               & args->piece_boards[player][QUEEN][TOP]));
  qr += (temp * 4);
  temp = get_piece_num_sboard((ratings[rating_order[3]][0]
                               & args->piece_boards[player][PAWN][BOTTOM]));
  pr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[3]][0]
                               & args->piece_boards[player][PAWN][MIDDLE]));
  pr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[3]][0]
                               & args->piece_boards[player][PAWN][TOP]));
  pr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[3]][1]
                               & args->piece_boards[player][PAWN][BOTTOM]));
  pr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[3]][1]
                               & args->piece_boards[player][PAWN][MIDDLE]));
  pr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[3]][1]
                               & args->piece_boards[player][PAWN][TOP]));
  pr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[3]][2]
                               & args->piece_boards[player][PAWN][BOTTOM]));
  pr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[3]][2]
                               & args->piece_boards[player][PAWN][MIDDLE]));
  pr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[3]][2]
                               & args->piece_boards[player][PAWN][TOP]));
  pr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[4]][0]
                               & args->piece_boards[player][KNIGHT][BOTTOM]));
  nr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[4]][0]
                               & args->piece_boards[player][KNIGHT][MIDDLE]));
  nr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[4]][0]
                               & args->piece_boards[player][KNIGHT][TOP]));
  nr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[4]][1]
                               & args->piece_boards[player][KNIGHT][BOTTOM]));
  nr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[4]][1]
                               & args->piece_boards[player][KNIGHT][MIDDLE]));
  nr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[4]][1]
                               & args->piece_boards[player][KNIGHT][TOP]));
  nr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[4]][2]
                               & args->piece_boards[player][KNIGHT][BOTTOM]));
  nr += (temp * 3);
  temp = get_piece_num_sboard((ratings[rating_order[4]][2]
                               & args->piece_boards[player][KNIGHT][MIDDLE]));
  nr += (temp * 3);
  temp = get_piece_num_sboard((ratings[rating_order[4]][2]
                               & args->piece_boards[player][KNIGHT][TOP]));
  nr += (temp * 3);
  temp = get_piece_num_sboard((ratings[rating_order[5]][0]
                               & args->piece_boards[player][KING][BOTTOM]));
  kr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[5]][0]
                               & args->piece_boards[player][KING][MIDDLE]));
  kr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[5]][0]
                               & args->piece_boards[player][KING][TOP]));
  kr += temp;
  temp = get_piece_num_sboard((ratings[rating_order[5]][1]
                               & args->piece_boards[player][KING][BOTTOM]));
  kr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[5]][1]
                               & args->piece_boards[player][KING][MIDDLE]));
  kr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[5]][1]
                               & args->piece_boards[player][KING][TOP]));
  kr += (temp * 2);
  temp = get_piece_num_sboard((ratings[rating_order[5]][2]
                               & args->piece_boards[player][KING][BOTTOM]));
  kr += (temp * 4);
  temp = get_piece_num_sboard((ratings[rating_order[5]][2]
                               & args->piece_boards[player][KING][MIDDLE]));
  kr += (temp * 4);
  temp = get_piece_num_sboard((ratings[rating_order[5]][2]
                               & args->piece_boards[player][KING][TOP]));
  kr += (temp * 4);
  if ((pr * pn_multiplier) < 0) {
    pr = 0;
  } else {
    pr *= pn_multiplier;
  }
  if ((nr * pn_multiplier) < 0) {
    nr = 0;
  } else {
    nr *= pn_multiplier;
  }
  br *= pos_multiplier;
  rr *= pos_multiplier;
  kr *= pos_multiplier;
  qr *= pos_multiplier;
  return (pr + br + nr + rr + kr + qr);
}
