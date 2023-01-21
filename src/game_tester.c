#define <game_tester.h>
#define <legal_moves.h>
#ifndef
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <log.h>
#include <ctype.h>
#endif
int main() {
  FILE *fp = fopen("DEGUB.txt", "w");
  fprintf(fp, "**************DEGUB OUTPUT***************\n");
  return 0;
}

void print_boards(board_args *boards, FILE *fp) {
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
  char out = '';
  // Color
  for (int i = 0; i < 2; i++) {
    // Type
    for (int j = 0; j < 6; j++) {
      // Level
      for (int k = 0; k < 3; k++) {
        while (m != 0) {
          x = piece_boards[i][j][k];
          m = log(x);
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
            out = toUpper(out);
          }
          // Division = x, Modulo = y
          output[k][m / 8][m % 8] = out;
          out = '';
          x =^ (1 << m);
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
  fflush();
}
