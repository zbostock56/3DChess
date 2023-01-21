#include <legal_moves.h>

uint64_t get_legal(SIDE player, unsigned int pos[2], PIECE_TYPE type,
                   BOARD_ARGS args) {
  if (type == PAWN || type == KNIGHT || type == KING) {
    // SIMPLE

  } else {
    // COMPLEX LEGAL CALCULATION
    uint64_t sa_rook[3];
    uint64_t sa_bishop[3];
    uint64_t sa_queen[3];

    // CALCULATE SPACIALLY AWARE BOARDS
    PIECE_TYPE enemy_t = type == WHITE ? BLACK : WHITE;
    if (type == ROOK || type == QUEEN) {
      calc_spacially_aware(ROOK, pos, SA_rook);
    }
    if (type == BISHOP || type == QUEEN) {
      calc_spacially_aware(BISHOP, pos, SA_bishop);
    }
    if (type == QUEEN) {
      sa_queen[0] = sa_rook[0] | sa_bishop[0];
      sa_queen[1] = sa_rook[1] | sa_bishop[1];
      sa_queen[2] = sa_rook[2] | sa_bishop[2];
    }

    // DISCOVERY CHECK RESTRICTIONS
  }
}

void calc_spacially_aware(TYPE type, unsigned int *pos, uint64_t *sa) {
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
    sa[BOTTOM] = LEVLES[pos[0]] & args.boards[type][BOTTOM];
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
