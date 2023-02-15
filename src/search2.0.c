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
#define NUM_LEVELS 3

int ms_bit_lookup(uint64_t *input) {
  int ms_bit_poss = log2_lookup(*input);
  *input &= ~(ONE << ms_bit_poss);
  return ms_bit_poss;
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

//long stuff = 0;

MOVE search (BOARD_ARGS *args, SIDE to_move, unsigned int depth) {
 //stuff++;
 //printf("%ld\n", stuff);
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
  MOVE best;
  MOVE current;
  if (to_move == WHITE) {
    best.score = INT_MIN;
  } else {
    best.score = INT_MAX;
  }
  best.args = args;
  best.to_move = to_move;
  unsigned int current_position[2];
  unsigned int to_position[2];
  uint64_t junk[3];
  /*
    BREAKS WHEN KING BEING EVALUATED ON BLACK AFTER MOVING TO TOP LEVEL
  */
  get_legal(enemy, args->k_pos[enemy], KING, *args, junk, &enemy_flags);
  for (int i = 0; i < PIECE_TYPES; i++) {
    /*
      SELECT PIECE TYPE
    */
    for (int j = 0; j < NUM_LEVELS; j++) {
      /*
        SELECT LEVEL
      */
      current_position[0] = j;
      /*
        FIND MOST SIG BIT OF PIECES BOARD, THEN REMOVE FROM BITBOARD
      */
      uint64_t t_piece_board = args->piece_boards[to_move][i][j];
      while (t_piece_board) {
        unsigned int ms_bit[2] = {j, ms_bit_lookup(&t_piece_board)};
        current_position[1] = ms_bit[1];
        current.from[0] = current_position[0];
        current.from[1] = current_position[1];
        uint64_t legals[3];
        get_legal(to_move, ms_bit, i, *args, legals, &player_flags);
        if (depth == 0) {
          return evaluate(args, to_move, player_flags, enemy_flags);
        } else if ((player_flags & MATE) == 1 || (enemy_flags & MATE) == 1) {
          current = evaluate(args, to_move, player_flags, enemy_flags);
          if ((to_move == WHITE && current.score > best.score) ||
              (to_move == BLACK && current.score < best.score)) {
            best.score = current.score;
            best.from[0] = current_position[0];
            best.from[1] = current_position[1];
            best.to[0] = to_position[0];
            best.to[1] = to_position[1];
          }
        } else {
          for (int k = 0; k < NUM_LEVELS; k++) {
            /*
              SELECT LEGAL MOVE
            */
            to_position[0] = k;
            /*
              GET SIG BIT FOR LEGAL MOVES BIT BOARD, THEN MAKE ALL MOVES
            */
            uint64_t t_legal_board = legals[k];
            while (t_legal_board) {
              unsigned int ms_bit_poss_legal[2] = {k, ms_bit_lookup(&t_legal_board)};
              to_position[1] = ms_bit_poss_legal[1];
              current.to[0] = to_position[0];
              current.to[1] = to_position[1];
              BOARD_ARGS copy;
              make_temp_copy(args, &copy);
              make_move(&copy, to_move, i, ms_bit, ms_bit_poss_legal);
              current = search(&copy, enemy, depth - 1);
              if ((to_move == WHITE && current.score > best.score) ||
                (to_move == BLACK && current.score < best.score)) {
                best.score = current.score;
                best.from[0] = current_position[0];
                best.from[1] = current_position[1];
                best.to[0] = to_position[0];
                best.to[1] = to_position[1];
              }
            }
          }
        }
      }
    }
  }
  return best;
}

void make_move(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
                     unsigned int *from, unsigned int *to) {
  /*
    To/From : {Level, Bitposition}
  */

  SIDE enemy_t = to_move == WHITE ? BLACK : WHITE;
  uint64_t old_pos = LEVELS[from[1]];
  uint64_t new_pos = LEVELS[to[1]];

  args->boards[to_move][from[0]] ^= old_pos;
  args->piece_boards[to_move][p_type][from[0]] ^= old_pos;
  args->boards[to_move][to[0]] |= new_pos;
  args->piece_boards[to_move][p_type][to[0]] |= new_pos;

  if (p_type == ROOK || p_type == QUEEN) {
    args->hv_sliders[to_move][from[0]] ^= old_pos;
    args->hv_sliders[to_move][to[0]] |= new_pos;
  }
  if (p_type == BISHOP || p_type == QUEEN) {
    args->d_sliders[to_move][from[0]] ^= old_pos;
    args->d_sliders[to_move][to[0]] |= new_pos;
  }
  if (p_type == KING) {
    args->k_pos[to_move][0] = to[0];
    args->k_pos[to_move][1] = to[1];
  }

  if (new_pos & args->boards[enemy_t][to[0]]) {
    args->boards[enemy_t][to[0]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][BISHOP][to[0]]) {
    args->piece_boards[enemy_t][BISHOP][to[0]] ^= new_pos;
    args->d_sliders[enemy_t][to[0]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][ROOK][to[0]]) {
    args->piece_boards[enemy_t][ROOK][to[0]] ^= new_pos;
    args->hv_sliders[enemy_t][to[0]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][QUEEN][to[0]]) {
    args->piece_boards[enemy_t][QUEEN][to[0]] ^= new_pos;
    args->hv_sliders[enemy_t][to[0]] ^= new_pos;
    args->d_sliders[enemy_t][to[0]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][PAWN][to[0]]) {
    args->piece_boards[enemy_t][PAWN][to[0]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][KNIGHT][to[0]]) {
    args->piece_boards[enemy_t][KNIGHT][to[0]] ^= new_pos;
  }
}