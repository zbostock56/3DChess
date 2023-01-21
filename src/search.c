#include <search.h>

// TODO MAKE SEARCH WORK

void search(BOARD_ARGS *args, SIDE to_move, unsigned int depth) {
  uint64_t (*pieces)[3] = args->piece_boards[to_move];
  
  uint64_t legal_moves[3];
  uint64_t pieces[3];
  uint64_t temp = 0;
  unsigned int cur_pos[2];
  for (TYPE i = KING; i >= BISHOP; i--) {
    pieces[TOP] = args->piece_boards[to_move][i][TOP];
    pieces[MIDDLE] = args->piece_boards[to_move][i][MIDDLE];
    pieces[BOTTOM] = args->piece_boards[to_move][i][BOTTOM];

    for (LEVEL j = TOP; j <= BOTTOM; j++) {
      cur_pos[1] = j;
      temp = pieces[j];
      while (temp) {
        cur_pos[0] = log2_lookup(temp);
        temp ^= (ONE << cur_pos[0]);

        get_legal(to_move, *flags, cur_pos, i, args, legal_moves);
        make_moves(args, to_move, p_type, cur_pos, legal_moves, depth);
      }
    }
  }
}

void make_moves(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
                unsigned int *cur_pos, uint64_t *legal_moves,
                unsigned int depth) {
  SIDE enemy_t = to_move == WHITE ? BLACK : WHITE;
  uint64_t to_pos[2];
  uint64_t temp = 0;
  for (LEVEL i = TOP; i <= BOTTOM; i++) {
    to_pos[1] = i;
    temp = legal_moves[i];
    while (temp) {
      to_pos[0] = log2_lookup(temp);
      temp ^= (ONE << to_pos[0]);
      search(make_move(args, to_move, p_type, cur_pos, to_pos),
             enemy_t, depth - 1);
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
