#include <psuedo_legal_moves.h>

#define BISHOP_VAL (3)
#define ROOK_VAL (5)
#define QUEEN_VAL (9)
#define PAWN_VAL (1)
#define KNIGHT_VAL (3)

MOVE evaluate(BOARD_ARGS *args, SIDE to_move, uint32_t p_flags, uint32_t e_flags);
