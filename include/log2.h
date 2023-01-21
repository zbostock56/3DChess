#include <stdint.h>

#define L(x) x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x

const char lookup_table[] = {
 -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
 L(4), L(5), L(5), L(6), L(6), L(6), L(6),
 L(7), L(7), L(7), L(7), L(7), L(7), L(7), L(7),
 L(8), L(8), L(8), L(8), L(8), L(8), L(8), L(8),
 L(8), L(8), L(8), L(8), L(8), L(8), L(8), L(8)
};

unsigned int log2_lookup(uint64_t);
