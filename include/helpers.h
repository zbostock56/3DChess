#define UPPER_ZERO (0x7FFFFFFFFFFFFFFF)
#define LOWER_ZERO (0xFFFFFFFFFFFFFFFF)

#define MATE (0x00000001)
#define D_CHECK (0x00000002)
#define CHECK (0x00000004)

#define B_VAL (3)
#define R_VAL (5)
#define Q_VAL (9)
#define P_VAL (1)
#define K_VAL (3)

#define PIECE_TYPES 6
#define NUM_LEVELS 3

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
/*
  boards: Overall
  piece_boards: only contains pieces
  hv_sliders: contains horz and vert sliding pieces
  d_sliders: contains diag sliding pieces
*/
typedef struct board_args {
  uint64_t boards[2][3];
  uint64_t piece_boards[2][6][3];
  uint64_t hv_sliders[2][3];
  uint64_t d_sliders[2][3];
  unsigned int k_pos[2][2];
} BOARD_ARGS;

typedef struct move {
  BOARD_ARGS *args;
  SIDE to_move;
  unsigned int from[2];
  unsigned int to[2];
  int score;
  unsigned int rating;
} MOVE;

typedef struct s_info {
  BOARD_ARGS *args;
  SIDE to_move;
  int depth;
  int alpha;
  int beta;
  unsigned int turn;
  unsigned int id;
};

unsigned int log2_lookup(uint64_t);
void print_bitboard(uint64_t b, FILE *fp);
void print_bitboards(uint64_t *b, FILE *fp);
void printf_bitboard(uint64_t b);
void printf_bitboards(uint64_t *b);
unsigned int get_piece_num(uint64_t *board);
