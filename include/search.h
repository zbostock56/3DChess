#include <stdio.h>
#include <helpers.h>

int evaluate(BOARD_ARGS *args, int *scores);
BOARD_ARGS make_move(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
               unsigned int *from, unsigned int *to);
void get_legal(SIDE player, unsigned int *pos, TYPE type,
               BOARD_ARGS args, uint64_t *output);
