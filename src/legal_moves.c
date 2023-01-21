#include <legal_moves.h>

void get_legal(SIDE player, unsigned int *pos, TYPE type,
               BOARD_ARGS args, uint64_t *output) {
  //CHECK DETECTION
  uint64_t check = 0;
  unsigned int k_pos = args.k_pos[type][0];
  PIECE_TYPE enemy_t = type == WHITE ? BLACK : WHITE;
  uint64_t bishop_king= BISHOP_PL[k_pos][0];
  uint64_t rook_king = ROOK_PL[k_pos][0];
  uint64_t bk_sa[3];
  uint64_t rk_sa[3];

  calc_sa(BISHOP, enemy_t, args.k_pos, bk_sa);
  calc_sa(ROOK, enemy_t, args.k_pos, bk_sa);

  unsigned int double_check = 0;
  unsigned int in_check = 0;
  uint64_t d_check[3] = { 0, 0, 0 };
  slider_check_detect(bk_sa, bishop_king, BISHOP_PL, args.k_pos[type],
                      args.d_sliders[enemy_t], d_check, &double_check);

  uint64_t hv_check[3] = { 0, 0, 0 };
  slider_check_detect(rk_sa, rook_king, ROOK_PL, args.k_pos[type],
                      args.hv_sliders[enemy_t], hv_check, &double_check);

  uint64_t j_check[3] = { 0, 0, 0 };
  uint64_t *p_attack = type == WHITE ? B_PAWN_ATTACK : W_PAWN_ATTACK;
  jump_check_detect(args.k_pos[type],
                    KNIGHT_PL | p_attack,
                    args.piece_boards[enemy_t][KNIGHT] |
                    args.piece_boards[enemy_t][PAWN], j_check,
                    &double_check);

  uint64_t total_check[3] = { 0, 0, 0 };
  total_check[TOP] = d_check[TOP] | hv_check[TOP] | j_check[TOP];
  total_check[MIDDLE] = d_check[MIDDLE] | hv_check[MIDDLE] | j_check[MIDDLE];
  total_check[BOTTOM] = d_check[BOTTOM] | hv_check[BOTTOM] | j_check[BOTTOM];
  in_check = total_check[TOP] | total_check[MIDDLE] | total_check[BOTTOM];

  // LEGAL MOVES
  if (type == KING) {
    // KING
    uint64_t under_attack[3] = { 0, 0, 0 };
    uint64_t sa[2][3] = 0;

    uint64_t pl = KING_PL[pos[0]];
    
    unsigned int s_pos;
    unsigned int coord[2];
    coord[1] = pos[1];
    while (pl) {
      s_pos = log(pl);
      pl ^= (1 << s_pos);

      coord[0] = s_pos;
      calc_sa(BISHOP, enemy_t, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, coord, sa[ROOK]);

      if (sa[BISHOP][pos[1]] & args.d_sliders[enemy_t][pos[1]] ||
          sa[ROOK][pos[1]] & args.hv_sliders[enemy_t][pos[1]]) {
        under_attack[coord[1]] |= (1 << coord[0]);
      }
    }
    coord[0] = pos[0];
    if (pos[0] == TOP) {
      coord[1] = MIDDLE;
      calc_sa(BISHOP, enemy_t, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, coord, sa[ROOK]);
      if (sa[BISHOP][pos[1]] & args.d_sliders[enemy_t][pos[1]] ||
          sa[ROOK][pos[1]] & args.hv_sliders[enemy_t][pos[1]]) {
        under_attack[MIDDLE] |= (1 << pos[0]);
      }
    } else if (pos[0] == MIDDLE) {
      coord[1] = TOP;
      calc_sa(BISHOP, enemy_t, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, coord, sa[ROOK]);
      if (sa[BISHOP][pos[1]] & args.d_sliders[enemy_t][pos[1]] ||
          sa[ROOK][pos[1]] & args.hv_sliders[enemy_t][pos[1]]) {
        under_attack[TOP] |= (1 << pos[0]);
      }
      coord[1] = BOTTOM;
      calc_sa(BISHOP, enemy_t, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, coord, sa[ROOK]);
      if (sa[BISHOP][pos[1]] & args.d_sliders[enemy_t][pos[1]] ||
          sa[ROOK][pos[1]] & args.hv_sliders[enemy_t][pos[1]]) {
        under_attack[BOTTOM] |= (1 << pos[0]);
      }
    } else {
      coord[1] = MIDDLE;
      calc_sa(BISHOP, enemy_t, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, coord, sa[ROOK]);
      if (sa[BISHOP][pos[1]] & args.d_sliders[enemy_t][pos[1]] ||
          sa[ROOK][pos[1]] & args.hv_sliders[enemy_t][pos[1]]) {
        under_attack[MIDDLE] |= (1 << pos[0]);
      }
    }

    output[top] = under_attack[TOP] ^ KING_PL[TOP];
    output[MIDDLE] = under_attack[MIDDLE] ^ KING_PL[MIDDLE];
    output[BOTTOM] = under_attack[BOTTOM] ^ KING_PL[BOTTOM];
    return;
  } else if (double_check) {
    output[TOP] = 0;
    output[MIDDLE] = 0;
    output[BOTTOM] 0;
    return;
  } else if (type == PAWN || type == KNIGHT) {
    // SIMPLE
    uint64_t *pl;
    if (type == PAWN) {
      pl = player == WHITE ? W_PAWN_PL : B_PAWN_PL;
    } else {
      pl = KNIGHT_PL;
    }
    output[pos[1]] = pl & args.boards[player][pos[1]];
    if (pos[1] == TOP) {
      output[MIDDLE] = LEVELS[pos[0]] & args.boards[player][MIDDLE];
      output[BOTTOM] = 0;
    } else if (pos[1] == MIDDLE) {
      output[TOP] = LEVELS[pos[0]] & args.boards[player][TOP];
      output[BOTTOM] = LEVELS[pos[0]] & args.boards[player][BOTTOM];
    } else {
      output[TOP] = 0;
      output[MIDDLE] = LEVELS[pos[0]] & args.boards[player][MIDDLE];
    }
    return;
  } else {
    // COMPLEX LEGAL CALCULATION

    // SA BOARDS FOR ROOK, BISHOP, QUEEN, RESPECTIVELY
    uint64_t sa[3][3];

    // CALCULATE SPACIALLY AWARE BOARDS
    if (type == ROOK || type == QUEEN) {
      calc_sa(ROOK, enemy_t, pos, sa[ROOK]);
    }
    if (type == BISHOP || type == QUEEN) {
      calc_sa(BISHOP, enemy_t, pos, sa[BISHOP]);
    }
    if (type == QUEEN) {
      sa[QUEEN] = sa[ROOK] | sa[BISHOP];
    }

    // DISCOVERY CHECK RESTRICTIONS
    if (k_pos[type][0] == pos[0]) {
      uint64_t cur = LEVELS[pos[0]];
      if (bishop_king & cur == 0 && rook_king & cur == 0) {
        // NOT IN DISC CHECK
        if (in_check) {
          output[TOP] = total_check[TOP] & sa[type][TOP]; 
          output[MIDDLE] = total_check[MIDDLE] & sa[type][MIDDLE]; 
          output[BOTTOM] = total_check[BOTTOM] & sa[type][BOTTOM]; 
        } else {
          output[TOP] = sa[type][TOP];
          output[MIDDLE] = sa[type][MIDDLE];
          output[BOTTOM] = sa[type][BOTTOM];
        }
      }

      uint64_t path = 0;
      uint64_t between = 0;
      uint64_t past = 0;
      uint64_t k_to_cur = 0;
      uint64_t cur_to_k = 0;
      uint64_t enemies = 0;
      if ((1 << k_pos) > cur) {
        k_to_cur = UPPER_ZERO >> (63 - k_pos);
        cur_to_k = LOWER_ZERO << log(cur);
      } else {
        k_to_cur = LOWER_ZERO << k_pos;
        cur_to_k = UPPER_ZERO >> (63 - log(cur));
      }

      if (bishop_king & cur != 0) {
        // PIECE IS ALONG DIAGONAL FROM KING
        path = bishop_king & BISHOP_PL[pos[0]];
        enemies = d_sliders[enemy_t][pos[0]];
      } else {
        // PIECE IS ALONG RANK/FILE FROM KING
        path = rook_king & ROOK_PL[pos[0]];
        enemies = hv_sliders[enemy_t][pos[0]];
      }
      between = path & k_to_cur & cur_to_k;

      if (between & (boards[WHITE][pos[0]] | boards[BLACK][pos[0]])) {
        // NOT IN DISC CHECK
        if (in_check) {
          output[TOP] = total_check[TOP] & sa[type][TOP]; 
          output[MIDDLE] = total_check[MIDDLE] & sa[type][MIDDLE]; 
          output[BOTTOM] = total_check[BOTTOM] & sa[type][BOTTOM]; 
        } else {
          output[TOP] = sa[type][TOP];
          output[MIDDLE] = sa[type][MIDDLE];
          output[BOTTOM] = sa[type][BOTTOM];
        }
      }

      uint64_t from_k = k_to_cur & path;
      uint64_t behind_enemies = from_k & ~enemies;
      unsigned int closest_enemy = 0;
      if (behind_enemies) {
        if ((1 << k_pos) > cur) {
          // CUR IN FRONT OF ENEMY
          closest_enemy = log(behind_enemies);
          from_k &= LOWER_ZERO << closest_enemy;
        } else {
          // CUR BEHIND ENEMY
          closest_enemy = log(behind_enemies & -behind_enemies);
          from_k &= UPPER_ZERO >> (63 - closest_enemy);
        }

        if (from_k & ~(boards[type][pos[0]] ^ ~LEVELS[pos[0]]) == 0) {
          // DIST CHECK FOUND
          if (in_check) {
            output[TOP] = total_check[TOP] & sa[type][TOP] &
                          (from_k | between); 
            output[MIDDLE] = total_check[MIDDLE] & sa[type][MIDDLE] &
                             (from_k | between); 
            output[BOTTOM] = total_check[BOTTOM] & sa[type][BOTTOM] &
                             (from_k | between); 
          } else {
            output[TOP] = sa[type][TOP];
            output[MIDDLE] = sa[type][MIDDLE];
            output[BOTTOM] = sa[type][BOTTOM];
          }          
        } else {
          // NOT IN DISC CHECK
          if (in_check) {
            output[TOP] = total_check[TOP] & sa[type][TOP]; 
            output[MIDDLE] = total_check[MIDDLE] & sa[type][MIDDLE]; 
            output[BOTTOM] = total_check[BOTTOM] & sa[type][BOTTOM]; 
          } else {
            output[TOP] = sa[type][TOP];
            output[MIDDLE] = sa[type][MIDDLE];
            output[BOTTOM] = sa[type][BOTTOM];
          }
        }
      } else {
        // NOT IN DISC CHECK
        if (in_check) {
          output[TOP] = total_check[TOP] & sa[type][TOP]; 
          output[MIDDLE] = total_check[MIDDLE] & sa[type][MIDDLE]; 
          output[BOTTOM] = total_check[BOTTOM] & sa[type][BOTTOM]; 
        } else {
          output[TOP] = sa[type][TOP];
          output[MIDDLE] = sa[type][MIDDLE];
          output[BOTTOM] = sa[type][BOTTOM];
        }
      }
    } else {
      // NOT IN DISC CHECK
      if (in_check) {
        output[TOP] = total_check[TOP] & sa[type][TOP]; 
        output[MIDDLE] = total_check[MIDDLE] & sa[type][MIDDLE]; 
        output[BOTTOM] = total_check[BOTTOM] & sa[type][BOTTOM]; 
      } else {
        output[TOP] = sa[type][TOP];
        output[MIDDLE] = sa[type][MIDDLE];
        output[BOTTOM] = sa[type][BOTTOM];
      }
    }
  }
}

void calc_spacially_aware(TYPE type, SIDE enemy_t, unsigned int *pos,
                          uint64_t *sa) {
  uint64_t *pl = NULL;
  if (type == BISHOP) {
    boards = BISHOP_PL[pos[0]];
  } else if (type == ROOK) {
    boards = ROOL_PL[pos[0]];
  }

  // GRAB PSEUDO LEGAL MOVES FOR CURRENT LEVEL
  sa[pos[1]] = pl[0];

  // CANNOT MOVE DOWN OR UP WHERE FRIENDS ARE
  if (pos[1] == TOP) {
    sa[MIDDLE] = LEVELS[pos[0]] & args.boards[type][MIDDLE];
    sa[BOTTOM] = 0;
  } else if (pos[1] == MIDDLE) {
    sa[TOP] = LEVELS[pos[0]] & args.boards[type][TOP];
    sa[BOTTOM] = LEVELS[pos[0]] & args.boards[type][BOTTOM];
  } else {
    sa[TOP] = 0;
    sa[MIDDLE] = LEVELS[pos[0]] & args.boards[type][MIDDLE];
  }

  // ELIMINATE POSITIONS OF FRIENDS AND ENEMIES
  sa[pos[1]] &= (args.boards[enemy_t][pos[1]] & args.boards[type][pos[1]]);

  // "SHADOW" FRIENDS AND ENEMIES
  uint64_t up = sa[pos[1]] & pl[1]; 
  up &= UPPER_ZERO >> (63 - log(up & -up));

  uint64_t right = sa[pos[1]] & pl[2]; 
  right &= UPPER_ZERO >> (63 - log(right & -right));

  uint64_t down = sa[pos[1]] & pl[3]; 
  down &= UPPER_ZERO >> (63 - log(down & -down));

  uint64_t left = sa[pos[1]] & pl[4]; 
  left &= LOWER_ZERO << log(left);

  // COMBINE SHADOWS
  sa[pos[1]] = up | right | down | left;  
  // RENABLE ATTACKING
  sa[pos[1]] |= (pl[0] & args.boards[enemy_t][pos1]);
}

void slider_check_detect(uint64_t *sk, uint64_t sk_xray, uint64_t *pl,
                         unsigned int *pos, uint64_t *sliders,
                         uint64_t *output, int *double_check) {
  unsigned int num_check = 0;
  output[pos[1]] = sk[pos[1]] & ~sliders[pos[1]];
  if (pos[1] == TOP) {
    output[MIDDLE] = LEVELS[pos[1]] & sliders[MIDDLE];
    output[BOTTOM] = 0;
  } else if (pos[1] == MIDDLE) {
    output[TOP] = LEVELS[pos[1]] & sliders[TOP];
    output[BOTTOM] = LEVELS[pos[1]] & sliders[BOTTOM];
  } else {
    output[TOP] = 0;
    output[MIDDLE] = LEVELS[pos[1]] & sliders[BOTTOM];
  }
  if (output[TOP]) {
    num_check++;
  }
  if (output[MIDDLE]) {
    num_check++;
  }
  if (output[BOTTOM]) {
    num_check++;
  }

  if (num_check > 1 || (output[pos[1]] & -output[pos[1]])) {
    *double_check = 1;
  } else if (output[pos[1]]) {
    // Check is vector between slider and king
    unsigned int e_pos = log(output[pos[1]]);

    uint64_t between_mask = 0;
    if (pos[0] > e_pos) {
      between_mask = (UPPER_ZERO >> (63 - pos[0])) & (LOWER_ZERO << e_pos);
    } else {
      between_mask = (UPPER_ZERO >> (63 - e_pos)) & (LOWER_ZERO << pos[0]);
    }
    output[pos[1]] = sk_xray & (pl[e_pos] | LEVELS[e_pos]) & between_mask;
  }
}

void jump_check_detect(uint64_t *pos, uint64_t *pl, uint64_t *piece_board,
                       uint64_t *output, unsigned int *double_check) {
  unsigned int num_check = 0;
  output[pos[1]] = pl[pos[0]] & piece_board[pos[1]];
  if (pos[1] == TOP) {
    output[MIDDLE] = LEVELS[pos[1]] & piece_board[MIDDLE];
    output[BOTTOM] = 0;
  } else if (pos[1] == MIDDLE) {
    output[TOP] = LEVELS[pos[1]] & piece_board[TOP];
    output[BOTTOM] = LEVELS[pos[1]] & piece_board[BOTTOM];
  } else {
    output[TOP] = 0;
    output[MIDDLE] = LEVELS[pos[1]] & piece_board[MIDDLE];
  }

  if (output[TOP]) {
    num_check++;
  }
  if (output[MIDDLE]) {
    num_check++;
  }
  if (output[BOTTOM]) {
    num_check++;
  }

  if (num_check > 1 || (output[pos[1]] & -output[pos[1]])) {
    *double_check = 1;
    return 0;
  }
}
