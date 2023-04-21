#include <legal_moves.h>

void get_legal(SIDE player, unsigned int *pos, TYPE type,
               BOARD_ARGS args, uint64_t *output, uint32_t *flags) {
  /*
    POS[1] = Bitposition
    POS[0] = Level
  */

  //CHECK DETECTION
  unsigned int k_pos = args.k_pos[player][1];
  SIDE enemy_t = player == WHITE ? BLACK : WHITE;
  uint64_t bishop_king = (BISHOP_PL + (5 * k_pos))[0];
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
  uint64_t in_check;
  uint64_t d_check[3] = { 0, 0, 0 };
  slider_check_detect(bk_sa, bishop_king, BISHOP_PL, args.k_pos[player],
                      args.d_sliders[enemy_t], d_check, &double_check);

  uint64_t hv_check[3] = { 0, 0, 0 };
  slider_check_detect(rk_sa, rook_king, ROOK_PL, args.k_pos[player],
                      args.hv_sliders[enemy_t], hv_check, &double_check);

  uint64_t jp_check[3] = { 0, 0, 0 };
  uint64_t jn_check[3] = { 0, 0, 0 };
  uint64_t p_attack = player == WHITE ? W_PAWN_ATTACK[args.k_pos[player][1]] :
                                       B_PAWN_ATTACK[args.k_pos[player][1]];
  uint64_t kp_enemies[3];
  uint64_t kn_enemies[3];
  kp_enemies[TOP] = args.piece_boards[enemy_t][PAWN][TOP];
  kp_enemies[MIDDLE] = args.piece_boards[enemy_t][PAWN][MIDDLE];
  kp_enemies[BOTTOM] = args.piece_boards[enemy_t][PAWN][BOTTOM];
  kn_enemies[TOP] = args.piece_boards[enemy_t][KNIGHT][TOP];
  kn_enemies[MIDDLE] = args.piece_boards[enemy_t][KNIGHT][MIDDLE];
  kn_enemies[BOTTOM] = args.piece_boards[enemy_t][KNIGHT][BOTTOM];

  jump_check_detect(args.k_pos[player],
                    KNIGHT_PL[args.k_pos[player][1]], kn_enemies,
                    jn_check, &double_check);
  jump_check_detect(args.k_pos[player],
                    p_attack, kp_enemies,
                    jp_check, &double_check);

  uint64_t total_check[3] = { 0, 0, 0 };
  total_check[TOP] = d_check[TOP] | hv_check[TOP] | jp_check[TOP] | jn_check[TOP];
  total_check[MIDDLE] = d_check[MIDDLE] | hv_check[MIDDLE] | jp_check[MIDDLE] | jn_check[MIDDLE];
  total_check[BOTTOM] = d_check[BOTTOM] | hv_check[BOTTOM] | jp_check[BOTTOM] | jn_check[BOTTOM];
  in_check = total_check[TOP] | total_check[MIDDLE] | total_check[BOTTOM];


  // SET CHECK FLAGS
  uint64_t warning_board[3] = { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff };
  if (in_check) {
    *flags |= CHECK;
  }

  // LEGAL MOVES
  if (type == KING) {
    BOARD_ARGS copy;
    // KING
    if (*flags & CHECK) {
      uint64_t bc_sa[3];
      uint64_t rc_sa[3];
      uint64_t check_bishop;
      uint64_t check_rook;
      unsigned int c_pos;
      unsigned int temp;
      uint64_t t_check[3] = { 0, 0, 0 };
      unsigned int pos_tuple[2];
      for (int i = 0; i < NUM_LEVELS; i++) {
        pos_tuple[0] = i;
        while (total_check[i]) {
          c_pos = log2_lookup(total_check[i]);
          total_check[i] ^= (ONE << c_pos);
          t_check[0] = 0;
          t_check[1] = 0;
          t_check[2] = 0;
          check_bishop = (BISHOP_PL + (5 * c_pos))[0];
          check_rook = (ROOK_PL + (5 * c_pos))[0];
          pos_tuple[1] = c_pos;
          make_temp_copy(&args, &copy);
          make_move(&copy, player, KING, args.k_pos[player], pos_tuple);
          calc_sa(BISHOP, enemy_t, copy.boards, pos_tuple, bc_sa);
          calc_sa(ROOK, enemy_t, copy.boards, pos_tuple, rc_sa);
          slider_check_detect(bc_sa, check_bishop, BISHOP_PL, pos_tuple,
            copy.d_sliders[enemy_t], d_check, &temp);
          slider_check_detect(rc_sa, check_rook, ROOK_PL, pos_tuple,
            copy.hv_sliders[enemy_t], hv_check, &temp);
          jump_check_detect(pos_tuple,
            KNIGHT_PL[c_pos], kn_enemies, jn_check, &temp);
          jump_check_detect(pos_tuple,
            (player == WHITE ? W_PAWN_ATTACK[c_pos] : B_PAWN_ATTACK[c_pos]),
            kp_enemies, jp_check, &temp);
          t_check[TOP] = d_check[TOP] | hv_check[TOP] | jp_check[TOP] | jn_check[TOP];
          t_check[MIDDLE] = d_check[MIDDLE] | hv_check[MIDDLE] | jp_check[MIDDLE] | jn_check[MIDDLE];
          t_check[BOTTOM] = d_check[BOTTOM] | hv_check[BOTTOM] | jp_check[BOTTOM] | jn_check[BOTTOM];
          if (t_check[TOP] | t_check[MIDDLE] | t_check[BOTTOM]) {
            warning_board[i] ^= (ONE << c_pos);
          }
        }
      }
    }
    uint64_t under_attack[3] = { 0, 0, 0 };
    uint64_t sa[2][3];
    uint64_t pl = KING_PL[pos[1]];
    unsigned int s_pos;
    unsigned int coord[2];
    coord[0] = pos[0];

    while (pl) {
      s_pos = log2_lookup(pl);
      pl ^= (ONE << s_pos);

      coord[1] = s_pos;
      make_temp_copy(&args, &copy);
      make_move(&copy, player, KING, args.k_pos[player], coord);
      calc_sa(BISHOP, enemy_t, copy.boards, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, copy.boards, coord, sa[ROOK]);

      if (sa[BISHOP][pos[0]] & args.d_sliders[enemy_t][pos[0]] ||
          sa[ROOK][pos[0]] & args.hv_sliders[enemy_t][pos[0]]) {
        under_attack[coord[0]] |= (ONE << coord[1]);
      }
    }

    coord[1] = pos[1];
    output[pos[0]] = under_attack[pos[0]] ^ KING_PL[pos[1]];
    if (pos[0] == TOP) {
      coord[0] = MIDDLE;
      calc_sa(BISHOP, enemy_t, args.boards, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, args.boards, coord, sa[ROOK]);
      if (sa[BISHOP][pos[0]] & args.d_sliders[enemy_t][pos[0]] ||
          sa[ROOK][pos[0]] & args.hv_sliders[enemy_t][pos[0]]) {
        under_attack[MIDDLE] |= (ONE << pos[1]);
      }
      if (args.boards[enemy_t][BOTTOM] & LEVELS[pos[1]]) {
        under_attack[MIDDLE] |= LEVELS[pos[1]];
      }
      output[MIDDLE] = (ONE << pos[1]) ^ under_attack[MIDDLE];
      output[BOTTOM] = 0;
    } else if (pos[0] == MIDDLE) {
      coord[0] = TOP;
      calc_sa(BISHOP, enemy_t, args.boards, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, args.boards, coord, sa[ROOK]);
      if (sa[BISHOP][pos[0]] & args.d_sliders[enemy_t][pos[0]] ||
          sa[ROOK][pos[0]] & args.hv_sliders[enemy_t][pos[0]]) {
        under_attack[TOP] |= (ONE << pos[1]);
      }
      output[TOP] = (ONE << pos[1]) ^ under_attack[TOP];

      coord[0] = BOTTOM;
      calc_sa(BISHOP, enemy_t, args.boards, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, args.boards, coord, sa[ROOK]);
      if (sa[BISHOP][pos[0]] & args.d_sliders[enemy_t][pos[0]] ||
          sa[ROOK][pos[0]] & args.hv_sliders[enemy_t][pos[0]]) {
        under_attack[BOTTOM] |= (ONE << pos[1]);
      }
      output[BOTTOM] = (ONE << pos[1]) ^ under_attack[BOTTOM];
    } else {
      coord[0] = MIDDLE;
      calc_sa(BISHOP, enemy_t, args.boards, coord, sa[BISHOP]);
      calc_sa(ROOK, enemy_t, args.boards, coord, sa[ROOK]);
      if (sa[BISHOP][pos[0]] & args.d_sliders[enemy_t][pos[0]] ||
          sa[ROOK][pos[0]] & args.hv_sliders[enemy_t][pos[0]]) {
        under_attack[MIDDLE] |= (ONE << pos[1]);
      }
      if (args.boards[enemy_t][TOP] & LEVELS[pos[1]]) {
        under_attack[MIDDLE] |= LEVELS[pos[1]];
      }
      output[MIDDLE] = (ONE << pos[1]) ^ under_attack[MIDDLE];
      output[TOP] = 0;
    }
    output[TOP] &= ~args.boards[player][TOP];
    output[MIDDLE] &= ~args.boards[player][MIDDLE];
    output[BOTTOM] &= ~args.boards[player][BOTTOM];
    output[TOP] &= warning_board[TOP];
    output[MIDDLE] &= warning_board[MIDDLE];
    output[BOTTOM] &= warning_board[BOTTOM];

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
      pl = (player == WHITE ? W_PAWN_PL[pos[1]] : B_PAWN_PL[pos[1]]);
      if (pos[1] >= 15 && pos[1] <= 47) {
        pl &= ~args.boards[enemy_t][pos[0]];
      } else {
        // SHADOW TO PREVENT PAWN FIRST MOVE FROM JUMPING OVER OTHER PIECE
        int front = pos[1];
        if (player == WHITE) {
          front += 8;
        } else {
          front -= 8;
        }
        if (args.boards[enemy_t][pos[0]] & LEVELS[front]) {
          // THERE IS AN ENEMY IN THE IMMEDIATE FRONT
          pl = 0;
        } else {
          pl &= ~args.boards[enemy_t][pos[0]];
        }
      }
      pl |= ((player == WHITE ? W_PAWN_ATTACK[pos[1]] : B_PAWN_ATTACK[pos[1]]) & args.boards[enemy_t][pos[0]]);
    } else {
      pl = KNIGHT_PL[pos[1]];
    }
    output[pos[0]] = pl & ~args.boards[player][pos[0]];
    if (pos[0] == TOP) {
      output[MIDDLE] = LEVELS[pos[1]] & ~args.boards[player][MIDDLE];
      output[BOTTOM] = 0;
    } else if (pos[0] == MIDDLE) {
      output[TOP] = LEVELS[pos[1]] & ~args.boards[player][TOP];
      output[BOTTOM] = LEVELS[pos[1]] & ~args.boards[player][BOTTOM];
    } else {
      output[TOP] = 0;
      output[MIDDLE] = LEVELS[pos[1]] & ~args.boards[player][MIDDLE];
    }
    if (*flags & CHECK) {
      output[TOP] &= total_check[TOP];
      output[MIDDLE] &= total_check[MIDDLE];
      output[BOTTOM] &= total_check[BOTTOM];
      //if ((total_check[TOP] & ~output[TOP]) || (total_check[MIDDLE] & ~output[MIDDLE]) || (total_check[BOTTOM] & ~output[BOTTOM])) {
      //  output[TOP] = 0;
      //  output[MIDDLE] = 0;
      //  output[BOTTOM] = 0;
      //}
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
    if (args.k_pos[player][0] == pos[0]) {
      uint64_t cur = LEVELS[pos[1]];
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
        //if ((total_check[TOP] & ~output[TOP]) || (total_check[MIDDLE] & ~output[MIDDLE]) || (total_check[BOTTOM] & ~output[BOTTOM])) {
        //  output[TOP] = 0;
        //  output[MIDDLE] = 0;
        //  output[BOTTOM] = 0;
        //}
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
        path = bishop_king & BISHOP_PL[5 * pos[1]];
        enemies = args.d_sliders[enemy_t][pos[0]];
      } else {
        // PIECE IS ALONG RANK/FILE FROM KING
        path = rook_king & ROOK_PL[5 * pos[1]];
        enemies = args.hv_sliders[enemy_t][pos[0]];
      }
      between = path & k_to_cur & cur_to_k;

      if (between & (args.boards[WHITE][pos[0]] |
                     args.boards[BLACK][pos[0]])) {
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
        //if ((total_check[TOP] & ~output[TOP]) || (total_check[MIDDLE] & ~output[MIDDLE]) || (total_check[BOTTOM] & ~output[BOTTOM])) {
        //  output[TOP] = 0;
        //  output[MIDDLE] = 0;
        //  output[BOTTOM] = 0;
        //}
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

        if ((from_k & (args.boards[player][pos[0]] ^ (LEVELS[pos[1]] | 
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
            output[TOP] &= (sa[type][TOP] & total_check[TOP]);
            output[MIDDLE] &= (sa[type][MIDDLE] & total_check[MIDDLE]);
            output[BOTTOM] &= (sa[type][BOTTOM] & total_check[BOTTOM]);
          } else {
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

            output[TOP] &= sa[type][TOP];
            output[MIDDLE] &= sa[type][MIDDLE];
            output[BOTTOM] &= sa[type][BOTTOM];
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
    //if ((total_check[TOP] & ~output[TOP]) || (total_check[MIDDLE] & ~output[MIDDLE]) || (total_check[BOTTOM] & ~output[BOTTOM])) {
    //  output[TOP] = 0;
    //  output[MIDDLE] = 0;
    //  output[BOTTOM] = 0;
    //}
  }
}

void calc_sa(TYPE type, SIDE enemy_t, uint64_t (*boards)[3],
                          unsigned int *pos, uint64_t *sa) {
  uint64_t *pl;
  if (type == BISHOP) {
    pl = BISHOP_PL + (5 * pos[1]);
  } else {
    pl = ROOK_PL + (5 * pos[1]);
  }

  // GRAB PSEUDO LEGAL MOVES FOR CURRENT LEVEL
  sa[pos[0]] = pl[0];

  // CANNOT MOVE DOWN OR UP WHERE FRIENDS ARE
  SIDE player = enemy_t == WHITE ? BLACK : WHITE;
  if (pos[0] == TOP) {
    sa[MIDDLE] = (LEVELS[pos[1]] ^ boards[player][MIDDLE]) & LEVELS[pos[1]];
    sa[BOTTOM] = 0;
  } else if (pos[0] == MIDDLE) {
    sa[TOP] = (LEVELS[pos[1]] ^ boards[player][TOP]) & LEVELS[pos[1]];
    sa[BOTTOM] = (LEVELS[pos[1]] ^ boards[player][BOTTOM]) & LEVELS[pos[1]];
  } else {
    sa[TOP] = 0;
    sa[MIDDLE] = (LEVELS[pos[1]] ^ boards[player][MIDDLE]) & LEVELS[pos[1]];
  }

  // ELIMINATE POSITIONS OF FRIENDS AND ENEMIES
  sa[pos[0]] &= (sa[pos[0]] ^
                (boards[enemy_t][pos[0]] | boards[player][pos[0]]));

  uint64_t attacks = 0;
  unsigned int closest_bit = 0;

  // "SHADOW" FRIENDS AND ENEMIES
  uint64_t up = sa[pos[0]] & pl[1];
  uint64_t temp = ~sa[pos[0]] & pl[1];
  // GET LEAST SIG BIT IN THE FORM OF A BITBOARD
  //temp &= -temp;
  //temp &= ~boards[player][pos[0]];
  if (temp) {
    // WANT LEAST SIG BIT
    //closest_bit = log2_lookup(temp);
    closest_bit = temp & -temp;
    if (!((temp & -temp) & boards[player][pos[0]])) {
      // BEING ATTACKED
      attacks |= (ONE << closest_bit);
    }
    up &= UPPER_ZERO >> (63 - /*log2_lookup(temp & -temp)*/closest_bit);
    //attacks |= up & boards[enemy_t][pos[0]];
  }

  uint64_t right = sa[pos[0]] & pl[2];
  temp = ~sa[pos[0]] & pl[2];
  if (type == BISHOP) {
    //temp &= -temp;
    //temp &= ~boards[player][pos[0]];
    if (temp) {
      // WANT LSB
      //closest_bit = log2_lookup(temp);
      closest_bit = temp & -temp;
      if (!((temp & -temp) & boards[player][pos[0]])) {
        attacks |= (ONE << closest_bit);
      }
      right &= UPPER_ZERO >> (63 - /*log2_lookup(temp & -temp)*/closest_bit);
      //attacks |= right & boards[enemy_t][pos[0]];
    }
  } else {
    //if (temp) {
    //  temp = (ONE << log2_lookup(temp));
    //}
    //temp &= ~boards[player][pos[0]];
    if (temp) {
      closest_bit = log2_lookup(temp);
      if (!((ONE << closest_bit) & boards[player][pos[0]])) {
        attacks |= (ONE << closest_bit);
      }
      right &= LOWER_ZERO << closest_bit/*log2_lookup(temp)*/;
      //attacks |= right & boards[enemy_t][pos[0]];
    }
  }

  uint64_t down = sa[pos[0]] & pl[3];
  temp = ~sa[pos[0]] & pl[3];
  //if (temp) {
  //  temp = (ONE << log2_lookup(temp));
  //}
  //temp &= ~boards[player][pos[0]];
  if (temp) {
    closest_bit = log2_lookup(temp);
    if (!((ONE << closest_bit) & boards[player][pos[0]])) {
      attacks |= (ONE << closest_bit);
    }
    down &= LOWER_ZERO << closest_bit/*log2_lookup(temp)*/;
    //attacks |= down & boards[enemy_t][pos[0]];
  }

  uint64_t left = sa[pos[0]] & pl[4];
  temp = ~sa[pos[0]] & pl[4];
  if (type == BISHOP) {
    //if (temp) {
    //  temp = (ONE << log2_lookup(temp));
    //}
    //temp &= ~boards[player][pos[0]];
    if (temp) {
      closest_bit = log2_lookup(temp);
      if (!((ONE << closest_bit) & boards[player][pos[0]])) {
        attacks |= (ONE << closest_bit);
      }
      left &= LOWER_ZERO << closest_bit/*log2_lookup(temp)*/;
      //attacks |= left & boards[enemy_t][pos[0]];
    }
  } else {
    //temp &= -temp;
    //temp &= ~boards[player][pos[0]];
    if (temp) {
      // WANT LSB
      //closest_bit = log2_lookup(temp);
      closest_bit = temp & -temp;
      if (!((temp & -temp) & boards[player][pos[0]])) {
        attacks |= (ONE << closest_bit);
      }
      left &= UPPER_ZERO >> (63 - closest_bit/*log2_lookup(temp & -temp)*/);
      //attacks |= left & boards[enemy_t][pos[0]];
    }
  }

  // COMBINE SHADOWS
  sa[pos[0]] = up | right | down | left;
  // RENABLE ATTACKING
  sa[pos[0]] |= attacks;
}

void slider_check_detect(uint64_t *sk, uint64_t sk_xray, uint64_t *pl,
                         unsigned int *pos, uint64_t *sliders,
                         uint64_t *output, unsigned int *double_check) {
  unsigned int num_check = 0;
  output[pos[0]] = sk[pos[0]] & sliders[pos[0]];
  if (pos[0] == TOP) {
    output[MIDDLE] = LEVELS[pos[1]] & sliders[MIDDLE];
    output[BOTTOM] = 0;
  } else if (pos[0] == MIDDLE) {
    output[TOP] = LEVELS[pos[1]] & sliders[TOP];
    output[BOTTOM] = LEVELS[pos[1]] & sliders[BOTTOM];
  } else {
    output[TOP] = 0;
    output[MIDDLE] = LEVELS[pos[1]] & sliders[MIDDLE];
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

  if (num_check > 1 || (output[pos[0]] ^ (output[pos[0]] & -output[pos[0]]))) {
    *double_check = 1;
  } else if (output[pos[0]]) {
    // Check is vector between slider and king
    unsigned int e_pos = log2_lookup(output[pos[0]]);

    uint64_t between_mask = 0;
    if (pos[1] > e_pos) {
      between_mask = (UPPER_ZERO >> (63 - pos[1])) & (LOWER_ZERO << e_pos);
    } else {
      between_mask = (UPPER_ZERO >> (63 - e_pos)) & (LOWER_ZERO << pos[1]);
    }
    output[pos[0]] = sk_xray & (pl[5 * e_pos] | LEVELS[e_pos]) & between_mask;
  }
}

void jump_check_detect(unsigned int *pos, uint64_t pl, uint64_t *piece_board,
                       uint64_t *output, unsigned int *double_check) {
  unsigned int num_check = 0;
  output[pos[0]] = pl & piece_board[pos[0]];
  if (pos[0] == TOP) {
    output[MIDDLE] = LEVELS[pos[1]] & piece_board[MIDDLE];
    output[BOTTOM] = 0;
  } else if (pos[0] == MIDDLE) {
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

  if (num_check > 1 || (output[pos[0]] & -output[pos[0]])) {
    *double_check = 1;
  }
}
