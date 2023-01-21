#include <stdio.h>
#include <stdint.h>

#define UPPER_ZERO = 0x7FFFFFFFFFFFFFFF
#define LOWER_ZERO = 0xFFFFFFFFFFFFFFFF

typedef enum {
  PAWN = 0,
  KNIGHT = 1,
  BISHOP = 2,
  ROOK = 3,
  QUEEN = 4,
  KING = 
} TYPE;

typedef enum {
  WHITE = 0,
  BLACK = 1
} SIDE;

typedef enum {
  TOP = 0,
  MIDDLE = 1,
  BOTTOM = 2
} LEVEL;

typedef struct board_args {
  uint64_t boards[2][3];
  uint64_t piece_boards[2][6][3];
  uint64_t hv_sliders[2][3];
  uint64_t d_sliders[2][3];
  unsigned int k_pos[2][2];
} BOARD_ARGS;

uint64_t calc_sa(TYPE type, SIDE enemy_t, unsigned int *pos,
                              uint64_t *sa);
