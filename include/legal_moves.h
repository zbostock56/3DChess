#include <stdio.h>
#include <stdint.h>
#include <psuedo_legal_moves.h>
#include <helpers.h>

void calc_sa(TYPE type, SIDE enemy_t, uint64_t (*boards)[3],
                 unsigned int *pos, uint64_t *sa);
void slider_check_detect(uint64_t *sk, uint64_t sk_xray, uint64_t *pl,
                         unsigned int *pos, uint64_t *sliders,
                         uint64_t *output, unsigned int *double_check);

void jump_check_detect(unsigned int *pos, uint64_t pl, uint64_t *piece_board,
                       uint64_t *output, unsigned int *double_check);
