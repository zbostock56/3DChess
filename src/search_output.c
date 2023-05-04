#include <search_output.h>

typedef struct node {
  int *position;
  unsigned int level_from;
  unsigned int bitposition_from;
  unsigned int level_to;
  unsigned int bitposition_to;
  unsigned int result;
  MOVE move;
} node_t;

node_t chosen_moves[1000000];
node_t possible_moves[5000];
int possible_moves_i = 0;
int piece_positions[192];
int chosen_moves_i = 0;

void search_output(BOARD_ARGS *args, SIDE to_move) {
  SIDE enemy = to_move == WHITE ? BLACK : WHITE;
  uint32_t player_flags = 0;
  uint32_t enemy_flags = 0;
  MOVE move;
  move.args = args;
  move.to_move = to_move;
  move.to[0] = 0;
  move.to[1] = 0;
  move.from[0] = 0;
  move.from[1] = 0;;
  move.rating = 0;
  unsigned int current_position[2];
  unsigned int to_position[2];
  uint64_t junk[3];
  get_legal(enemy, args->k_pos[enemy], KING, *args, junk, &enemy_flags);
  if (enemy_flags & CHECK) {
    //printf("\n\nCURRENT GAME CONFIG\n\n");
    //print_game(args);
    get_legal(enemy, args->k_pos[enemy], KING, *args, junk, &enemy_flags);
  }
  get_legal(to_move, args->k_pos[to_move], KING, *args, junk, &player_flags);
  if (player_flags & CHECK) {
    //printf("\n\nCURRENT GAME CONFIG\n\n");
    //print_game(args);
    get_legal(to_move, args->k_pos[to_move], KING, *args, junk, &player_flags);
  }
  if ((enemy_flags & MATE) == 1) {
    possible_moves_i = 0;
    possible_moves[possible_moves_i].result = 1;
    possible_moves[possible_moves_i].level_from = 0;
    possible_moves[possible_moves_i].level_to = 0;
    possible_moves[possible_moves_i].bitposition_from = 0;
    possible_moves[possible_moves_i++].bitposition_to = 0;
    return;
  } else if ((player_flags & MATE) == 1) {
    possible_moves_i = 0;
    possible_moves[possible_moves_i].result = 0;
    possible_moves[possible_moves_i].level_from = 0;
    possible_moves[possible_moves_i].level_to = 0;
    possible_moves[possible_moves_i].bitposition_from = 0;
    possible_moves[possible_moves_i++].bitposition_to = 0;
    return;
  }
  for (int i = 0; i < PIECE_TYPES; i++) {
    // PIECE TYPE
    for (int j = 0; j < NUM_LEVELS; j++) {
      // LEVEL
      current_position[0] = j;
      uint64_t t_piece_board = args->piece_boards[to_move][i][j];
      while (t_piece_board) {
        unsigned int ms_bit[2] = {j, ms_bit_lookup(&t_piece_board)};
        current_position[1] = ms_bit[1];
        move.from[0] = current_position[0];
        move.from[1] = current_position[1];
        uint64_t legals[3];
        get_legal(to_move, ms_bit, i, *args, legals, &player_flags);
        for (int k = 0; k < NUM_LEVELS; k++) {
          to_position[0] = k;
          uint64_t t_legal_board = legals[k];
          while (t_legal_board) {
            unsigned int ms_bit_poss_legal[2] = {k, ms_bit_lookup(&t_legal_board)};
            to_position[1] = ms_bit_poss_legal[1];
            move.to[0] = to_position[0];
            move.to[1] = to_position[1];
            possible_moves[possible_moves_i].result = -1;
            possible_moves[possible_moves_i].level_from = move.from[0];
            possible_moves[possible_moves_i].level_to = move.to[0];
            possible_moves[possible_moves_i].bitposition_from = move.from[1];
            possible_moves[possible_moves_i].bitposition_to = move.to[1];
            possible_moves[possible_moves_i].move = move;
            translate_position(move.args, possible_moves_i++);
          }
        }
      }
    }
  }
}

/*
  1: BISHOP
  2: ROOK
  3: QUEEN
  4: PAWN
  5: KNIGHT
  6: KING
*/

void translate_position(BOARD_ARGS *args, int arr_pos) {
  /* MAKE SURE TO FREE */
  int *p = (int *) calloc(192, sizeof(int));
  uint64_t x = 0;
  int m = -1;
  int val = 0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 6; j++) {
      for (int k = 0; k < 3; k++) {
        x = args->piece_boards[i][j][k];
        m = log2_lookup(x);
        while (x != 0) {
          if (j == PAWN) {
            val = 4;
          } else if (j == BISHOP) {
            val = 1;
          } else if (j == ROOK) {
            val = 2;
          } else if (j == QUEEN) {
            val = 3;
          } else if (j == KING) {
            val = 6;
          } else if (j == KNIGHT) {
            val = 5;
          }
          p[(k * 64) + m] = val;
          x ^= (ONE << m);
          m = log2_lookup(x);
        }
      }
    }
  }
  possible_moves[arr_pos].position = p;
}

void reset_possible_moves() {
  for (int i = 0; i < possible_moves_i; i++) {
    //if (possible_moves[i].position != NULL)
    //  free(possible_moves[i].position);
    possible_moves[i].result = 0;
    possible_moves[i].level_from = 0;
    possible_moves[i].level_to = 0;
    possible_moves[i].bitposition_to = 0;
    possible_moves[i].bitposition_from = 0;
  }
  possible_moves_i = 0;
}

void output_to_file(BOARD_ARGS *args, SIDE to_move) {
  srand(random_val_gen());
  FILE *fp = fopen("output.csv", "a+");
  int turn_number = 0;
  int turn = 0;
  while (1) {
    if (turn_number == 500) {
      fprintf(stderr, "HIT MAX MOVE NUMBER... EXITING...\n");
     // fprintf(stderr, "random_val_gen(): %u\n", random_val_gen());
      fclose(fp);
      exit(0);
    }
    if (turn == 0) {
      // WHITE'S MOVE
      search_output(args, WHITE);
      fprintf(stderr, "MOVE NUMBER: %d\n", turn_number++);
      //fprintf(stderr, "WHITE'S MOVE\n");
      //print_game(args);
      //sleep(6);
      if (possible_moves_i == 1) {
        // MATE
        if (possible_moves[0].result == 1) {
          // BLACK IS IN MATE
          fprintf(stderr, "BLACK IN MATE\n");
          write_to_file(1, fp);
          return;
        } else {
          // WHITE IN MATE
          fprintf(stderr, "WHITE IN MATE\n");
          write_to_file(0, fp);
          return;
        }
        return;
      } else {
        // NORMAL MOVE
        int num = (rand() % (possible_moves_i - 1)) + 1;
        chosen_moves[chosen_moves_i++] = possible_moves[num];
        unsigned int *to = possible_moves[num].move.to;
        unsigned int *from = possible_moves[num].move.from;
        uint64_t cur = (ONE << from[1]);
        TYPE type;
        if (args->piece_boards[WHITE][BISHOP][from[0]] & cur) {
          type = BISHOP;
        } else if (args->piece_boards[WHITE][ROOK][from[0]] & cur) {
          type = ROOK;
        } else if (args->piece_boards[WHITE][QUEEN][from[0]] & cur) {
          type = QUEEN;
        } else if (args->piece_boards[WHITE][PAWN][from[0]] & cur) {
          type = PAWN;
        } else if (args->piece_boards[WHITE][KNIGHT][from[0]] & cur) {
          type = KNIGHT;
        } else {
          type = KING;
        }
        make_move(args, WHITE, type, from, to);
        //printf("from[0] %u\nfrom[1] %u\nto[0]"
        //" %u\nto[1] %u\n", from[0], from[1], to[0], to[1]);
        //print_game(args);
        reset_possible_moves();
      }
      turn = 1;
    } else {
      // BLACK'S MOVE
      search_output(args, BLACK);
      //fprintf(stderr, "BLACK'S MOVE\n");
      //print_game(args);
     // sleep(6);
      if (possible_moves_i == 1) {
        // MATE
        if (possible_moves[0].result == 1) {
          // WHITE IN MATE
          fprintf(stderr, "WHITE IN MATE\n");
          write_to_file(0, fp);
          return;
        } else {
          // BLACK IN MATE
          fprintf(stderr, "BLACK IN MATE\n");
          write_to_file(1, fp);
          return;
        }
        return;
      } else {
        // NORMAL MOVE
        int num = (rand() % (possible_moves_i - 1)) + 1;
        unsigned int *to = possible_moves[num].move.to;
        unsigned int *from = possible_moves[num].move.from;
        uint64_t cur = (ONE << from[1]);
        TYPE type;
        if (args->piece_boards[BLACK][BISHOP][from[0]] & cur) {
          type = BISHOP;
        } else if (args->piece_boards[BLACK][ROOK][from[0]] & cur) {
          type = ROOK;
        } else if (args->piece_boards[BLACK][QUEEN][from[0]] & cur) {
          type = QUEEN;
        } else if (args->piece_boards[BLACK][PAWN][from[0]] & cur) {
          type = PAWN;
        } else if (args->piece_boards[BLACK][KNIGHT][from[0]] & cur) {
          type = KNIGHT;
        } else {
          type = KING;
        }
        make_move(args, BLACK, type, from, to);
        //printf("from[0] %u\nfrom[1] %u\nto[0]"
        //" %u\nto[1] %u\n", from[0], from[1], to[0], to[1]);
        //print_game(args);
        reset_possible_moves();
        turn = 0;
      }
    }
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

void write_to_file(int result, FILE *fp) {
  if (result == 1) {
    // GAME WAS A WIN
    for (int i = 0; i < chosen_moves_i; i++) {
      for (int j = 0; j < 192; j++) {
        fprintf(fp, "%d", chosen_moves[i].position[j]);
      }
      int from = ((chosen_moves[i].level_from) * 63) + (chosen_moves[i].bitposition_from);
      int to = ((chosen_moves[i].level_to) * 63) + (chosen_moves[i].bitposition_to);
      fprintf(fp, ",%d,%d,%d\n", from, to, 1);
    }
  } else {
    // GAME WAS A LOSS
    for (int i = 0; i < chosen_moves_i; i++) {
      for (int j = 0; j < 192; j++) {
        fprintf(fp, "%d", chosen_moves[i].position[j]);
      }
      int from = ((chosen_moves[i].level_from) * 63) + (chosen_moves[i].bitposition_from);
      int to = ((chosen_moves[i].level_to) * 63) + (chosen_moves[i].bitposition_to);
      fprintf(fp, ",%d,%d,%d\n", from, to, 0);
    }
  }
  fclose(fp);
}

unsigned int random_val_gen() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long long state = (((long long) tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
    return (unsigned int) state;
}
