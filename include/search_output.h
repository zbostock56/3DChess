#include <psuedo_legal_moves.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

void search_output(BOARD_ARGS *args, SIDE to_move);
void translate_position(BOARD_ARGS *args, int arr_pos);
void reset_possible_moves();
void output_to_file(BOARD_ARGS *args, SIDE to_move);
unsigned int log2_lookup(uint64_t);
int ms_bit_lookup(uint64_t *input);
void get_legal(SIDE player, unsigned int *pos, TYPE type,
               BOARD_ARGS args, uint64_t *output, uint32_t *flags);
void make_move(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
               unsigned int *from, unsigned int *to);
void print_game(BOARD_ARGS *args);
