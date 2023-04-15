#include <psuedo_legal_moves.h>
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
  return 0;
}
