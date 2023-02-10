#include <search.h>


/*
  ORDER OF EVENTS:
  -> Determine which side it is to move
  -> get legal moves for position
  -> Iterate over all pieces, starting with the king, and
     move to all of their legal moves, checking for mate,
     double check and check.
     -> To iterate over legals (each piece)
        -> iterate over levels
          -> iterate over each space of each level
  -> Once depth or checkmate has been hit, return the eval
     up the tree to be used in choosing the best move
*/
#define PIECE_TYPES 6
#define LEVELS 3
#define ONE 1

int ms_bit_lookup(uint64_t *input) {
  int ms_bit_poss = log2_lookup(*input);
  *input &= ~(ONE << ms_bit_poss);
  return ms_bit_lookup;
}

void make_temp_copy(BOARD_ARGS *source, BOARD_ARGS *dest) {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
    dest->boards[i][j] = source->boards[i][j];
    }
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 6; j++) {
      for (int k = 0; k < 3; k++) {
        dest->piece_boards[i][j][k] = source->piece_boards[i][j][k];
      }
    }
  }
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
    dest->hv_sliders[i][j] = source->hv_sliders[i][j];
    }
  }
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
    dest->d_sliders[i][j] = source->d_sliders[i][j];
    }
  }
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
    dest->k_pos[i][j] = source->k_pos[i][j];
    }
  }
}

MOVE search (BOARD_ARGS *args, SIDE to_move, unsigned int depth) {
  SIDE enemy = to_move == WHITE ? BLACK : WHITE;
  /*
    PLAYER/ENEMY FLAGS
    MATE : 0x00000001
           0b0001
    D_CHECK: 0x00000002
             0b0010
    CHECK: 0x00000004
           0b0100
  */
  uint32_t player_flags = 0;
  uint32_t enemy_flags = 0;
  for (int i = 0; i < PIECE_TYPES; i++) {
    for (int j = 0; j < LEVELS; j++) {
      /*
        FIND MOST SIG BIT OF PIECES BOARD, THEN REMOVE FROM BITBOARD
      */
      uint64_t t_piece_board = args.piece_boards[to_move][i][j];
      int ms_bit[2] = {ms_bit_lookup(&t_piece_board), j};
      uint64_t legals[3];
      get_legal(enemy, args->k_pos[enemy], KING, args, legals, &enemy_flags);
      if (depth == 0 || (player_flags & MATE == 1 || enemy_flags & MATE == 1)) {
        return evaluate(args, to_move, player_flags, enemy_flags);
      }
      get_legal(to_move, ms_bit, i, args, legals, &player_flags);
      for (int k = 0; k < LEVELS; k++) {
        /*
          GET SIG BIT FOR LEGAL MOVES BIT BOARD, THEN MAKE ALL MOVES
        */
        uint64_t t_legal_board = legals[k];
        int ms_bit_poss_legal[2] = {ms_bit_lookup(&t_legal_board), k};
        BOARD_ARGS copy;
        make_temp_copy(args, &copy);
        MOVE new_move = make_move(args, player, i, ms_bit, ms_bit_poss_legal);
        search(args, enemy, depth - 1);
        make_temp_copy(copy, args);
      }
    }
  }
  
}

BOARD_ARGS make_move(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
                     unsigned int *from, unsigned int *to) {
  SIDE enemy_t = to_move == WHITE ? BLACK : WHITE;
  uint64_t old_pos = LEVELS[from[0]];
  uint64_t new_pos = LEVELS[to[0]];

  args->boards[to_move][from[1]] ^= old_pos;
  args->piece_boards[to_move][p_type][from[1]] ^= old_pos;
  args->boards[to_move][to[1]] |= new_pos;
  args->piece_boards[to_move][p_type][to[1]] |= new_pos;

  if (p_type == ROOK || p_type == QUEEN) {
    args->hv_sliders[to_move][from[1]] ^= old_pos;
    args->hv_sliders[to_move][to[1]] |= new_pos;
  }
  if (p_type == BISHOP || p_type == QUEEN) {
    args->d_sliders[to_move][from[1]] ^= old_pos;
    args->d_sliders[to_move][to[1]] |= new_pos;
  }
  if (p_type == KING) {
    args->k_pos[to_move][0] = to[0];
    args->k_pos[to_move][1] = to[1];
  }

  if (new_pos & args->boards[enemy_t][to[1]]) {
    args->boards[enemy_t][to[1]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][BISHOP][to[1]]) {
    args->piece_boards[enemy_t][BISHOP][to[1]] ^= new_pos;
    args->d_sliders[enemy_t][to[1]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][ROOK][to[1]]) {
    args->piece_boards[enemy_t][ROOK][to[1]] ^= new_pos;
    args->hv_sliders[enemy_t][to[1]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][QUEEN][to[1]]) {
    args->piece_boards[enemy_t][QUEEN][to[1]] ^= new_pos;
    args->hv_sliders[enemy_t][to[1]] ^= new_pos;
    args->d_sliders[enemy_t][to[1]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][PAWN][to[1]]) {
    args->piece_boards[enemy_t][PAWN][to[1]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][KNIGHT][to[1]]) {
    args->piece_boards[enemy_t][KNIGHT][to[1]] ^= new_pos;
  }

  return *args;
}
