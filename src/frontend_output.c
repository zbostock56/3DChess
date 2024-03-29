#include <frontend.h>

BOARD_ARGS game;
int piece = 0;
unsigned int from[2] = {0,0};
unsigned int to[2] = {0,0};
int to_move = 0; // WHITE == 0 BLACK == 1
int change_ready = 0;
int turn = 1;
int piece_to_move = 0;
int piece_taken = 0;
int white_piece = 16;
int black_pieces = 16;
int moved = 0;

int main() {
  init_pieceboard(game);
  output_to_file(&game, WHITE);
  //five_move_random(&game, WHITE);
  return 0;
}

/*
* Title: printf_bitboards()
* Params:
* -> uint64_t *b
*   -> Array of three bitboards which are
*      to be printed in a more human readable
*      format
* Description:
* Prints out the passed in array of bitboards
* in a more human readable format
*/

void printf_bitboards(uint64_t *b) {
  for (int i = 0; i < 3; i++) {
    printf("%s\n", i == 0 ? "TOP" : i == 1 ? "MIDDLE" : "BOTTOM");
    printf_bitboard(b[i]);
  }
}

/*
* Title: printf_bitboard()
* Params:
* -> uint64_t b
*   -> bitboard which is
*      to be printed in a more human readable
*      format
* Description:
* Prints out the passed bitboard
* in a more human readable format
*/

void printf_bitboard(uint64_t b) {
  #ifdef __linux__
    printf("%ld:\n", b);
  #elif __APPLE__
    printf("%lld:\n", b);
  #endif
  for (int i = 63; i >= 0; i--) {
    if (b & (ONE << i)) {
      printf("1 ");
    } else {
      printf("0 ");
    }
    if (i % 8 == 0) {
      printf("\n");
    }
  }
  printf("\n");
}


