#include <search.h>

MOVE search(BOARD_ARGS *args, SIDE to_move, unsigned int depth) {
  uint32_t p_flags = 0;
  uint32_t e_flags = 0;
  uint64_t legal_moves[3];
  SIDE enemy = to_move == WHITE ? BLACK : WHITE;
  get_legal(to_move, args->k_pos[to_move], KING, *args, legal_moves, &p_flags);
  get_legal(enemy, args->k_pos[enemy], KING, *args, legal_moves, &e_flags);

  if (depth == 0 || (p_flags & MATE) || (e_flags & MATE)) {
    return evaluate(args, to_move, p_flags, e_flags);
  }

  MOVE best;
  best.args = args;
  best.to_move = to_move;
  if (to_move == WHITE) {
    best.score = INT_MIN;
  } else {
    best.score = INT_MAX;
  }
  MOVE cur;

  uint64_t pieces[3];
  uint64_t temp = 0;
  uint64_t cur_move_board = 0;
  unsigned int cur_pos[2];
  unsigned int to_pos[2];
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

        get_legal(to_move, cur_pos, i, *args, legal_moves, &p_flags);



        for (LEVEL k = TOP; k <= BOTTOM; k++) {
          to_pos[1] = k;
          cur_move_board = legal_moves[i];
          while (cur_move_board) {
            to_pos[0] = log2_lookup(cur_move_board);
            cur_move_board ^= (ONE << to_pos[0]);
            BOARD_ARGS n_args = make_move(args, to_move, i, cur_pos, to_pos);
            cur = search(&n_args, enemy, depth - 1);

            if ((to_move == WHITE && cur.score > best.score) ||
                (to_move == BLACK && cur.score < best.score)) {
              best.score = cur.score;
              best.from[0] = cur_pos[0];
              best.from[1] = cur_pos[1];
              best.to[0] = cur.to[0];
              best.to[1] = cur.to[1];
            }
          }
        }
      


      }
    }
  }

  return cur;
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
