#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <helpers.h>

void get_legal(SIDE player, unsigned int *pos, TYPE type,
               BOARD_ARGS args, uint64_t *output);
unsigned int log2_lookup(uint64_t);

// Visual Functions
void print_bitboard(uint64_t b, FILE *fp);
void printf_bitboard(uint64_t b);
void print_boards(BOARD_ARGS *, FILE *);

