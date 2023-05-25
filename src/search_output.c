#include <search_output.h>

typedef struct node {
  int *position;
  unsigned int level_from;
  unsigned int bitposition_from;
  unsigned int level_to;
  unsigned int bitposition_to;
  unsigned int result;
  MOVE move;
  TYPE piece_type;
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
    possible_moves[possible_moves_i].bitposition_to = 0;
    possible_moves[possible_moves_i++].piece_type = 10;
    //possible_moves[possible_moves_i++].position = (int *) (malloc(192 * sizeof(int)));
    return;
  } else if ((player_flags & MATE) == 1) {
    possible_moves_i = 0;
    possible_moves[possible_moves_i].result = 0;
    possible_moves[possible_moves_i].level_from = 0;
    possible_moves[possible_moves_i].level_to = 0;
    possible_moves[possible_moves_i].bitposition_from = 0;
    possible_moves[possible_moves_i].bitposition_to = 0;
    possible_moves[possible_moves_i++].piece_type = 10;
    //possible_moves[possible_moves_i++].position = (int *) (malloc(192 * sizeof(int)));
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
            possible_moves[possible_moves_i].piece_type = i;
            translate_position(move.args, possible_moves_i++, 1);
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

void translate_position(BOARD_ARGS *args, int arr_pos, int arr_number) {
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
          if (i == BLACK)
            val += 6;
          p[(k * 64) + m] = val;
          x ^= (ONE << m);
          m = log2_lookup(x);
        }
      }
    }
  }
  if (arr_number == 1) {
    possible_moves[arr_pos].position = p;
  } else {
    chosen_moves[arr_pos].position = p;
  }
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
  FILE *fp = fopen("move_data/samples.csv", "a+");
  FILE *labels = fopen("move_data/lables.csv", "a+");
  FILE *all = fopen("move_data/all.csv", "a+");
  FILE *chess_poss = fopen("move_data/chess_poss.csv", "a+");
  FILE *from_move_only = fopen("move_data/from_move_only.csv", "a+");
  int turn_number = 0;
  int turn = 0;
  while (1) {
    turn_number++;
    if (turn_number == 300) {
      fprintf(stderr, "HIT MAX MOVE NUMBER... EXITING...\n");
      fclose(fp);
      fclose(labels);
      fclose(all);
      fclose(chess_poss);
      fclose(from_move_only);
      exit(0);
    }
    if (turn == 0) {
      // WHITE'S MOVE
      search_output(args, WHITE);
      turn++;
      if (possible_moves_i == 1) {
        // MATE
        if (possible_moves[0].result == 1) {
          // BLACK IS IN MATE
          fprintf(stderr, "BLACK IN MATE\n");
          write_to_file(1, fp, labels, all, chess_poss, from_move_only);
          return;
        } else {
          // WHITE IN MATE
          fprintf(stderr, "WHITE IN MATE\n");
          write_to_file(0, fp, labels, all, chess_poss, from_move_only);
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
        reset_possible_moves();
      }
      turn = 1;
    } else {
      // BLACK'S MOVE
      search_output(args, BLACK);
      if (possible_moves_i == 1) {
        // MATE
        if (possible_moves[0].result == 1) {
          // WHITE IN MATE
          fprintf(stderr, "WHITE IN MATE\n");
          write_to_file(0, fp, labels, all, chess_poss, from_move_only);
          return;
        } else {
          // BLACK IN MATE
          fprintf(stderr, "BLACK IN MATE\n");
          write_to_file(1, fp, labels, all, chess_poss, from_move_only);
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

void write_to_file(int result, FILE *samples, FILE *labels, FILE *all, FILE *poss_only, FILE *from_move_only) {
  if (result == 1) {
    // GAME WAS A WIN
    for (int i = 0; i < chosen_moves_i; i++) {
      for (int j = 0; j < 192; j++) {
        fprintf(samples, "%d,", chosen_moves[i].position[j]);
        fprintf(all, "%d,", chosen_moves[i].position[j]);
        if (j != 191)
          fprintf(poss_only, "%d,", chosen_moves[i].position[j]);
        else
          fprintf(poss_only, "%d", chosen_moves[i].position[j]);
      }
      fprintf(poss_only, "\n");
      int from = ((chosen_moves[i].level_from) * 63) + (chosen_moves[i].bitposition_from);
      int to = ((chosen_moves[i].level_to) * 63) + (chosen_moves[i].bitposition_to);
      unsigned int piece_type = chosen_moves[i].piece_type;
      fprintf(samples, "%d,%u\n", from, piece_type);
      fprintf(labels, "%d\n", to);
      fprintf(all, "%d,%u,%d\n", from, piece_type, to);
      fprintf(from_move_only, "%d\n", from);
    }
  } else {
    // GAME WAS A LOSS
    for (int i = 0; i < chosen_moves_i; i++) {
      for (int j = 0; j < 192; j++) {
        fprintf(samples, "%d,", chosen_moves[i].position[j]);
        fprintf(all, "%d,", chosen_moves[i].position[j]);
        if (j != 191)
          fprintf(poss_only, "%d,", chosen_moves[i].position[j]);
        else
          fprintf(poss_only, "%d", chosen_moves[i].position[j]);
      }
      fprintf(poss_only, "\n");
      int from = ((chosen_moves[i].level_from) * 63) + (chosen_moves[i].bitposition_from);
      int to = ((chosen_moves[i].level_to) * 63) + (chosen_moves[i].bitposition_to);
      unsigned int piece_type = chosen_moves[i].piece_type;
      fprintf(samples, "%d,%u\n", from, piece_type);
      fprintf(labels, "%d\n", to);
      fprintf(all, "%d,%u,%d\n", from, piece_type, to);
      fprintf(from_move_only, "%d\n", from);
    }
  }
  fclose(samples);
  fclose(labels);
  fclose(all);
  fclose(poss_only);
  fclose(from_move_only);
}

unsigned int random_val_gen() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long long state = (((long long) tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
    return (unsigned int) state;
}

void five_move_random(BOARD_ARGS *args, SIDE to_move) {
  srand(random_val_gen());
  FILE *fp = fopen("move_data/samples.csv", "a+");
  FILE *labels = fopen("move_data/lables.csv", "a+");
  FILE *all = fopen("move_data/all.csv", "a+");
  FILE *chess_poss = fopen("move_data/chess_poss.csv", "a+");
  FILE *from_move_only = fopen("move_data/from_move_only.csv", "a+");
  int turn_number = 0;
  int turn = 0;
  while (1) {
    turn_number++;
    fprintf(stderr, "MOVE NUMBER: %d\n", turn_number);
    if (turn_number == 150) {
      fprintf(stderr, "HIT MAX MOVE NUMBER... EXITING...\n");
      fclose(fp);
      fclose(labels);
      fclose(all);
      fclose(chess_poss);
      fclose(from_move_only);
      exit(0);
    }
    if (turn_number <= 10) {
      // PLAY RANDOM MOVES
        if (turn == 0) {
        // WHITE'S MOVE
        search_output(args, WHITE);
        turn++;
        if (possible_moves_i == 1) {
          // MATE
          if (possible_moves[0].result == 1) {
            // BLACK IS IN MATE
            fprintf(stderr, "BLACK IN MATE\n");
            write_to_file(1, fp, labels, all, chess_poss, from_move_only);
            return;
          } else {
            // WHITE IN MATE
            fprintf(stderr, "WHITE IN MATE\n");
            write_to_file(0, fp, labels, all, chess_poss, from_move_only);
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
          reset_possible_moves();
        }
        turn = 1;
      } else {
        // BLACK'S MOVE
        search_output(args, BLACK);
        if (possible_moves_i == 1) {
          // MATE
          if (possible_moves[0].result == 1) {
            // WHITE IN MATE
            fprintf(stderr, "WHITE IN MATE\n");
            write_to_file(0, fp, labels, all, chess_poss, from_move_only);
            return;
          } else {
            // BLACK IN MATE
            fprintf(stderr, "BLACK IN MATE\n");
            write_to_file(1, fp, labels, all, chess_poss, from_move_only);
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
          reset_possible_moves();
          turn = 0;
        }
      }
    } else {
      // PLAY CALCULATED MOVES
      if (turn == 0) {
        // WHITE MOVE
        MOVE com_move = search(args, WHITE, 4, INT_MIN, INT_MAX, turn);
        if (com_move.score == -2) {
          // BLACK IN MATE
          fprintf(stderr, "BLACK IN MATE\n");
          write_to_file(1, fp, labels, all, chess_poss, from_move_only);
          return;
        } else if (com_move.score == -1) {
          // WHITE IN MATE
          fprintf(stderr, "WHITE IN MATE\n");
          write_to_file(0, fp, labels, all, chess_poss, from_move_only);
          return;
        }
        unsigned int *to = com_move.to;
        unsigned int *from = com_move.from;
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
        turn = 1;
        make_move(args, WHITE, type, from, to);
        translate_position(args, chosen_moves_i, 0);
        chosen_moves[chosen_moves_i].result = -1;
        chosen_moves[chosen_moves_i].level_from = from[0];
        chosen_moves[chosen_moves_i].level_to = to[0];
        chosen_moves[chosen_moves_i].bitposition_from = from[1];
        chosen_moves[chosen_moves_i].bitposition_to = to[1];
        chosen_moves[chosen_moves_i].move = com_move;
        chosen_moves[chosen_moves_i++].piece_type = type;
        for (int i = 1; i < chosen_moves_i - 3; i++) {
          if (chosen_moves[i - 1].bitposition_from == chosen_moves[i + 2].bitposition_to) {
            fprintf(stderr, "REPEATED MOVES, EXITING...\n");
            fclose(fp);
            fclose(labels);
            fclose(all);
            fclose(chess_poss);
            fclose(from_move_only);
            int ret = system("./outop");
            if (ret < 0) {
              perror("system");
            }
            _exit(1);
          }
        }
        print_game(args);
      } else {
        // BLACK MOVE
        MOVE com_move = search(args, BLACK, 4, INT_MIN, INT_MAX, turn);
        if (com_move.score == -1) {
          // BLACK IN MATE
          fprintf(stderr, "BLACK IN MATE\n");
          write_to_file(1, fp, labels, all, chess_poss, from_move_only);
          return;
        } else if (com_move.score == -2) {
          // WHITE IN MATE
          fprintf(stderr, "WHITE IN MATE\n");
          write_to_file(0, fp, labels, all, chess_poss, from_move_only);
          return;
        }
        unsigned int *to = com_move.to;
        unsigned int *from = com_move.from;
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
        turn = 0;
        make_move(args, BLACK, type, from, to);
        print_game(args);
      }
    }
  }
}
