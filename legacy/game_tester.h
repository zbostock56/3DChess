#include <psuedo_legal_moves.h>

MOVE search(BOARD_ARGS *args, SIDE to_move, unsigned int depth);
void get_legal(SIDE player, unsigned int *pos, TYPE type,
               BOARD_ARGS args, uint64_t *output);
unsigned int log2_lookup(uint64_t);

// Visual Functions
void print_bitboard(uint64_t b, FILE *fp);
void printf_bitboard(uint64_t b);
void print_boards(BOARD_ARGS *, FILE *);

