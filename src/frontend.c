#include <psuedo_legal_moves.h>

MOVE search(BOARD_ARGS *args, SIDE to_move, unsigned int depth);
BOARD_ARGS make_move(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
                     unsigned int *from, unsigned int *to);
int turn = 1;
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
  fprintf(fp, "%llu:\n", b);
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
  printf("%llu:\n", b);
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


int main() {
  BOARD_ARGS game;
  game.boards[WHITE][0] = 0;
  game.boards[WHITE][1] = 0xffff;
  game.boards[WHITE][2] = 0;

  game.boards[BLACK][0] = 0;
  game.boards[BLACK][1] = 0xffff000000000000;
  game.boards[BLACK][2] = 0;

  game.piece_boards[WHITE][BISHOP][0] = 0;
  game.piece_boards[WHITE][BISHOP][1] = 0x42;
  game.piece_boards[WHITE][BISHOP][2] = 0;

  game.piece_boards[BLACK][BISHOP][0] = 0;
  game.piece_boards[BLACK][BISHOP][1] = 0x4200000000000000;
  game.piece_boards[BLACK][BISHOP][2] = 0;

  game.piece_boards[WHITE][ROOK][0] = 0;
  game.piece_boards[WHITE][ROOK][1] = 0x81;
  game.piece_boards[WHITE][ROOK][2] = 0;

  game.piece_boards[BLACK][ROOK][0] =0;
  game.piece_boards[BLACK][ROOK][1]= 0x8100000000000000;
  game.piece_boards[BLACK][ROOK][2]= 0;

  game.piece_boards[WHITE][QUEEN][0] = 0;
  game.piece_boards[WHITE][QUEEN][1]= 0x10;
  game.piece_boards[WHITE][QUEEN][2]= 0;

  game.piece_boards[BLACK][QUEEN][0] = 0;
  game.piece_boards[BLACK][QUEEN][1] = 0x1000000000000000;
  game.piece_boards[BLACK][QUEEN][2] = 0;

  game.piece_boards[WHITE][PAWN][0] = 0;
  game.piece_boards[WHITE][PAWN][1]= 0xff00;
  game.piece_boards[WHITE][PAWN][2]= 0;

  game.piece_boards[BLACK][PAWN][0] = 0;
  game.piece_boards[BLACK][PAWN][1] = 0xff000000000000;
  game.piece_boards[BLACK][PAWN][2]= 0;

  game.piece_boards[WHITE][KNIGHT][0] = 0;
  game.piece_boards[WHITE][KNIGHT][1]= 0x24;
  game.piece_boards[WHITE][KNIGHT][2]= 0;

  game.piece_boards[BLACK][KNIGHT][0] = 0;
  game.piece_boards[BLACK][KNIGHT][1]= 0x2400000000000000;
  game.piece_boards[BLACK][KNIGHT][2]= 0;

  game.piece_boards[WHITE][KING][0] = 0;
  game.piece_boards[WHITE][KING][1]= 0x8;
  game.piece_boards[WHITE][KING][2]=0;

  game.piece_boards[BLACK][KING][0] = 0;
  game.piece_boards[BLACK][KING][1]= 0x800000000000000;
  game.piece_boards[BLACK][KING][2]= 0;

  game.hv_sliders[WHITE][0] = 0;
  game.hv_sliders[WHITE][1]= game.piece_boards[WHITE][ROOK][MIDDLE] |
    game.piece_boards[WHITE][QUEEN][MIDDLE];
  game.hv_sliders[WHITE][2]= 0;

  game.hv_sliders[BLACK][0] = 0;
  game.hv_sliders[BLACK][1]= game.piece_boards[BLACK][ROOK][MIDDLE] |
    game.piece_boards[BLACK][QUEEN][MIDDLE];
  game.hv_sliders[BLACK][2]= 0;

  game.d_sliders[WHITE][0] = 0;
  game.d_sliders[WHITE][1] = game.piece_boards[WHITE][BISHOP][MIDDLE] |
    game.piece_boards[WHITE][QUEEN][MIDDLE];
  game.d_sliders[WHITE][2] = 0;

  game.d_sliders[BLACK][0] = 0;
  game.d_sliders[BLACK][1]= game.piece_boards[BLACK][BISHOP][MIDDLE] |
    game.piece_boards[BLACK][QUEEN][MIDDLE];
  game.d_sliders[BLACK][2]= 0;
  game.k_pos[WHITE][0] = 3;
  game.k_pos[WHITE][1] = MIDDLE;
  game.k_pos[BLACK][0] = 59;
  game.k_pos[BLACK][1] = MIDDLE;

  int piece = 0;
  unsigned int from[2];
  unsigned int to[2];
  while (1) {
    if (turn == 1) {
      printf_bitboard(game.boards[BLACK][0]);
      printf_bitboard(game.boards[BLACK][1]);
      printf_bitboard(game.boards[BLACK][2]);
      printf_bitboard(game.boards[WHITE][0]);
      printf_bitboard(game.boards[WHITE][1]);
      printf_bitboard(game.boards[WHITE][2]);
      TYPE type;
      printf("Enter piece:\n0: BISHOP\n1: ROOK\n2: QUEEN\n3: PAWN\n4: KNIGHT\nAny: KING\n");
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
      } else {
        type = KING;
      }

      printf("Enter from level (0 - 2)\n");
      scanf("%d", from + 1);
      printf("Enter from position (0 - 63)\n");
      scanf("%d", from);
      if (from[0] > 63) {
        from[0] = 63;
      } else if (from[0] < 0) {
        from[0] = 0;
      }
      if (from[1] < 0) {
        from[1] = 0;
      } else if (from[1] > 2) {
        from[1] = 2;
      }

      printf("Enter to level (0 - 2)\n");
      scanf("%d", to + 1);
      printf("Enter to position (0 - 63)\n");
      scanf("%d", to);
      if (to[0] > 63) {
        to[0] = 63;
      } else if (to[0] < 0) {
        to[0] = 0;
      }
      if (to[1] < 0) {
        to[1] = 0;
      } else if (to[1] > 2) {
        to[1] = 2;
      }


      game = make_move(&game, WHITE, type, from, to);
      turn = 0;
    } else {
      printf_bitboard(game.boards[WHITE][0]);
      printf_bitboard(game.boards[WHITE][1]);
      printf_bitboard(game.boards[WHITE][2]);
      printf_bitboard(game.boards[BLACK][0]);
      printf_bitboard(game.boards[BLACK][1]);
      printf_bitboard(game.boards[BLACK][2]);
      MOVE com_move = search(&game, BLACK, 3);
      unsigned int *to = com_move.to;
      unsigned int *from = com_move.from;
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
      game = make_move(&game, BLACK, type, from, to);
      turn = 1;
    }
  }
}
