#include <psuedo_legal_moves.h>
#include <frontend.h>
MOVE search(BOARD_ARGS *args, SIDE to_move, unsigned int depth);
void make_move(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
                     unsigned int *from, unsigned int *to);
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

int main() {
  /*
    LOAD INITIAL BOARD
  */

  BOARD_ARGS game;
  init_pieceboard(game);
  int piece = 0;
  unsigned int from[2] = {0,0};
  unsigned int to[2] = {0,0};
  int turn = 0;
  while (1) {
     if (turn == 0) {
      print_boards(game);
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
      to[1]--;
      from[1]--;
      printf("from[0] %u\nfrom[1] %u\nto[0] %u\nto[1] %u\n", from[0], from[1], to[0], to[1]);
      make_move(&game, WHITE, type, from, to);
      turn = 1;
    } else {
      MOVE com_move = search(&game, BLACK, 4);
      unsigned int *to = com_move.to;
      unsigned int *from = com_move.from;
      unsigned int temp = to[0];
      to[0] = to[1];
      to[1] = temp;
      temp = from[0];
      from[0] = from[1];
      from[1] = temp;
      printf("\n\n\nBLACK'S MOVE\nfrom[0] %u\nfrom[1] %u\nto[0] %u\nto[1] %u\n", from[0], from[1], to[0], to[1]);
      uint64_t cur = (ONE << from[0]);
      TYPE type;
      if (game.piece_boards[BLACK][BISHOP][from[1]] & cur) {
        type = BISHOP;
      } else if (game.piece_boards[BLACK][ROOK][from[1]] & cur) {
        type = ROOK;
      } else if (game.piece_boards[BLACK][QUEEN][from[1]] & cur) {
        type = QUEEN;
      } else if (game.piece_boards[BLACK][PAWN][from[1]] & cur) {
        type = PAWN;
      } else if (game.piece_boards[BLACK][KNIGHT][from[1]] & cur) {
        type = KNIGHT;
      } else {
        type = KING;
      }
      make_move(&game, BLACK, type, from, to);
      turn = 0;
    }
  }
}
