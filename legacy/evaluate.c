#include <evaluate.h>

MOVE evaluate(BOARD_ARGS *args, SIDE to_move, uint32_t p_flags, uint32_t e_flags) {
  MOVE move;
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
