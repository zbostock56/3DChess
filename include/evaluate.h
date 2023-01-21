#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <helpers.h>

#define BISHOP_VAL (3)
#define ROOK_VAL (5)
#define QUEEN_VAL (9)
#define PAWN_VAL (1)
#define KNIGHT_VAL (3)

int evaluate(BOARD_ARGS *args, int *scores);
