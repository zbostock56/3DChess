#define init_pieceboard(game) {\
  game.boards[WHITE][TOP] = 0;\
  game.boards[WHITE][MIDDLE] = 0xffff;\
  game.boards[WHITE][BOTTOM] = 0;\
  game.boards[BLACK][TOP] = 0;\
  game.boards[BLACK][MIDDLE] = 0xffff000000000000;\
  game.boards[BLACK][BOTTOM] = 0;\
  game.piece_boards[WHITE][BISHOP][TOP] = 0;\
  game.piece_boards[WHITE][BISHOP][MIDDLE] = 0x24;\
  game.piece_boards[WHITE][BISHOP][BOTTOM] = 0;\
  game.piece_boards[BLACK][BISHOP][TOP] = 0;\
  game.piece_boards[BLACK][BISHOP][MIDDLE] = 0x2400000000000000;\
  game.piece_boards[BLACK][BISHOP][BOTTOM] = 0;\
  game.piece_boards[WHITE][ROOK][TOP] = 0;\
  game.piece_boards[WHITE][ROOK][MIDDLE] = 0x81;\
  game.piece_boards[WHITE][ROOK][BOTTOM] = 0;\
  game.piece_boards[BLACK][ROOK][TOP] =0;\
  game.piece_boards[BLACK][ROOK][MIDDLE]= 0x8100000000000000;\
  game.piece_boards[BLACK][ROOK][BOTTOM]= 0;\
  game.piece_boards[WHITE][QUEEN][TOP] = 0;\
  game.piece_boards[WHITE][QUEEN][MIDDLE]= 0x10;\
  game.piece_boards[WHITE][QUEEN][BOTTOM]= 0;\
  game.piece_boards[BLACK][QUEEN][TOP] = 0;\
  game.piece_boards[BLACK][QUEEN][MIDDLE] = 0x1000000000000000;\
  game.piece_boards[BLACK][QUEEN][BOTTOM] = 0;\
  game.piece_boards[WHITE][PAWN][TOP] = 0;\
  game.piece_boards[WHITE][PAWN][MIDDLE]= 0xff00;\
  game.piece_boards[WHITE][PAWN][BOTTOM]= 0;\
  game.piece_boards[BLACK][PAWN][TOP] = 0;\
  game.piece_boards[BLACK][PAWN][MIDDLE] = 0xff000000000000;\
  game.piece_boards[BLACK][PAWN][BOTTOM]= 0;\
  game.piece_boards[WHITE][KNIGHT][TOP] = 0;\
  game.piece_boards[WHITE][KNIGHT][MIDDLE]= 0x42;\
  game.piece_boards[WHITE][KNIGHT][BOTTOM]= 0;\
  game.piece_boards[BLACK][KNIGHT][TOP] = 0;\
  game.piece_boards[BLACK][KNIGHT][MIDDLE]= 0x4200000000000000;\
  game.piece_boards[BLACK][KNIGHT][BOTTOM]= 0;\
  game.piece_boards[WHITE][KING][TOP] = 0;\
  game.piece_boards[WHITE][KING][MIDDLE]= 0x8;\
  game.piece_boards[WHITE][KING][BOTTOM]=0;\
  game.piece_boards[BLACK][KING][TOP] = 0;\
  game.piece_boards[BLACK][KING][MIDDLE]= 0x800000000000000;\
  game.piece_boards[BLACK][KING][BOTTOM]= 0;\
  game.hv_sliders[WHITE][TOP] = 0;\
  game.hv_sliders[WHITE][MIDDLE]= game.piece_boards[WHITE][ROOK][MIDDLE] |\
  game.piece_boards[WHITE][QUEEN][MIDDLE];\
  game.hv_sliders[WHITE][BOTTOM]= 0;\
  game.hv_sliders[BLACK][TOP] = 0;\
  game.hv_sliders[BLACK][MIDDLE]= game.piece_boards[BLACK][ROOK][MIDDLE] |\
  game.piece_boards[BLACK][QUEEN][MIDDLE];\
  game.hv_sliders[BLACK][BOTTOM]= 0;\
  game.d_sliders[WHITE][TOP] = 0;\
  game.d_sliders[WHITE][MIDDLE] = game.piece_boards[WHITE][BISHOP][MIDDLE] |\
  game.piece_boards[WHITE][QUEEN][MIDDLE];\
  game.d_sliders[WHITE][BOTTOM] = 0;\
  game.d_sliders[BLACK][TOP] = 0;\
  game.d_sliders[BLACK][MIDDLE]= game.piece_boards[BLACK][BISHOP][MIDDLE] |\
  game.piece_boards[BLACK][QUEEN][MIDDLE];\
  game.d_sliders[BLACK][BOTTOM]= 0;\
  game.k_pos[WHITE][MIDDLE] = 3;\
  game.k_pos[WHITE][TOP] = MIDDLE;\
  game.k_pos[BLACK][MIDDLE] = 59;\
  game.k_pos[BLACK][TOP] = MIDDLE;\
}
#if 0
// BLACK KING IN CHECK TEST POSITION
#define init_pieceboard(game) {\
  game.boards[WHITE][TOP] = 0x800000000080000;\
  game.boards[WHITE][MIDDLE] = 0xff6f;\
  game.boards[WHITE][BOTTOM] = 0;\
  game.boards[BLACK][TOP] = 0;\
  game.boards[BLACK][MIDDLE] = 0xffff000000000000;\
  game.boards[BLACK][BOTTOM] = 0;\
  game.piece_boards[WHITE][BISHOP][TOP] = 0;\
  game.piece_boards[WHITE][BISHOP][MIDDLE] = 0x24;\
  game.piece_boards[WHITE][BISHOP][BOTTOM] = 0;\
  game.piece_boards[BLACK][BISHOP][TOP] = 0;\
  game.piece_boards[BLACK][BISHOP][MIDDLE] = 0x2400000000000000;\
  game.piece_boards[BLACK][BISHOP][BOTTOM] = 0;\
  game.piece_boards[WHITE][ROOK][TOP] = 0x80000;\
  game.piece_boards[WHITE][ROOK][MIDDLE] = 0x1;\
  game.piece_boards[WHITE][ROOK][BOTTOM] = 0;\
  game.piece_boards[BLACK][ROOK][TOP] =0;\
  game.piece_boards[BLACK][ROOK][MIDDLE]= 0x8100000000000000;\
  game.piece_boards[BLACK][ROOK][BOTTOM]= 0;\
  game.piece_boards[WHITE][QUEEN][TOP] = 0x800000000000000;\
  game.piece_boards[WHITE][QUEEN][MIDDLE]= 0;\
  game.piece_boards[WHITE][QUEEN][BOTTOM]= 0;\
  game.piece_boards[BLACK][QUEEN][TOP] = 0;\
  game.piece_boards[BLACK][QUEEN][MIDDLE] = 0x1000000000000000;\
  game.piece_boards[BLACK][QUEEN][BOTTOM] = 0;\
  game.piece_boards[WHITE][PAWN][TOP] = 0;\
  game.piece_boards[WHITE][PAWN][MIDDLE]= 0xff00;\
  game.piece_boards[WHITE][PAWN][BOTTOM]= 0;\
  game.piece_boards[BLACK][PAWN][TOP] = 0;\
  game.piece_boards[BLACK][PAWN][MIDDLE] = 0xff000000000000;\
  game.piece_boards[BLACK][PAWN][BOTTOM]= 0;\
  game.piece_boards[WHITE][KNIGHT][TOP] = 0;\
  game.piece_boards[WHITE][KNIGHT][MIDDLE]= 0x42;\
  game.piece_boards[WHITE][KNIGHT][BOTTOM]= 0;\
  game.piece_boards[BLACK][KNIGHT][TOP] = 0;\
  game.piece_boards[BLACK][KNIGHT][MIDDLE]= 0x4200000000000000;\
  game.piece_boards[BLACK][KNIGHT][BOTTOM]= 0;\
  game.piece_boards[WHITE][KING][TOP] = 0;\
  game.piece_boards[WHITE][KING][MIDDLE]= 0x8;\
  game.piece_boards[WHITE][KING][BOTTOM]=0;\
  game.piece_boards[BLACK][KING][TOP] = 0;\
  game.piece_boards[BLACK][KING][MIDDLE]= 0x800000000000000;\
  game.piece_boards[BLACK][KING][BOTTOM]= 0;\
  game.hv_sliders[WHITE][BOTTOM] = game.piece_boards[WHITE][ROOK][BOTTOM] | game.piece_boards[WHITE][QUEEN][BOTTOM];\
  game.hv_sliders[WHITE][MIDDLE]= game.piece_boards[WHITE][ROOK][MIDDLE] | game.piece_boards[WHITE][QUEEN][MIDDLE];\
  game.hv_sliders[WHITE][TOP]= game.piece_boards[WHITE][ROOK][TOP] | game.piece_boards[WHITE][QUEEN][TOP];\
  game.hv_sliders[BLACK][BOTTOM] = game.piece_boards[BLACK][ROOK][BOTTOM] | game.piece_boards[BLACK][QUEEN][BOTTOM];\
  game.hv_sliders[BLACK][MIDDLE]= game.piece_boards[BLACK][ROOK][MIDDLE] | game.piece_boards[BLACK][QUEEN][MIDDLE];\
  game.hv_sliders[BLACK][MIDDLE]= game.piece_boards[BLACK][ROOK][TOP] | game.piece_boards[BLACK][QUEEN][TOP];\
  game.d_sliders[WHITE][BOTTOM] = game.piece_boards[WHITE][BISHOP][BOTTOM] | game.piece_boards[WHITE][QUEEN][BOTTOM];\
  game.d_sliders[WHITE][MIDDLE] = game.piece_boards[WHITE][BISHOP][MIDDLE] | game.piece_boards[WHITE][QUEEN][MIDDLE];\
  game.d_sliders[WHITE][TOP] = game.piece_boards[WHITE][BISHOP][TOP] | game.piece_boards[WHITE][QUEEN][TOP];\
  game.d_sliders[BLACK][BOTTOM] = game.piece_boards[BLACK][BISHOP][BOTTOM] | game.piece_boards[BLACK][QUEEN][BOTTOM];\
  game.d_sliders[BLACK][MIDDLE]= game.piece_boards[WHITE][BISHOP][MIDDLE] | game.piece_boards[WHITE][QUEEN][MIDDLE];\
  game.d_sliders[BLACK][TOP]= game.piece_boards[BLACK][BISHOP][TOP] | game.piece_boards[BLACK][QUEEN][TOP];\
  game.k_pos[WHITE][MIDDLE] = 3;\
  game.k_pos[WHITE][TOP] = MIDDLE;\
  game.k_pos[BLACK][MIDDLE] = 59;\
  game.k_pos[BLACK][TOP] = MIDDLE;\
}
#endif
#if 0
// Test knight/pawn check detection
#define init_pieceboard(game) {\
  game.boards[WHITE][TOP] = 0;\
  game.boards[WHITE][MIDDLE] = 0x4000000fffd;\
  game.boards[WHITE][BOTTOM] = 0;\
  game.boards[BLACK][TOP] = 0;\
  game.boards[BLACK][MIDDLE] = 0xfdf1000000000000;\
  game.boards[BLACK][BOTTOM] = 0;\
  game.piece_boards[WHITE][BISHOP][TOP] = 0;\
  game.piece_boards[WHITE][BISHOP][MIDDLE] = 0x24;\
  game.piece_boards[WHITE][BISHOP][BOTTOM] = 0;\
  game.piece_boards[BLACK][BISHOP][TOP] = 0;\
  game.piece_boards[BLACK][BISHOP][MIDDLE] = 0x2400000000000000;\
  game.piece_boards[BLACK][BISHOP][BOTTOM] = 0;\
  game.piece_boards[WHITE][ROOK][TOP] = 0;\
  game.piece_boards[WHITE][ROOK][MIDDLE] = 0x81;\
  game.piece_boards[WHITE][ROOK][BOTTOM] = 0;\
  game.piece_boards[BLACK][ROOK][TOP] =0;\
  game.piece_boards[BLACK][ROOK][MIDDLE]= 0x8100000000000000;\
  game.piece_boards[BLACK][ROOK][BOTTOM]= 0;\
  game.piece_boards[WHITE][QUEEN][TOP] = 0;\
  game.piece_boards[WHITE][QUEEN][MIDDLE]= 0x10;\
  game.piece_boards[WHITE][QUEEN][BOTTOM]= 0;\
  game.piece_boards[BLACK][QUEEN][TOP] = 0;\
  game.piece_boards[BLACK][QUEEN][MIDDLE] = 0;\
  game.piece_boards[BLACK][QUEEN][BOTTOM] = 0;\
  game.piece_boards[WHITE][PAWN][TOP] = 0;\
  game.piece_boards[WHITE][PAWN][MIDDLE]= 0xff00;\
  game.piece_boards[WHITE][PAWN][BOTTOM]= 0;\
  game.piece_boards[BLACK][PAWN][TOP] = 0;\
  game.piece_boards[BLACK][PAWN][MIDDLE] = 0x00f1000000000000;\
  game.piece_boards[BLACK][PAWN][BOTTOM]= 0;\
  game.piece_boards[WHITE][KNIGHT][TOP] = 0;\
  game.piece_boards[WHITE][KNIGHT][MIDDLE]= 0x40000000040;\
  game.piece_boards[WHITE][KNIGHT][BOTTOM]= 0;\
  game.piece_boards[BLACK][KNIGHT][TOP] = 0;\
  game.piece_boards[BLACK][KNIGHT][MIDDLE]= 0x4000000000000000;\
  game.piece_boards[BLACK][KNIGHT][BOTTOM]= 0;\
  game.piece_boards[WHITE][KING][TOP] = 0;\
  game.piece_boards[WHITE][KING][MIDDLE]= 0x8;\
  game.piece_boards[WHITE][KING][BOTTOM]=0;\
  game.piece_boards[BLACK][KING][TOP] = 0;\
  game.piece_boards[BLACK][KING][MIDDLE]= 0x800000000000000;\
  game.piece_boards[BLACK][KING][BOTTOM]= 0;\
  game.hv_sliders[WHITE][BOTTOM] = game.piece_boards[WHITE][ROOK][BOTTOM] | game.piece_boards[WHITE][QUEEN][BOTTOM];\
  game.hv_sliders[WHITE][MIDDLE]= game.piece_boards[WHITE][ROOK][MIDDLE] | game.piece_boards[WHITE][QUEEN][MIDDLE];\
  game.hv_sliders[WHITE][TOP]= game.piece_boards[WHITE][ROOK][TOP] | game.piece_boards[WHITE][QUEEN][TOP];\
  game.hv_sliders[BLACK][BOTTOM] = game.piece_boards[BLACK][ROOK][BOTTOM] | game.piece_boards[BLACK][QUEEN][BOTTOM];\
  game.hv_sliders[BLACK][MIDDLE]= game.piece_boards[BLACK][ROOK][MIDDLE] | game.piece_boards[BLACK][QUEEN][MIDDLE];\
  game.hv_sliders[BLACK][MIDDLE]= game.piece_boards[BLACK][ROOK][TOP] | game.piece_boards[BLACK][QUEEN][TOP];\
  game.d_sliders[WHITE][BOTTOM] = game.piece_boards[WHITE][BISHOP][BOTTOM] | game.piece_boards[WHITE][QUEEN][BOTTOM];\
  game.d_sliders[WHITE][MIDDLE] = game.piece_boards[WHITE][BISHOP][MIDDLE] | game.piece_boards[WHITE][QUEEN][MIDDLE];\
  game.d_sliders[WHITE][TOP] = game.piece_boards[WHITE][BISHOP][TOP] | game.piece_boards[WHITE][QUEEN][TOP];\
  game.d_sliders[BLACK][BOTTOM] = game.piece_boards[BLACK][BISHOP][BOTTOM] | game.piece_boards[BLACK][QUEEN][BOTTOM];\
  game.d_sliders[BLACK][MIDDLE]= game.piece_boards[WHITE][BISHOP][MIDDLE] | game.piece_boards[WHITE][QUEEN][MIDDLE];\
  game.d_sliders[BLACK][TOP]= game.piece_boards[BLACK][BISHOP][TOP] | game.piece_boards[BLACK][QUEEN][TOP];\
  game.k_pos[WHITE][MIDDLE] = 3;\
  game.k_pos[WHITE][TOP] = MIDDLE;\
  game.k_pos[BLACK][MIDDLE] = 59;\
  game.k_pos[BLACK][TOP] = MIDDLE;\
}
#endif

#include <glad/glad.h>
#include <glfw3.h>
#include <string.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/quat.h>
#include <entity_str.h>
#include <pthread.h>

#define DIR "/home/zbostock/Projects/3DChess"

extern unsigned long long stuff;
MOVE search(BOARD_ARGS *args, SIDE to_move, unsigned int depth, int alpha,
            int beta, unsigned int turn);
void make_move(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
                     unsigned int *from, unsigned int *to);
MOVE level_zero_search(BOARD_ARGS *args, unsigned int turn, SIDE to_move,
                       unsigned int depth, int alpha, int beta);
void play();
// Graphics Functions

void mouse_input(GLFWwindow *widnow, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow *, int, int);
void keyboard_input(GLFWwindow *window);

// Helper Functions

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
  {{{'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'}},
  {{'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'}},
  {{'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'},
  {'-','-','-','-','-','-','-','-'}}};
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
    if (i == 0) {
      printf("\nTOP\n\n");
    } else if (i == 1) {
      printf("\nMIDDLE\n\n");
    } else {
      printf("\nBOTTOM\n\n");
    }
    for (int j = 7; j > -1; j--) {
      printf("%d | ", j + 1);
      for (int k = 7; k > -1; k--) {
        printf("%c", output[i][j][k]);
        printf(" ");
      }
      printf("\n");
    }
   printf("----------------------\n    A B C D E F G H\n");
  }
  printf("\n");
  fflush(stdout);
}


