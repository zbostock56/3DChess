#ifndef main_h
  #define main_h
  #include <unistd.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <stdint.h>
  #include <limits.h>
  #include <string.h>
  #include <math.h>
  #include <ctype.h>
  #include <pthread.h>

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
  } S_INFO;

  //***START WHITE PAWNS***
  extern int w_pawn_len;
  extern uint64_t W_PAWN_PL[64];
  //**START BLACK PAWNS***
  extern int b_pawn_len;
  extern uint64_t B_PAWN_PL[64];
  //**START WHITE ATTACK PAWNS***
  extern int w_pawn_attack_len;// = 64;
  extern uint64_t W_PAWN_ATTACK[64];
  //**START BLACK ATTACK PAWNS***
  extern int b_pawn_attack_len;
  extern uint64_t B_PAWN_ATTACK[64];
  //**START KNIGHT MOVES***
  extern int knight_len;
  extern uint64_t KNIGHT_PL[64];
  //**START ROOK MOVES***
  extern int rook_len;
  extern uint64_t ROOK_PL[320];
  //**START BISHOP MOVES***
  extern int bishop_len;
  extern uint64_t BISHOP_PL[320];
  //**START KING MOVES***
  extern int king_len;
  extern uint64_t KING_PL[64];
  //***START LEVELING MOVES
  extern int num_levels;
  extern uint64_t LEVELS[64];
#endif
