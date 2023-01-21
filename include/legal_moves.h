#include <stdio.h>
#include <stdint.h>

#define UPPER_ZERO = 0x7FFFFFFFFFFFFFFF
#define LOWER_ZERO = 0xFFFFFFFFFFFFFFFF

typedef enum {
  BISHOP = 0,
  ROOK = 1,
  QUEEN = 2,
  PAWN = 3,
  KNIGHT = 4,
  KING = 5
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
void slider_check_detect(uint64_t *sk, uint64_t sk_xray, uint64_t *pl,
                         unsigned int *pos, uint64_t *sliders,
                         uint64_t *output, int *double_check);

void jump_check_detect(uint64_t *pos, uint64_t *pl, uint64_t *piece_board,
                       uint64_t *output, unsigned int *double_check);
