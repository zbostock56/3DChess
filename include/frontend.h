#include <glad/glad.h>
#include <glfw3.h>
#include <string.h>
#include <cglm/cglm.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/quat.h>
#include <entity_str.h>
#include <pthread.h>

#define DIR "/home/zbostock/Projects/3DChess"

// HELPERS

void set_game_to_graphics(unsigned int *from, unsigned int *to);
void printf_bitboards(uint64_t *b);
void print_bitboards(uint64_t *b, FILE *fp);
void print_bitboard(uint64_t b, FILE *fp);
void printf_bitboard(uint64_t b);
void print_boards(BOARD_ARGS game);
void print_game(BOARD_ARGS *args);
int switch_letters(char letter);
void play();
void output_to_file(BOARD_ARGS *args, SIDE to_move);

extern unsigned long long stuff;
MOVE search(BOARD_ARGS *args, SIDE to_move, unsigned int depth, int alpha,
  int beta, unsigned int turn);
void make_move(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
  unsigned int *from, unsigned int *to);
MOVE level_zero_search(BOARD_ARGS *args, unsigned int turn, SIDE to_move,
  unsigned int depth, int alpha, int beta);
void play();
void set_game_to_graphics(unsigned int *from, unsigned int *to);

// Graphics Functions

void mouse_input(GLFWwindow *widnow, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow *, int, int);
void keyboard_input(GLFWwindow *window);

// BOARD POSITIONS

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


