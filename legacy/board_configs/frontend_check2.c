#include <psuedo_legal_moves.h>
#include <frontend_check2.h>
extern unsigned long long stuff;
MOVE search(BOARD_ARGS *args, SIDE to_move, unsigned int depth, int alpha,
            int beta, unsigned int turn);
void make_move(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
                     unsigned int *from, unsigned int *to);
MOVE level_zero_search(BOARD_ARGS *args, unsigned int turn, SIDE to_move,
                       unsigned int depth, int alpha, int beta);
void printf_bitboards(uint64_t *b) {
  for (int i = 0; i < 3; i++) {
    printf("%s\n", i == 0 ? "TOP" : i == 1 ? "MIDDLE" : "BOTTOM");
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

void print_boards(BOARD_ARGS game) {
   if (game.boards[BLACK][0]) {
     printf("-------------BLACK TOP-----------------\n");
     printf_bitboard(game.boards[BLACK][0]);
     printf("---------------------------------------\n");
   }
   if (game.boards[BLACK][1]) {
     printf("-------------BLACK MIDDLE--------------\n");
     printf_bitboard(game.boards[BLACK][1]);
     printf("---------------------------------------\n");
   }
   if (game.boards[BLACK][2]) {
     printf("-------------BLACK BOTTOM--------------\n");
     printf_bitboard(game.boards[BLACK][2]);
     printf("---------------------------------------\n");
   }
   if (game.boards[WHITE][0]) {
     printf("-------------WHITE TOP-----------------\n");
     printf_bitboard(game.boards[WHITE][0]);
     printf("---------------------------------------\n");
   }
   if (game.boards[WHITE][1]) {
     printf("-------------WHITE MIDDLE--------------\n");
     printf_bitboard(game.boards[WHITE][1]);
     printf("---------------------------------------\n");
   }
   if (game.boards[WHITE][2]) {
     printf("-------------WHITE BOTTOM--------------\n");
     printf_bitboard(game.boards[WHITE][2]);
     printf("---------------------------------------\n");
   }
}

void print_game(BOARD_ARGS *args) {
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
  uint64_t x = 0;
  int m = -1;
  char out = ' ';
  // Color
  for (int i = 0; i < 2; i++) {
    // Type
    for (int j = 0; j < 6; j++) {
      // Level
      for (int k = 0; k < 3; k++) {
        x = args->piece_boards[i][j][k];
        m = log2_lookup(x);
        while (x != 0) {
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
          x ^= (ONE << m);
          m = log2_lookup(x);
        }
      }
    }
  }
  for (int i = 0; i < 3; i++) {
    if (i == 1) {
      printf("\nTOP\n\n");
    } else if (i == 0) {
      printf("\nMIDDLE\n\n");
    } else {
      printf("\nBOTTOM\n\n");
    }
    for (int j = 7; j > -1; j--) {
      for (int k = 7; k > -1; k--) {
        printf("%c", output[i][j][k]);
        printf(" ");
      }
      printf("\n");
    }
  }
  printf("\n");
  fflush(stdout);
}

int main() {
  /*
    LOAD INITIAL BOARD
  */
 /*
 unsigned int moves[6][2] = {
    {1, 12},
    {1, 20},
    {1, 13},
    {1, 21},
    {1, 14},
    {1, 22},
  };
  */
  //int game_score = 0;
  BOARD_ARGS game;
  init_pieceboard(game);
  int piece = 0;
  unsigned int from[2] = {0,0};
  //unsigned int from[2] = {1,12};
  unsigned int to[2] = {0,0};
  //unsigned int to[2] = {1,20};
  int to_move = 1;
  unsigned int turn = 1;
  while (1) {
     if (to_move == 0) {
      print_game(&game);
      TYPE type;
      printf("Enter piece:\n0: BISHOP\n1: ROOK\n2: QUEEN\n3: PAWN\n4: KNIGHT\n5: KING\nQuit (Any Other)\n");
      scanf("%d", &piece);
      if (piece == 0) {
        type = BISHOP;
      } else if (piece == 1) {
        type = ROOK;
      } else if (piece == 2) {
        type = QUEEN;
      } else if (piece == 3) {
        type = PAWN;
      } else if (piece == 4) {
        type = KNIGHT;
      } else if (piece == 5) {
        type = KING;
      } else {
        return 0;
      }
      printf("Enter from level (0 - 2) and position (1 - 64)\n");
      scanf("%u %u", &from[0], &from[1]);
      if (from[1] > 63) {
        from[1] = 63;
      } else if (from[1] < 0) {
        from[1] = 0;
      }
      if (from[0] < 0) {
        from[0] = 0;
      } else if (from[0] > 2) {
        from[0] = 2;
      }

      printf("Enter to level (0 - 2) and position (1 - 64)\n");
      scanf("%u %u", &to[0], &to[1]);
      if (to[1] > 63) {
        to[1] = 63;
      } else if (to[1] < 0) {
        to[1] = 0;
      }
      if (to[0] < 0) {
        to[0] = 0;
      } else if (to[0] > 2) {
        to[0] = 2;
      }
      printf("from[0] %u\nfrom[1] %u\nto[0] %u\nto[1] %u\n", from[0], from[1], to[0], to[1]);
      make_move(&game, WHITE, type, from, to);
      to_move = 1;
    } else {
      /*
        To/From: {Level, Bitposition}
      */
      MOVE com_move = level_zero_search(&game, turn, BLACK, 3, INT_MIN, INT_MAX);
      //MOVE com_move = search(&game, BLACK, 4);
      //MOVE com_move = search(&game, BLACK, 2, INT_MIN, INT_MAX, turn);
      unsigned int *to = com_move.to;
      unsigned int *from = com_move.from;
      printf("BLACK'S MOVE\nfrom[0] %u\nfrom[1] %u\nto[0] %u\nto[1] %u\n", from[0], from[1], to[0], to[1]);
      printf("COMPUTER EVAL: %d\n", com_move.score);
      uint64_t cur = (ONE << from[1]);
      TYPE type;
      if (game.piece_boards[BLACK][BISHOP][from[0]] & cur) {
        type = BISHOP;
      } else if (game.piece_boards[BLACK][ROOK][from[0]] & cur) {
        type = ROOK;
      } else if (game.piece_boards[BLACK][QUEEN][from[0]] & cur) {
        type = QUEEN;
      } else if (game.piece_boards[BLACK][PAWN][from[0]] & cur) {
        type = PAWN;
      } else if (game.piece_boards[BLACK][KNIGHT][from[0]] & cur) {
        type = KNIGHT;
      } else {
        type = KING;
      }
      make_move(&game, BLACK, type, from, to);
      to_move = 0;
      printf("Positions Searched: %llu\n", stuff);
      printf("Rating: %u\n", com_move.rating);
      print_game(&game);
      return 0;
      turn++;
    }
  }
}
