#ifndef evaluate_h
  #define evaluate_h
  #ifndef main_h
    #include <main.h>
  #endif
//  #ifndef log2_h
//    #include <log2.h>
//  #endif

  #define BISHOP_VAL (3)
  #define ROOK_VAL (5)
  #define QUEEN_VAL (9)
  #define PAWN_VAL (1)
  #define KNIGHT_VAL (3)

  MOVE evaluate(BOARD_ARGS *args, SIDE to_move, uint32_t p_flags, uint32_t e_flags,
                unsigned int turn);
  unsigned int get_piece_num(uint64_t *);
  unsigned int get_piece_num_sboard(uint64_t);
  unsigned int get_weighted_rating(unsigned int turn, SIDE player,
                                   BOARD_ARGS *args);
  #ifndef log2_h
    unsigned int log2_lookup(uint64_t);
  #endif

  /*
    ORDERING OF RATING BITBOARDS
    Knight, White King,
    Black King, Queen,
    White Rook, Black Rook,
    White Pawn, Black Pawn,
    White Bishop, Black Bishop
  */

  /*
    Knight Rating:
    -> Much higher at the start of the game, then decreases
    [+1.0]  (0) Knight moves up from back rank
    [+1.0]  (1) Knight moves up to second from back rank
    [+3.0]  (2) Knight moves into middle of board
  */

  /*
    King Rating:
    -> Decreases in value as the game progresses
    [+1.0]  (0) King stays in the back rank, but only in the center four squares
    [+2.0]  (1) King moves slightly away from back rank
    [+4.0]  (2) King moves to either the right or the left of the board
  */

  /*
    Queen Rating:
    -> Increases in value as the game progresses
    [+1.0]  (0) Queen moves generally into the middle of the board
    [+2.0]  (1) Queen closer to the center of the board
    [+4.0]  (2) Queen takes control of the center
  */

  /*
    Rook Rating:
    -> Increases as the game progresses
    [+1.0]  (0) Rook takes control of the center
    [+2.0]  (1) Rook on the back rank
    [+2.0]  (2) Rook on the second to back rank
  */

  /*
    Pawn Rating:
    -> High at the start of the game for progression, moves to higher indecies
    as more moves have been played
    [+1.0]  (0) Starting pawn move, one square from start
    [+1.0]  (1) Double move from starting position
    [+2.0]  (2) Taking center control
    ** [+3.0]  (3) Promotion **
  */

  /*
    Bishop Rating:
    -> Increases as the game progresses
    [+1.0]  (0) Bishop move onto second rank
    [+2.0]  (1) Second and third rank
    [+3.0]  (2) Takes control of center with emphesis on high value diagonals
  */
    uint64_t ratings[10][3] = {
    {
      0b0000000000000000000000000000000000000000000000000010010000000000,
      0b0000000000000000000000000000000000000000011111100101101000000000,
      0b0000000000000000001001000011110000111100000000000000000000000000
    },
    {
      0b0000000000100100000000000000000000000000000000000000000000000000,
      0b0000000001011010011111100000000000000000000000000000000000000000,
      0b0000000000000000000000000011110000111100001001000000000000000000
    },
    {
      0b0000000000000000011111000111110001111100011111000000000000000000,
      0b0000000000000000000000000000000000000000000000000000000011111111,
      0b0000000000000000000000000000000000000000000000000111111000000000
    },
    {
      0b0000000000000000011111000111110001111100011111000000000000000000,
      0b1111111100000000000000000000000000000000000000000000000000000000,
      0b0000000001111110000000000000000000000000000000000000000000000000
    },
    {
      0b0000000000111100010000101100001111000011010000100011110000000000,
      0b0000000000000000001111000010010000100100001111000000000000000000,
      0b0000000000000000000000000001100000011000000000000000000000000000
    },
    {
      0b0000000000000000000000000000000000000000000000001111111100000000,
      0b0000000000000000000000000000000000000000111111110000000000000000,
      0b0000000000000000000000000000000000111100001111000000000000000000
    },
    {
      0b0000000011111111000000000000000000000000000000000000000000000000,
      0b0000000000000000111111110000000000000000000000000000000000000000,
      0b0000000000000000001111000011110000000000000000000000000000000000
    },
    {
      0b0000000011111111000000000000000000000000000000001111111100000000,
      0b0000000000000000111111111000000110000001111111110000000000000000,
      0b0000000000000000000000000111111001111110000000000000000000000000
    },
    {
      0b0000000000000000000000000000000000000000000000000000000000111100,
      0b0000000000000000000000000000000000000000100000010000000000000000,
      0b0000000000000000000000000000000000000000000000001100001111000011
    },
    {
      0b0000000000000000011111000111110001111100011111000000000000000000,
      0b1111111100000000000000000000000000000000000000000000000000000000,
      0b0000000001111110000000000000000000000000000000000000000000000000
    }
    };
  /*
  ratings[0][1] = wb_rating[0];
  ratings[0][2] = wb_rating[1];
  ratings[0][3] = wb_rating[2];
  ratings[1][1] = bb_rating[0];
  ratings[1][2] = bb_rating[1];
  ratings[1][3] = bb_rating[2];
  ratings[2][1] = wr_rating[0];
  ratings[2][2] = wr_rating[1];
  ratings[2][3] = wr_rating[2];
  ratings[3][1] = br_rating[0];
  ratings[3][2] = br_rating[1];
  ratings[3][3] = br_rating[2];
  ratings[4][1] = q_rating[0];
  ratings[4][2] = q_rating[1];
  ratings[4][3] = q_rating[2];
  ratings[5][1] = wp_rating[0];
  ratings[5][2] = wp_rating[1];
  ratings[5][3] = wp_rating[2];
  ratings[6][1] = bp_rating[0];
  ratings[6][2] = bp_rating[1];
  ratings[6][3] = bp_rating[2];
  ratings[7][1] = n_rating[0];
  ratings[7][2] = n_rating[1];
  ratings[7][3] = n_rating[2];
  ratings[8][1] = wk_rating[0];
  ratings[8][2] = wk_rating[1];
  ratings[8][3] = wk_rating[2];
  ratings[9][1] = bk_rating[0];
  ratings[9][2] = bk_rating[1];
  ratings[9][3] = bk_rating[2];
  */
#endif
