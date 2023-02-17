#include <psuedo_legal_moves.h>
#include <pthread.h>

MOVE search(BOARD_ARGS *args, SIDE to_move, unsigned int depth, unsigned int alpha,
            unsigned int beta);
MOVE evaluate(BOARD_ARGS *args, SIDE to_move, uint32_t p_flags, uint32_t e_flags);
void make_move(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
               unsigned int *from, unsigned int *to);
void get_legal(SIDE player, unsigned int *pos, TYPE type,
               BOARD_ARGS args, uint64_t *output, uint32_t *flags);
unsigned int log2_lookup(uint64_t);

/*
typedef struct s_info {
  BOARD_ARGS *args;
  SIDE to_move;
  int depth;
};
*/
