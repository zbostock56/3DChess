#include <legal_moves.h>

uint64_t get_legal(SIDE player, unsigned int pos[2], TYPE type,
                   BOARD_ARGS args) {
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

  // TODO CALCULATE CHECK MULTIDIMENTIONALLY, IS CURRENTLY 2D

  uint64_t used_bk = bk_sa[args.k_pos[1]];
  used_bk &= ~args.d_sliders[enemy_t][args.k_pos[1]];
  if (used_bk && used_bk ^ (1 << log(used_bk))) {
    // DOUBLE CHECK
    return 0;
  } else if (used_bk) {
    // Check is vector between bishop and king
    unsigned int e_pos = log(used_bk);
    uint64_t between_mask = 0;
    if (k_pos > e_pos) {
      between_mask = (UPPER_ZERO >> (63 - k_pos)) & (LOWER_ZERO << e_pos);
    } else {
      between_mask = (UPPER_ZERO >> (63 - e_pos)) & (LOWER_ZERO << k_pos);
    }
    check = bishop_king & (BISHOP_PL[e_pos] | LEVELS[e_pos]) & between_mask;
  }

  uint64_t used_rk = rk_sa[args.k_pos[1]];
  used_rk &= ~args.hv_sliders[enemy_t][args.k_pos[1]];
  if ((used_rk && used_rk ^ (1 << log(used_rk))) || (used_rk && check)) {
    // DOUBLE CHECK
    return 0;
  } else if (used_bk) {
    // Check is vector between rook and king
    unsigned int e_pos = log(used_rk);
    uint64_t between_mask = 0;
    if (k_pos > e_pos) {
      between_mask = (UPPER_ZERO >> (63 - k_pos)) & (LOWER_ZERO << e_pos);
    } else {
      between_mask = (UPPER_ZERO >> (63 - e_pos)) & (LOWER_ZERO << k_pos);
    }
    check = rook_king & (ROOK_PL[e_pos] | LEVELS[e_pos]) & between_mask;
  }

  uint64_t k_threat = KNIGHT_PL[k_pos] &
                      args.piece_boards[enemy_t][KNIGHT][args.k_pos[1]];
  if ((k_threat && k_threat ^ (1 << log(k_threat))) || (k_threat && check)) {
    // DOUBLE CHECK
    return 0;
  } else if (k_threat) {
    check = k_threat;
  }

  uint64_t *p_attack = type == WHITE ? B_PAWN_ATTACK : W_PAWN_ATTACK;
  uint64_t p_threat = p_attack[args.k_pos[1]];
  if ((p_threat && p_threat ^ (1 << log(p_threat))) || (p_threat && check)) {
    // DOUBLE CHECK
    return 0;
  } else if (p_threat) {
    check = p_threat;
  }

  if (type == PAWN || type == KNIGHT || type == KING) {
    // SIMPLE

  } else {
    // COMPLEX LEGAL CALCULATION
    uint64_t sa_rook[3];
    uint64_t sa_bishop[3];
    uint64_t sa_queen[3];

    // CALCULATE SPACIALLY AWARE BOARDS
    if (type == ROOK || type == QUEEN) {
      calc_sa(ROOK, enemy_t, pos, SA_rook);
    }
    if (type == BISHOP || type == QUEEN) {
      calc_sa(BISHOP, enemy_t, pos, SA_bishop);
    }
    if (type == QUEEN) {
      sa_queen[0] = sa_rook[0] | sa_bishop[0];
      sa_queen[1] = sa_rook[1] | sa_bishop[1];
      sa_queen[2] = sa_rook[2] | sa_bishop[2];
    }

    // TODO: TAKE IF KING IS CURRENTLY IN CHECK INTO ACCOUNT

    // DISCOVERY CHECK RESTRICTIONS
    if (k_pos[type][0] == pos[0]) {
      uint64_t cur = LEVELS[pos[0]];
      if (bishop_king & cur == 0 && rook_king & cur == 0) {
        // NOT IN DISC CHECK
        if (check) {
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

      if (between & (boards[WHITE][pos[0]] & boards[BLACK][pos[0]]) ==
          between) {
        // NOT IN DISC CHECK
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

        if (from_k & ~boards[type][pos[0]]) {
          // DIST CHECK FOUND
        } else {
          // NOT IN DISC CHECK
        }
      } else {
        // NOT IN DISC CHECK
      }
    } else {
      // NOT IN DISC CHECK
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
