#include <psuedo_legal_moves.h>
#include <pthread.h>
#define MAX_NUM_LEGAL_MOVES (204)
MOVE search(BOARD_ARGS *args, SIDE to_move, unsigned int depth, unsigned int alpha,
            unsigned int beta, int turn);
MOVE evaluate(BOARD_ARGS *args, SIDE to_move, uint32_t p_flags, uint32_t e_flags
              , int turn);
void make_move(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
               unsigned int *from, unsigned int *to);
void get_legal(SIDE player, unsigned int *pos, TYPE type,
               BOARD_ARGS args, uint64_t *output, uint32_t *flags);
unsigned int log2_lookup(uint64_t);
void print_game(BOARD_ARGS *args);
int ms_bit_lookup(uint64_t *input);
void make_temp_copy(BOARD_ARGS *source, BOARD_ARGS *dest);
int get_max_min(int is_max, int one, int two);
MOVE level_zero_search(BOARD_ARGS *args, unsigned int turn, SIDE to_move,
                       unsigned int depth, unsigned int alpha);
void s_th_wrapper(void *arg);

MOVE move_list[MAX_NUM_LEGAL_MOVES];
pthread_t threads[MAX_NUM_LEGAL_MOVES];
