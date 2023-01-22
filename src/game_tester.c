#include <game_tester.h>

int main() {
  FILE *fp = fopen("DEGUB.txt", "w");
  fprintf(fp, "**************DEGUB OUTPUT***************\n");

   BOARD_ARGS args;
  // Top of white board (all of the white pieces "OR'ed" together)
  args.boards[WHITE][TOP] = 0;
  args.boards[WHITE][MIDDLE] = 0xffff;
  args.boards[WHITE][BOTTOM] = 0;
  // All black pieces OR'ed together
  args.boards[BLACK][TOP] = 0;
  args.boards[BLACK][MIDDLE] = 0xffff000000000000;
  args.boards[BLACK][BOTTOM] = 0;

  args.piece_boards[WHITE][BISHOP][TOP] = 0;
  args.piece_boards[WHITE][BISHOP][MIDDLE] = 0x24;
  args.piece_boards[WHITE][BISHOP][BOTTOM] = 0;

  args.piece_boards[BLACK][BISHOP][TOP] = 0;
  args.piece_boards[BLACK][BISHOP][MIDDLE] = 0x2400000000000000;
  args.piece_boards[BLACK][BISHOP][BOTTOM] = 0;

  args.piece_boards[WHITE][ROOK][TOP] = 0;
  args.piece_boards[WHITE][ROOK][MIDDLE] = 0x81;
  args.piece_boards[WHITE][ROOK][BOTTOM] = 0;

  args.piece_boards[BLACK][ROOK][TOP] = 0;
  args.piece_boards[BLACK][ROOK][MIDDLE] = 0x8100000000000000;
  args.piece_boards[BLACK][ROOK][BOTTOM] = 0;

  args.piece_boards[WHITE][QUEEN][TOP] = 0;
  args.piece_boards[WHITE][QUEEN][MIDDLE] = 0x10;
  args.piece_boards[WHITE][QUEEN][BOTTOM] = 0;

  args.piece_boards[BLACK][QUEEN][TOP] = 0;
  args.piece_boards[BLACK][QUEEN][MIDDLE] = 0x1000000000000000;
  args.piece_boards[BLACK][QUEEN][BOTTOM] = 0;

  args.piece_boards[WHITE][PAWN][TOP] = 0;
  args.piece_boards[WHITE][PAWN][MIDDLE] = 0xff00;
  args.piece_boards[WHITE][PAWN][BOTTOM] = 0;

  args.piece_boards[BLACK][PAWN][TOP] = 0;
  args.piece_boards[BLACK][PAWN][MIDDLE] = 0xff000000000000;
  args.piece_boards[BLACK][PAWN][BOTTOM] = 0;

  args.piece_boards[WHITE][KNIGHT][TOP] = 0;
  args.piece_boards[WHITE][KNIGHT][MIDDLE] = 0x42;
  args.piece_boards[WHITE][KNIGHT][BOTTOM] = 0;

  args.piece_boards[BLACK][KNIGHT][TOP] = 0;
  args.piece_boards[BLACK][KNIGHT][MIDDLE] = 0x4200000000000000;
  args.piece_boards[BLACK][KNIGHT][BOTTOM] = 0;

  args.piece_boards[WHITE][KING][TOP] = 0;
  args.piece_boards[WHITE][KING][MIDDLE] = 0x8;
  args.piece_boards[WHITE][KING][BOTTOM] = 0;

  args.piece_boards[BLACK][KING][TOP] = 0;
  args.piece_boards[BLACK][KING][MIDDLE] = 0x800000000000000;
  args.piece_boards[BLACK][KING][BOTTOM] = 0;


  args.hv_sliders[WHITE][TOP] = 0;
  args.hv_sliders[WHITE][MIDDLE] = 0x91;
  args.hv_sliders[WHITE][BOTTOM] = 0;

  args.hv_sliders[BLACK][TOP] = 0;
  args.hv_sliders[BLACK][MIDDLE] = 0x9100000000000000;
  args.hv_sliders[BLACK][BOTTOM] = 0;

  args.d_sliders[WHITE][TOP] = 0;
  args.d_sliders[WHITE][MIDDLE] = 0x34;
  args.d_sliders[WHITE][BOTTOM] = 0;

  args.d_sliders[BLACK][TOP] = 0;
  args.d_sliders[BLACK][MIDDLE] = 0x3400000000000000;
  args.d_sliders[BLACK][BOTTOM] = 0;

  args.k_pos[WHITE][0] = 3;
  args.k_pos[WHITE][1] = MIDDLE;
  args.k_pos[BLACK][0] = 59;
  args.k_pos[BLACK][1] = MIDDLE;
  MOVE best_move = search(&args, WHITE, 4);
  printf("score: %d\n", best_move.score);

  return 0;
}

void printf_bitboards(uint64_t *b) {
  for (int i = 0; i < 3; i++) {
    printf_bitboard(b[i]);
  }
}

void print_bitboards(uint64_t *b, FILE *fp) {
  for (int i = 0; i < 3; i++) {
    print_bitboard(b[i], fp);
  }
}


void print_bitboard(uint64_t b, FILE *fp) {
  fprintf(fp, "%ld:\n", b);
  for (int i = 63; i >= 0; i--) {
    if (b & (ONE << i)) {
      fprintf(fp, "1 ");
    } else {
      fprintf(fp, "0 ");
    }
    if (i % 8 == 0) {
      fprintf(fp, "\n");
    }
  }
  fprintf(fp, "\n");
}

void printf_bitboard(uint64_t b) {
  printf("%ld:\n", b);
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

void print_boards(BOARD_ARGS *args, FILE *fp) {
  char output[3][8][8] =
  {{{'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'}},
  {{'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'}},
  {{'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'},
  {'0','0','0','0','0','0','0','0'}}};

  // Color, type, level
  // 63 in top left, 0 in bottom right
  // output[level][x][y]
  /*
           ( y )
      0 1 2 3 4 5 6 7
      -----------------
   0  | 0 0 0 0 0 0 0 0
   1  | 0 0 0 0 0 0 0 0
   2  | 0 0 0 0 0 0 0 0
*  3  | 0 0 0 0 0 0 0 0
x  4  | 0 0 0 0 0 0 0 0
*  5  | 0 0 0 0 0 0 0 0
   6  | 0 0 0 0 0 0 0 0
   7  | 0 0 0 0 0 0 0 0
  */
  uint64_t x = 0;
  int m = 0;
  char out = ' ';
  // Color
  for (int i = 0; i < 2; i++) {
    // Type
    for (int j = 0; j < 6; j++) {
      // Level
      for (int k = 0; k < 3; k++) {
        while (m != 0) {
          x = args->piece_boards[i][j][k];
          m = log2_lookup(x);
          if (j == PAWN) {
            out = 'p';
          } else if (j == KNIGHT) {
            out = 'n';
          } else if (j == BISHOP) {
            out = 'b';
          } else if (j == ROOK) {
            out = 'r';
          } else if (j == QUEEN) {
            out = 'q';
          } else if (j == KING) {
            out = 'k';
          }
          if (i == WHITE) {
            out = toupper(out);
          }
          // Division = x, Modulo = y
          output[k][m / 8][m % 8] = out;
          out = ' ';
          x ^= (1 << m);
        }
      }
    }
  }
  for (int i = 0; i < 3; i++) {
    if (i == 0) {
      printf("Level One\n");
      fprintf(fp, "Level One\n");
    } else if (i == 1) {
      printf("Level Two\n");
      fprintf(fp, "Level Two\n");
    } else {
      printf("Level Three\n");
      fprintf(fp, "Level Three\n");
    }
    for (int j = 0; j < 8; j++) {
      for (int k = 0; k < 8; k++) {
        printf("%c", output[i][j][k]);
        fprintf(fp, "%c", output[i][j][k]);
      }
      printf("\n");
      fprintf(fp, "\n");
    }
  }
  fflush(stdout);
  fflush(fp);
}
