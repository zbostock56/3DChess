#define UPPER_ZERO (0x7FFFFFFFFFFFFFFF)
#define LOWER_ZERO (0xFFFFFFFFFFFFFFFF)

#define W_MATE (0x00000001)
#define B_MATE (0x00000002)
#define WD_CHECK (0x00000004)
#define BD_CHECK (0x00000008)
#define W_CHECK (0x00000010)
#define B_CHECK (0x00000020)

static const uint64_t ONE = 1;

typedef enum {
  BISHOP = 0,
  ROOK = 1,
  QUEEN = 2,
  PAWN = 3,
  KNIGHT = 4,
  KING = 5
} TYPE;

typedef enum {
  WHITE = 0,
  BLACK = 1
} SIDE;

typedef enum {
  TOP = 0,
  MIDDLE = 1,
  BOTTOM = 2
} LEVEL;

typedef struct board_args {
  uint64_t boards[2][3];
  uint64_t piece_boards[2][6][3];
  uint64_t hv_sliders[2][3];
  uint64_t d_sliders[2][3];
  unsigned int k_pos[2][2];
} BOARD_ARGS;

unsigned int log2_lookup(uint64_t);
void print_bitboard(uint64_t b, FILE *fp);
void printf_bitboard(uint64_t b);
unsigned int get_piece_num(uint64_t *board);
