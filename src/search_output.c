typedef node {
  int position[192];
  unsigned int level_from;
  unsigned int bitposition_from;
  unsigned int level_to;
  unsigned int bitposition_from;
  unsigned int result;
} node_t;

node_t chosen_moves[10000];
int chosen_moves = 0;
node_t possible_moves[5000];
int possible_moves_i = 0;
int piece_positions[192];

void search_output(BOARD_ARGS, *args, SIDE to_move) {
  SIDE enemy = to_move == WHITE ? BLACK : WHITE;
  uint32_t player_flags = 0;
  uint32_t enemy_flags = 0;
  MOVE move;
  move.args = args;
  move.to_move = to_move;
  move.to[0] = 0;
  move.to[1] = 0;
  move.from[0] = 0;
  move.from[1];
  move.rating = 0;
  unsigned int current_position[2];
  unsigned int to_position[2];
  uint64_t junk[3];
  get_legal(enemy, args->k_pos[enemy], KING, *args, junk, &enemy_flags);
  get_legal(to_move, args->k_pos[to_move], KING, *args, junk, &player_flags);
  if ((enemy_flags & MATE) == 1) {
    possible_moves_i = 0;
    possible_moves[possible_moves_i].result = 1;
    possible_moves[possible_moves_i].level_from = 0;
    possible_moves[possible_moves_i].level_to = 0;
    possible_moves[possible_moves_i].bitposition_from = 0;
    possible_moves[possible_moves_i].bitposition_to = 0;
    return;
  } else if ((player_flags & MATE) == 1) {
    possible_moves_i = 0;
    possible_moves[possible_moves_i].result = 0;
    possible_moves[possible_moves_i].level_from = 0;
    possible_moves[possible_moves_i].level_to = 0;
    possible_moves[possible_moves_i].bitposition_from = 0;
    possible_moves[possible_moves_i].bitposition_to = 0;
    return;
  }
  for (int i = 0; i < PIECE_TYPES; i++) {
    // PIECE TYPE
    for (int j = 0; j < NUM_LEVELS; j++) {
      // LEVEL
      current_position[0] = j;
      uint64_t t_piece_board = args->piece_boards[to_move][i][j];
      while (t_piece_board) {
        unsigned int ms_bit[2] = {j, ms_bit_lookup(&t_piece_board)};
        current_position[1] = ms_bit[1];
        move.from[0] = current_position[0];
        move.from[1] = current_position[1];
        uint64_t legals[3];
        get_legal(to_move, ms_bit, i, *args, legals, &player_flags);
        for (int k = 0; k < NUM_LEVELS; k++) {
          to_position[0] = k;
          uint64_t t_legal_board = legals[k];
          while (t_legal_board) {
            unsigned int ms_bit_poss_legal[2] = {k, ms_bit_lookup(&t_legal_board)};
            to_position[1] = ms_bit_poss_legal[1];
            move.to[0] = to_position[0];
            move.to[1] = to_position[1];
            possible_moves[possible_moves_i].result = -1;
            possible_moves[possible_moves_i].level_from = move.from[0];
            possible_moves[possible_moves_i].level_to = move.to[0];
            possible_moves[possible_moves_i].bitposition_from = move.from[1];
            possible_moves[possible_moves_i].bitposition_to = move.from[1];
            translate_position(move.args, possible_moves_i++);
          }
        }
      }
    }
  }
}

/*
  1: BISHOP
  2: ROOK
  3: QUEEN
  4: PAWN
  5: KNIGHT
  6: KING
*/

void translate_position(BOARD_ARGS *args, int arr_pos) {
  /* MAKE SURE TO FREE */
  int *p = (int *) calloc(192, sizeof(int));
  uint64_t x = 0;
  int m = -1;
  int val = 0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 6; j++) {
      for (int k = 0; k < 3; k++) {
        x = args->piece_boards[i][j][k];
        m = log2_lookup(x);
        while (x != 0) {
          if (j == PAWN) {
            val = 4;
          } else if (j == BISHOP) {
            val = 1;
          } else if (j == ROOK) {
            val = 2;
          } else if (j == QUEEN) {
            val = 2;
          } else if (j == KING) {
            val = 6;
          }
          p[k][m / 8][ m % 8] = val;
          x ^= (ONE << m);
          m = log2_lookup(x);
        }
      }
    }
  }
  possible_moves[arr_pos] = p;
}
