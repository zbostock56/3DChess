/*
TODO: Fix Rook Legals (Messing up middle board, could be more)
      -> Effects Queen
      King Legals (Messed up middle board, could be more)

*/

#include <legal_moves.h>

void get_legal(SIDE player, unsigned int *pos, TYPE type,
               BOARD_ARGS args, uint64_t *output, uint32_t *flags) {
  //CHECK DETECTION
  unsigned int k_pos = args.k_pos[player][0];
  SIDE enemy_t = player == WHITE ? BLACK : WHITE;
  uint64_t bishop_king= (BISHOP_PL + (5 * k_pos))[0];
  uint64_t rook_king = (ROOK_PL + (5 * k_pos))[0];
  //printf_bitboard(bishop_king);
  //printf_bitboard(rook_king);
  uint64_t bk_sa[3];
  uint64_t rk_sa[3];

  calc_sa(BISHOP, enemy_t, args.boards, args.k_pos[player], bk_sa);
  calc_sa(ROOK, enemy_t, args.boards, args.k_pos[player], rk_sa);

  /*printf("BK SA\n");
  printf_bitboards(bk_sa);
  printf("RK SA\n");
  printf_bitboards(rk_sa);*/

  unsigned int double_check = 0;
  unsigned int in_check = 0;
  uint64_t d_check[3] = { 0, 0, 0 };
  slider_check_detect(bk_sa, bishop_king, BISHOP_PL, args.k_pos[player],
                      args.d_sliders[enemy_t], d_check, &double_check);

  uint64_t hv_check[3] = { 0, 0, 0 };
  slider_check_detect(rk_sa, rook_king, ROOK_PL, args.k_pos[player],
                      args.hv_sliders[enemy_t], hv_check, &double_check);

  uint64_t j_check[3] = { 0, 0, 0 };
  uint64_t p_attack = player == WHITE ? W_PAWN_ATTACK[args.k_pos[player][0]] :
                                       B_PAWN_ATTACK[args.k_pos[player][0]];
  uint64_t kp_enemies[3];
  kp_enemies[TOP] = args.piece_boards[enemy_t][KNIGHT][TOP] |
                    args.piece_boards[enemy_t][PAWN][TOP];
  kp_enemies[MIDDLE] = args.piece_boards[enemy_t][KNIGHT][MIDDLE] |
                       args.piece_boards[enemy_t][PAWN][MIDDLE];
  kp_enemies[BOTTOM] = args.piece_boards[enemy_t][KNIGHT][BOTTOM] |
                       args.piece_boards[enemy_t][PAWN][BOTTOM];
  jump_check_detect(args.k_pos[player],
                    KNIGHT_PL[args.k_pos[player][0]] | p_attack, kp_enemies,
                    j_check, &double_check);

  uint64_t total_check[3] = { 0, 0, 0 };
  total_check[TOP] = d_check[TOP] | hv_check[TOP] | j_check[TOP];
  total_check[MIDDLE] = d_check[MIDDLE] | hv_check[MIDDLE] | j_check[MIDDLE];
  total_check[BOTTOM] = d_check[BOTTOM] | hv_check[BOTTOM] | j_check[BOTTOM];
  in_check = total_check[TOP] | total_check[MIDDLE] | total_check[BOTTOM];


  // SET CHECK FLAGS
  if (in_check) {
    *flags |= CHECK;
  }

  // LEGAL MOVES
  if (type == KING) {
    // KING
    uint64_t under_attack[3] = { 0, 0, 0 };
    uint64_t sa[2][3];
    uint64_t pl = KING_PL[pos[0]];
    unsigned int s_pos;
    unsigned int coord[2];
    coord[1] = pos[1];
    while (pl) {
      s_pos = log2_lookup(pl);
      pl ^= (ONE << s_pos);

      coord[0] = s_pos;
      calc_sa(BISHOP, enemy_t, args.boards, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, args.boards, coord, sa[ROOK]);

      if (sa[BISHOP][pos[1]] & args.d_sliders[enemy_t][pos[1]] ||
          sa[ROOK][pos[1]] & args.hv_sliders[enemy_t][pos[1]]) {
        under_attack[coord[1]] |= (ONE << coord[0]);
      }
    }

    coord[0] = pos[0];
    output[pos[1]] = under_attack[pos[1]] ^ KING_PL[pos[0]];
    if (pos[1] == TOP) {
      coord[1] = MIDDLE;
      calc_sa(BISHOP, enemy_t, args.boards, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, args.boards, coord, sa[ROOK]);
      if (sa[BISHOP][pos[1]] & args.d_sliders[enemy_t][pos[1]] ||
          sa[ROOK][pos[1]] & args.hv_sliders[enemy_t][pos[1]]) {
        under_attack[MIDDLE] |= (ONE << pos[0]);
      }
      output[MIDDLE] = (ONE << pos[0]) ^ under_attack[MIDDLE];
      output[BOTTOM] = 0;
    } else if (pos[1] == MIDDLE) {
      coord[1] = TOP;
      calc_sa(BISHOP, enemy_t, args.boards, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, args.boards, coord, sa[ROOK]);
      if (sa[BISHOP][pos[1]] & args.d_sliders[enemy_t][pos[1]] ||
          sa[ROOK][pos[1]] & args.hv_sliders[enemy_t][pos[1]]) {
        under_attack[TOP] |= (ONE << pos[0]);
      }
      output[TOP] = (ONE << pos[0]) ^ under_attack[TOP];

      coord[1] = BOTTOM;
      calc_sa(BISHOP, enemy_t, args.boards, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, args.boards, coord, sa[ROOK]);
      if (sa[BISHOP][pos[1]] & args.d_sliders[enemy_t][pos[1]] ||
          sa[ROOK][pos[1]] & args.hv_sliders[enemy_t][pos[1]]) {
        under_attack[BOTTOM] |= (ONE << pos[0]);
      }
      output[BOTTOM] = (ONE << pos[0]) ^ under_attack[BOTTOM];
    } else {
      coord[1] = MIDDLE;
      calc_sa(BISHOP, enemy_t, args.boards, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, args.boards, coord, sa[ROOK]);
      if (sa[BISHOP][pos[1]] & args.d_sliders[enemy_t][pos[1]] ||
          sa[ROOK][pos[1]] & args.hv_sliders[enemy_t][pos[1]]) {
        under_attack[MIDDLE] |= (ONE << pos[0]);
      }
      output[MIDDLE] = (ONE << pos[0]) ^ under_attack[MIDDLE];
      output[BOTTOM] = 0;
    }
    output[TOP] &= ~args.boards[player][TOP];
    output[MIDDLE] &= ~args.boards[player][MIDDLE];
    output[BOTTOM] &= ~args.boards[player][BOTTOM];
    if ((output[TOP] | output[MIDDLE] | output[BOTTOM]) == 0 && in_check) {
      *flags |= MATE;
    }
    return;
  } else if (double_check) {
    output[TOP] = 0;
    output[MIDDLE] = 0;
    output[BOTTOM] = 0;
    *flags |= D_CHECK;
    return;
  } else if (type == PAWN || type == KNIGHT) {
    // SIMPLE
    uint64_t pl;
    if (type == PAWN) {
      pl = player == WHITE ? W_PAWN_PL[pos[0]] : B_PAWN_PL[pos[0]];
    } else {
      pl = KNIGHT_PL[pos[0]];
    }
    output[pos[1]] = pl & ~args.boards[player][pos[1]];
    if (pos[1] == TOP) {
      output[MIDDLE] = LEVELS[pos[0]] & ~args.boards[player][MIDDLE];
      output[BOTTOM] = 0;
    } else if (pos[1] == MIDDLE) {
      output[TOP] = LEVELS[pos[0]] & ~args.boards[player][TOP];
      output[BOTTOM] = LEVELS[pos[0]] & ~args.boards[player][BOTTOM];
    } else {
      output[TOP] = 0;
      output[MIDDLE] = LEVELS[pos[0]] & ~args.boards[player][MIDDLE];
    }
    return;
  } else {
    // COMPLEX LEGAL CALCULATION

    // SA BOARDS FOR ROOK, BISHOP, QUEEN, RESPECTIVELY
    uint64_t sa[3][3];

    // CALCULATE SPACIALLY AWARE BOARDS
    if (type == ROOK || type == QUEEN) {
      calc_sa(ROOK, enemy_t, args.boards, pos, sa[ROOK]);
    }
    if (type == BISHOP || type == QUEEN) {
      calc_sa(BISHOP, enemy_t, args.boards, pos, sa[BISHOP]);
    }
    if (type == QUEEN) {
      sa[QUEEN][TOP] = sa[ROOK][TOP] | sa[BISHOP][TOP];
      sa[QUEEN][MIDDLE] = sa[ROOK][MIDDLE] | sa[BISHOP][MIDDLE];
      sa[QUEEN][BOTTOM] = sa[ROOK][BOTTOM] | sa[BISHOP][BOTTOM];
    }

    // DISCOVERY CHECK RESTRICTIONS
    if (args.k_pos[player][1] == pos[1]) {
      uint64_t cur = LEVELS[pos[0]];
      if (((bishop_king & cur) == 0) && ((rook_king & cur) == 0)) {
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
        return;
      }

      uint64_t path = 0;
      uint64_t between = 0;
      uint64_t k_to_cur = 0;
      uint64_t cur_to_k = 0;
      uint64_t enemies = 0;
      if ((ONE << k_pos) > cur) {
        k_to_cur = UPPER_ZERO >> (63 - k_pos);
        cur_to_k = LOWER_ZERO << log2_lookup(cur);
      } else {
        k_to_cur = LOWER_ZERO << k_pos;
        cur_to_k = UPPER_ZERO >> (63 - log2_lookup(cur));
      }

      if ((bishop_king & cur) != 0) {
        // PIECE IS ALONG DIAGONAL FROM KING
        path = bishop_king & BISHOP_PL[5 * pos[0]];
        enemies = args.d_sliders[enemy_t][pos[1]];
      } else {
        // PIECE IS ALONG RANK/FILE FROM KING
        path = rook_king & ROOK_PL[5 * pos[0]];
        enemies = args.hv_sliders[enemy_t][pos[1]];
      }
      between = path & k_to_cur & cur_to_k;

      if (between & (args.boards[WHITE][pos[1]] |
                     args.boards[BLACK][pos[1]])) {
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
        return;
      }

      uint64_t from_k = k_to_cur & path;
      uint64_t behind_enemies = from_k & enemies;
      unsigned int closest_enemy = 0;
      if (behind_enemies) {
        if ((ONE << k_pos) > cur) {
          // CUR IN FRONT OF ENEMY
          closest_enemy = log2_lookup(behind_enemies);
          from_k &= LOWER_ZERO << closest_enemy;
        } else {
          // CUR BEHIND ENEMY
          closest_enemy = log2_lookup(behind_enemies & -behind_enemies);
          from_k &= UPPER_ZERO >> (63 - closest_enemy);
        }

        if ((from_k & (args.boards[player][pos[1]] ^ (LEVELS[pos[0]] | 
             LEVELS[k_pos]))) == 0) {
          // DIST CHECK FOUND
          if (in_check) {
            output[pos[0]] = from_k | (ONE << closest_enemy);
            if (pos[0] == TOP) {
              output[MIDDLE] = 0;
              output[BOTTOM] = 0;
            } else if (pos[0] == MIDDLE) {
              output[TOP] = 0;
              output[BOTTOM] = 0;
            } else {
              output[TOP] = 0;
              output[MIDDLE] = 0;
            }
            output[TOP] &= total_check[TOP]; 
            output[MIDDLE] &= total_check[MIDDLE]; 
            output[BOTTOM] &= total_check[BOTTOM]; 
          } else {
            output[pos[1]] = from_k | (ONE << closest_enemy);
            if (pos[1] == TOP) {
              output[MIDDLE] = 0;
              output[BOTTOM] = 0;
            } else if (pos[1] == MIDDLE) {
              output[TOP] = 0;
              output[BOTTOM] = 0;
            } else {
              output[TOP] = 0;
              output[MIDDLE] = 0;
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

void calc_sa(TYPE type, SIDE enemy_t, uint64_t (*boards)[3],
                          unsigned int *pos, uint64_t *sa) {
  uint64_t *pl;
  if (type == BISHOP) {
    pl = BISHOP_PL + (5 * pos[0]);
  } else {
    pl = ROOK_PL + (5 * pos[0]);
  }

  // GRAB PSEUDO LEGAL MOVES FOR CURRENT LEVEL
  sa[pos[1]] = pl[0];

  // CANNOT MOVE DOWN OR UP WHERE FRIENDS ARE
  SIDE player = enemy_t == WHITE ? BLACK : WHITE;
  if (pos[1] == TOP) {
    sa[MIDDLE] = (LEVELS[pos[0]] ^ boards[player][MIDDLE]) & LEVELS[pos[0]];
    sa[BOTTOM] = 0;
  } else if (pos[1] == MIDDLE) {
    sa[TOP] = (LEVELS[pos[0]] ^ boards[player][TOP]) & LEVELS[pos[0]];
    sa[BOTTOM] = (LEVELS[pos[0]] ^ boards[player][BOTTOM]) & LEVELS[pos[0]];
  } else {
    sa[TOP] = 0;
    sa[MIDDLE] = (LEVELS[pos[0]] ^ boards[player][MIDDLE]) & LEVELS[pos[0]];
  }

  // ELIMINATE POSITIONS OF FRIENDS AND ENEMIES
  sa[pos[1]] &= (sa[pos[1]] ^
                (boards[enemy_t][pos[1]] | boards[player][pos[1]]));

  uint64_t attacks = 0;

  // "SHADOW" FRIENDS AND ENEMIES
  uint64_t up = pl[1];
  uint64_t temp = ~sa[pos[1]] & pl[1];
  if (temp) {
    up &= UPPER_ZERO >> (63 - log2_lookup(temp & -temp));
    attacks |= up & boards[enemy_t][pos[1]];
  }

  uint64_t right = sa[pos[1]] & pl[2];
  temp = ~sa[pos[1]] & pl[2];
  if (type == BISHOP) {
    if (temp) {
      right &= UPPER_ZERO >> (63 - log2_lookup(temp & -temp));
      attacks |= right & boards[enemy_t][pos[1]];
    }
  } else {
    if (temp) {
      right &= LOWER_ZERO << log2_lookup(temp);
      attacks |= right & boards[enemy_t][pos[1]];
    }
  }

  uint64_t down = sa[pos[1]] & pl[3];
  temp = ~sa[pos[1]] & pl[3];
  if (temp) {
    down &= LOWER_ZERO << log2_lookup(temp);
    attacks |= down & boards[enemy_t][pos[1]];
  }

  uint64_t left = sa[pos[1]] & pl[4];
  temp = ~sa[pos[1]] & pl[4];
  if (type == BISHOP) {
    if (temp) {
      left &= LOWER_ZERO << log2_lookup(temp);
      attacks |= left & boards[enemy_t][pos[1]];
    }
  } else {
    if (temp) {
      left &= UPPER_ZERO >> (63 - log2_lookup(temp & -temp));
      attacks |= left & boards[enemy_t][pos[1]];
    }
  }

  // COMBINE SHADOWS
  sa[pos[1]] = up | right | down | left;
  // RENABLE ATTACKING
  sa[pos[1]] |= attacks;
}

void slider_check_detect(uint64_t *sk, uint64_t sk_xray, uint64_t *pl,
                         unsigned int *pos, uint64_t *sliders,
                         uint64_t *output, unsigned int *double_check) {
  unsigned int num_check = 0;
  output[pos[1]] = sk[pos[1]] & sliders[pos[1]];
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
    unsigned int e_pos = log2_lookup(output[pos[1]]);

    uint64_t between_mask = 0;
    if (pos[0] > e_pos) {
      between_mask = (UPPER_ZERO >> (63 - pos[0])) & (LOWER_ZERO << e_pos);
    } else {
      between_mask = (UPPER_ZERO >> (63 - e_pos)) & (LOWER_ZERO << pos[0]);
    }
    output[pos[1]] = sk_xray & (pl[5 * e_pos] | LEVELS[e_pos]) & between_mask;
  }
}

void jump_check_detect(unsigned int *pos, uint64_t pl, uint64_t *piece_board,
                       uint64_t *output, unsigned int *double_check) {
  unsigned int num_check = 0;
  output[pos[1]] = pl & piece_board[pos[1]];
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
  }
}
