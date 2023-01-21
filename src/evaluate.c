#include <evalutate.h>

int evaluate(BOARD_ARGS *args, uint32_t flags) {
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

  int material_score = (BISHOP_VAL * (b_num[0] - b_num[1])) +
                       (ROOK_VAL * (r_num[0] - r_num[1])) +
                       (QUEEN_VAL * (q_num[0] - q_num[1])) +
                       (PAWN_VAL * (p_num[0] - p_num[1])) +
                       (KNIGHT_VAL * (n_num[0] - n_num[1]));

  return material_score;
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
