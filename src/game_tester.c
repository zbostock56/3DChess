#include <game_tester.h>

int main() {
  FILE *fp = fopen("DEGUB.txt", "w");
  fprintf(fp, "**************DEGUB OUTPUT***************\n");

  BOARD_ARGS args;
  args.boards[WHITE][TOP] = 0;
  args.boards[WHITE][MIDDLE] = 0x10001000;
  args.boards[WHITE][BOTTOM] = 0;
  
  args.boards[BLACK][TOP] = 0;
  args.boards[BLACK][MIDDLE] = 0x1002000000000000;
  args.boards[BLACK][BOTTOM] = 0;

  args.piece_boards[WHITE][BISHOP][TOP] = 0;
  args.piece_boards[WHITE][BISHOP][MIDDLE] =
  args.piece_boards[WHITE][BISHOP][BOTTOM] =0;  

  args.piece_boards[BLACK][BISHOP][TOP] =0;  
  args.piece_boards[BLACK][BISHOP][MIDDLE] =0;
  args.piece_boards[BLACK][BISHOP][BOTTOM] =0;  

  args.piece_boards[WHITE][ROOK][TOP] =0;  
  args.piece_boards[WHITE][ROOK][MIDDLE] =0x10000000;
  args.piece_boards[WHITE][ROOK][BOTTOM] =0;  

  args.piece_boards[BLACK][ROOK][TOP] =0;  
  args.piece_boards[BLACK][ROOK][MIDDLE] =0x1000000000000000;
  args.piece_boards[BLACK][ROOK][BOTTOM] =0;  

  args.piece_boards[WHITE][QUEEN][TOP] =0;  
  args.piece_boards[WHITE][QUEEN][MIDDLE] =0;
  args.piece_boards[WHITE][QUEEN][BOTTOM] =0;  

  args.piece_boards[BLACK][QUEEN][TOP] =0;  
  args.piece_boards[BLACK][QUEEN][MIDDLE] =0;
  args.piece_boards[BLACK][QUEEN][BOTTOM] =0;  

  args.piece_boards[WHITE][PAWN][TOP] =0;  
  args.piece_boards[WHITE][PAWN][MIDDLE] =0;
  args.piece_boards[WHITE][PAWN][BOTTOM] =0;  

  args.piece_boards[BLACK][PAWN][TOP] =0;  
  args.piece_boards[BLACK][PAWN][MIDDLE] =0;
  args.piece_boards[BLACK][PAWN][BOTTOM] =0;  

  args.piece_boards[WHITE][KNIGHT][TOP] =0;  
  args.piece_boards[WHITE][KNIGHT][MIDDLE] =0;
  args.piece_boards[WHITE][KNIGHT][BOTTOM] =0;  

  args.piece_boards[BLACK][KNIGHT][TOP] =0;  
  args.piece_boards[BLACK][KNIGHT][MIDDLE] =0;
  args.piece_boards[BLACK][KNIGHT][BOTTOM] =0;  

  args.piece_boards[WHITE][KING][TOP] =0;  
  args.piece_boards[WHITE][KING][MIDDLE] =0x1000;
  args.piece_boards[WHITE][KING][BOTTOM] =0;  

  args.piece_boards[BLACK][KING][TOP] =0;  
  args.piece_boards[BLACK][KING][MIDDLE] =0x2000000000000;
  args.piece_boards[BLACK][KING][BOTTOM] =0;


  args.hv_sliders[WHITE][TOP] =0;
  args.hv_sliders[WHITE][MIDDLE] =0x10000000;
  args.hv_sliders[WHITE][BOTTOM] =0;

  args.hv_sliders[BLACK][TOP] =0;
  args.hv_sliders[BLACK][MIDDLE] =0x1000000000000000;
  args.hv_sliders[BLACK][BOTTOM] =0;

  args.d_sliders[WHITE][TOP] =0;
  args.d_sliders[WHITE][MIDDLE] =0;
  args.d_sliders[WHITE][BOTTOM] =0;

  args.d_sliders[BLACK][TOP] =0;
  args.d_sliders[BLACK][MIDDLE] =0;
  args.d_sliders[BLACK][BOTTOM] =0;

  args.k_pos[WHITE][0] = 12;
  args.k_pos[WHITE][1] = MIDDLE;
  args.k_pos[BLACK][0] = 49;
  args.k_pos[BLACK][1] = MIDDLE;

  fprintf(fp, "White:\n");
  print_bitboard(args.boards[WHITE][MIDDLE], fp);
  fprintf(fp, "Black:\n");
  print_bitboard(args.boards[BLACK][MIDDLE], fp);

  fprintf(fp, "W BISHOP:\n");
  print_bitboard(args.piece_boards[WHITE][BISHOP][MIDDLE], fp);
  fprintf(fp, "B BISHOP:\n");
  print_bitboard(args.piece_boards[BLACK][BISHOP][MIDDLE], fp);

  fprintf(fp, "W ROOK:\n");
  print_bitboard(args.piece_boards[WHITE][ROOK][MIDDLE], fp);
  fprintf(fp, "B ROOK:\n");
  print_bitboard(args.piece_boards[BLACK][ROOK][MIDDLE], fp);

  fprintf(fp, "W QUEEN:\n");
  print_bitboard(args.piece_boards[WHITE][QUEEN][MIDDLE], fp);
  fprintf(fp, "B QUEEN:\n");
  print_bitboard(args.piece_boards[BLACK][QUEEN][MIDDLE], fp);

  fprintf(fp, "W PAWN:\n");
  print_bitboard(args.piece_boards[WHITE][PAWN][MIDDLE], fp);
  fprintf(fp, "B PAWN:\n");
  print_bitboard(args.piece_boards[BLACK][PAWN][MIDDLE], fp);

  fprintf(fp, "W KNIGHT:\n");
  print_bitboard(args.piece_boards[WHITE][KNIGHT][MIDDLE], fp);
  fprintf(fp, "B KNIGHT:\n");
  print_bitboard(args.piece_boards[BLACK][KNIGHT][MIDDLE], fp);
  
  fprintf(fp, "W KING:\n");
  print_bitboard(args.piece_boards[WHITE][KING][MIDDLE], fp);
  fprintf(fp, "B KING:\n");
  print_bitboard(args.piece_boards[BLACK][KING][MIDDLE], fp);

  fprintf(fp, "W K POS: %d %d\n", args.k_pos[WHITE][0], args.k_pos[WHITE][1]);
  fprintf(fp, "B K POS: %d %d\n", args.k_pos[BLACK][0], args.k_pos[BLACK][1]);

  unsigned int pos[2] = { 28, MIDDLE };
  uint64_t output[3];
  get_legal(WHITE, pos, ROOK, args, output);

  print_bitboard(output[0], fp);
  print_bitboard(output[1], fp);
  print_bitboard(output[2], fp);

  return 0;
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
