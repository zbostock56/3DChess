#include <psuedo_legal_moves.h>
#include <frontend.h>
#include <string.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
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
  int to_move = 0;
  unsigned int turn = 1;
  //printf("Do you want the computer to play itself? (Y or N)\n");
  //char *c = "";
  //scanf("%s", c);
  if /*(!(strcmp(c,"Y")) || !(strcmp(c,"y")))*/ (1) {
    // Computer is going to play itself
    while (1) {
      if (to_move == 0) {
        /*
          To/From: {Level, Bitposition}
        */
        //MOVE com_move = level_zero_search(&game, turn, BLACK, 4, INT_MIN, INT_MAX);
        //MOVE com_move = search(&game, BLACK, 4);
        MOVE com_move = search(&game, WHITE, 4, INT_MIN, INT_MAX, turn);
        unsigned int *to = com_move.to;
        unsigned int *from = com_move.from;
        printf("WHITE'S MOVE\nfrom[0] %u\nfrom[1] %u\nto[0] %u\nto[1] %u\n", from[0], from[1], to[0], to[1]);
        printf("COMPUTER EVAL: %d\n", com_move.score);
        uint64_t cur = (ONE << from[1]);
        TYPE type;
        if (game.piece_boards[WHITE][BISHOP][from[0]] & cur) {
          type = BISHOP;
        } else if (game.piece_boards[WHITE][ROOK][from[0]] & cur) {
          type = ROOK;
        } else if (game.piece_boards[WHITE][QUEEN][from[0]] & cur) {
          type = QUEEN;
        } else if (game.piece_boards[WHITE][PAWN][from[0]] & cur) {
          type = PAWN;
        } else if (game.piece_boards[WHITE][KNIGHT][from[0]] & cur) {
          type = KNIGHT;
        } else {
          type = KING;
        }
        make_move(&game, WHITE, type, from, to);
        to_move = 1;
        printf("Positions Searched: %llu\n", stuff);
        stuff = 0;
        //printf("Rating: %u\n", com_move.rating);
        print_game(&game);
        //return 0;
        turn++;

      } else {
        /*
          To/From: {Level, Bitposition}
        */
        //MOVE com_move = level_zero_search(&game, turn, BLACK, 4, INT_MIN, INT_MAX);
        //MOVE com_move = search(&game, BLACK, 4);
        MOVE com_move = search(&game, BLACK, 4, INT_MIN, INT_MAX, turn);
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
        stuff = 0;
        //printf("Rating: %u\n", com_move.rating);
        print_game(&game);
        //return 0;
        turn++;
      }
    }
  } else {
    // Assume normal play
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
        //MOVE com_move = level_zero_search(&game, turn, BLACK, 4, INT_MIN, INT_MAX);
        //MOVE com_move = search(&game, BLACK, 4);
        MOVE com_move = search(&game, BLACK, 4, INT_MIN, INT_MAX, turn);
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
        //printf("Rating: %u\n", com_move.rating);
        //print_game(&game);
        //return 0;
        turn++;
      }
    }
    }
  }

#if 0
float board[] = {
9.000000, 0.000000, 9.000000, 0.875000, 0.875000,
9.000000, 0.000000, 12.000000, 1.000000, 0.875000,
12.000000, 0.000000, 12.000000, 1.000000, 1.000000,
12.000000, 0.000000, 9.000000, 0.875000, 1.000000,
9.000000, 0.000000, -3.000000, 0.375000, 0.875000,
9.000000, 0.000000, 0.000000, 0.500000, 0.875000,
12.000000, 0.000000, 0.000000, 0.500000, 1.000000,
12.000000, 0.000000, -3.000000, 0.375000, 1.000000,
-3.000000, 0.000000, -3.000000, 0.375000, 0.375000,
-3.000000, 0.000000, 0.000000, 0.500000, 0.375000,
0.000000, 0.000000, 0.000000, 0.500000, 0.500000,
0.000000, 0.000000, -3.000000, 0.375000, 0.500000,
-3.000000, 0.000000, 9.000000, 0.875000, 0.375000,
-3.000000, 0.000000, 12.000000, 1.000000, 0.375000,
0.000000, 0.000000, 12.000000, 1.000000, 0.500000,
0.000000, 0.000000, 9.000000, 0.875000, 0.500000,
-3.000000, 0.000000, 3.000000, 0.625000, 0.375000,
-3.000000, 0.000000, 6.000000, 0.750000, 0.375000,
0.000000, 0.000000, 6.000000, 0.750000, 0.500000,
0.000000, 0.000000, 3.000000, 0.625000, 0.500000,
-9.000000, 0.000000, 3.000000, 0.625000, 0.125000,
-9.000000, 0.000000, 6.000000, 0.750000, 0.125000,
-6.000000, 0.000000, 6.000000, 0.750000, 0.250000,
-6.000000, 0.000000, 3.000000, 0.625000, 0.250000,
-9.000000, 0.000000, 9.000000, 0.875000, 0.125000,
-9.000000, 0.000000, 12.000000, 1.000000, 0.125000,
-6.000000, 0.000000, 12.000000, 1.000000, 0.250000,
-6.000000, 0.000000, 9.000000, 0.875000, 0.250000,
-3.000000, 0.000000, -9.000000, 0.125000, 0.375000,
-3.000000, 0.000000, -6.000000, 0.250000, 0.375000,
0.000000, 0.000000, -6.000000, 0.250000, 0.500000,
0.000000, 0.000000, -9.000000, 0.125000, 0.500000,
-9.000000, 0.000000, -9.000000, 0.125000, 0.125000,
-9.000000, 0.000000, -6.000000, 0.250000, 0.125000,
-6.000000, 0.000000, -6.000000, 0.250000, 0.250000,
-6.000000, 0.000000, -9.000000, 0.125000, 0.250000,
-9.000000, 0.000000, -3.000000, 0.375000, 0.125000,
-9.000000, 0.000000, 0.000000, 0.500000, 0.125000,
-6.000000, 0.000000, 0.000000, 0.500000, 0.250000,
-6.000000, 0.000000, -3.000000, 0.375000, 0.250000,
9.000000, 0.000000, -9.000000, 0.125000, 0.875000,
9.000000, 0.000000, -6.000000, 0.250000, 0.875000,
12.000000, 0.000000, -6.000000, 0.250000, 1.000000,
12.000000, 0.000000, -9.000000, 0.125000, 1.000000,
3.000000, 0.000000, -9.000000, 0.125000, 0.625000,
3.000000, 0.000000, -6.000000, 0.250000, 0.625000,
6.000000, 0.000000, -6.000000, 0.250000, 0.750000,
6.000000, 0.000000, -9.000000, 0.125000, 0.750000,
3.000000, 0.000000, -3.000000, 0.375000, 0.625000,
3.000000, 0.000000, 0.000000, 0.500000, 0.625000,
6.000000, 0.000000, 0.000000, 0.500000, 0.750000,
6.000000, 0.000000, -3.000000, 0.375000, 0.750000,
9.000000, 0.000000, 3.000000, 0.625000, 0.875000,
9.000000, 0.000000, 6.000000, 0.750000, 0.875000,
12.000000, 0.000000, 6.000000, 0.750000, 1.000000,
12.000000, 0.000000, 3.000000, 0.625000, 1.000000,
3.000000, 0.000000, 3.000000, 0.625000, 0.625000,
3.000000, 0.000000, 6.000000, 0.750000, 0.625000,
6.000000, 0.000000, 6.000000, 0.750000, 0.750000,
6.000000, 0.000000, 3.000000, 0.625000, 0.750000,
3.000000, 0.000000, 9.000000, 0.875000, 0.625000,
3.000000, 0.000000, 12.000000, 1.000000, 0.625000,
6.000000, 0.000000, 12.000000, 1.000000, 0.750000,
6.000000, 0.000000, 9.000000, 0.875000, 0.750000,
3.000000, 0.000000, -12.000000, 0.000000, 0.625000,
6.000000, 0.000000, -12.000000, 0.000000, 0.750000,
0.000000, 0.000000, -12.000000, 0.000000, 0.500000,
9.000000, 0.000000, -12.000000, 0.000000, 0.875000,
12.000000, 0.000000, -12.000000, 0.000000, 1.000000,
-12.000000, 0.000000, -6.000000, 0.250000, 0.000000,
-12.000000, 0.000000, -3.000000, 0.375000, 0.000000,
-12.000000, 0.000000, 0.000000, 0.500000, 0.000000,
-9.000000, 0.000000, -12.000000, 0.000000, 0.125000,
-6.000000, 0.000000, -12.000000, 0.000000, 0.250000,
-12.000000, 0.000000, -12.000000, 0.000000, 0.000000,
-12.000000, 0.000000, -9.000000, 0.125000, 0.000000,
-3.000000, 0.000000, -12.000000, 0.000000, 0.375000,
-12.000000, 0.000000, 6.000000, 0.750000, 0.000000,
-12.000000, 0.000000, 9.000000, 0.875000, 0.000000,
-12.000000, 0.000000, 12.000000, 1.000000, 0.000000,
-12.000000, 0.000000, 3.000000, 0.625000, 0.000000
};

unsigned int indicies[] = {
1, 0, 3,
2, 1, 3,
5, 4, 7,
6, 5, 7,
9, 8, 11,
10, 9, 11,
13, 12, 15,
14, 13, 15,
17, 16, 19,
18, 17, 19,
21, 20, 23,
22, 21, 23,
25, 24, 27,
26, 25, 27,
29, 28, 31,
30, 29, 31,
33, 32, 35,
34, 33, 35,
37, 36, 39,
38, 37, 39,
41, 40, 43,
42, 41, 43,
45, 44, 47,
46, 45, 47,
49, 48, 51,
50, 49, 51,
53, 52, 55,
54, 53, 55,
57, 56, 59,
58, 57, 59,
61, 60, 63,
62, 61, 63,
60, 57, 58,
63, 60, 58,
15, 18, 57,
60, 15, 57,
14, 15, 60,
61, 14, 60,
56, 49, 50,
59, 56, 50,
19, 10, 49,
56, 19, 49,
18, 19, 56,
57, 18, 56,
52, 5, 6,
55, 52, 6,
59, 50, 5,
52, 59, 5,
58, 59, 52,
53, 58, 52,
48, 45, 46,
51, 48, 46,
11, 30, 45,
48, 11, 45,
10, 11, 48,
49, 10, 48,
44, 64, 65,
47, 44, 65,
31, 66, 64,
44, 31, 64,
30, 31, 44,
45, 30, 44,
40, 67, 68,
43, 40, 68,
47, 65, 67,
40, 47, 67,
46, 47, 40,
41, 46, 40,
36, 33, 34,
39, 36, 34,
70, 69, 33,
36, 70, 33,
71, 70, 36,
37, 71, 36,
32, 72, 73,
35, 32, 73,
75, 74, 72,
32, 75, 72,
69, 75, 32,
33, 69, 32,
28, 76, 66
};

#include <glad/glad.h>
#include <glfw3.h>
int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "ZACK IS LAME\n", NULL, NULL);
  glfwMakeContextCurrent(window);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  int success;
  const char *vs = "\
  #version 460 core\n\
  layout (location = 0) in vec3 aPos;\n\
  uniform mat4 proj;\n\
  out vec3 col;\n\
  void main() {\n\
    mat4 t = mat4(\
    vec4(1.0, 0.0, 0.0, 0.0),\
    vec4(0.0, 1.0, 0.0, 0.0),\
    vec4(0.0, 0.0, 1.0, 0.0),\
    vec4(0.0, -0.75, -4.0, 1.0)\
    );\
    mat4 r = mat4(\
    vec4(1.0, 0.0, 0.0, 0.0),\
    vec4(0.0, 0.0, 1.0, 0.0),\
    vec4(0.0, -1.0, 0.0, 0.0),\
    vec4(0.0, 0.0, 0.0, 1.0)\
    );\
    vec3 c = vec3(0.0, 0.0, 1.0);\n\
    if (proj[0][0] == 0.0) {\n\
      c[0] = 1.0;\n\
    }\n\
    gl_Position = proj * t * vec4(aPos, 1.0);\n\
    col = vec3(c);\n\
  }\n";
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vs, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  char info_log[512];
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, info_log);
    printf("ERR::VS\n%s\n", info_log);
  }

  const char *fs = "\
  #version 460 core\n\
  out vec4 FragColor;\n\
  in vec3 col;\n\
  void main() {\n\
    FragColor = vec4(col, 1.0);\n\
  }\n";
  unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_shader, 1, &fs, NULL);
  glCompileShader(frag_shader);
  glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
    printf("ERR::FS\n%s\n", info_log);
  }

  unsigned int shader_prog = glCreateProgram();
  glAttachShader(shader_prog, vertexShader);
  glAttachShader(shader_prog, frag_shader);
  glLinkProgram(shader_prog);
  glGetShaderiv(shader_prog, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_prog, 512, NULL, info_log);
    printf("ERR::PROG\n%s\n", info_log);
  }

  /*float points[] = {
  -0.5f, -0.5f, -1.0f,
  0.5f, -0.5f, -1.0f,
  0.0f, 0.5f, -1.0f
  };*/

  unsigned int VAO, VBO, EBO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(board), board, GL_STATIC_DRAW);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *) 0);
  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *) (sizeof(float) * 3));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  mat4 proj;
  glm_perspective(glm_rad(45.0), 800.0/600.0, 0.1, 100.0, proj);
  printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
         proj[0][0], proj[0][1], proj[0][2], proj[0][3],
         proj[1][0], proj[1][1], proj[1][2], proj[1][3],
         proj[2][0], proj[2][1], proj[2][2], proj[2][3],
         proj[3][0], proj[3][1], proj[3][2], proj[3][3]);
  glUseProgram(shader_prog);
  glUniformMatrix4fv(glGetUniformLocation(shader_prog, "proj"), 1, GL_FALSE, (float *) proj);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_prog);
    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, sizeof(indicies) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

}
#endif
