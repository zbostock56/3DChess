#include <psuedo_legal_moves.h>
#include <frontend.h>
#include <graphics_board_helpers.h>

// GRAPHICS MECHANICS
vec3 col_point = { 0.0, 0.0, 0.0 };
int enable_gravity;

vec3 cam_front = { -0.72, -0.68, 0.0 };
// {forward/back, up/down, left/right}
vec3 cam_pos = { 1.3, 2.35, -1.0 };
vec3 cam_up = { 0.0, 1.0, 0.0 };
float pitch = -45.0;
float yaw = -180.0;
int firstMouse = 1;

float deltaTime = 0.0;
float lastFrame = 0.0;
double lastX;
double lastY;

int pressed = 0;
unsigned int active_board = 1;

// GAME MECHANICS
BOARD_ARGS game;
int piece = 0;
unsigned int from[2] = {0,0};
unsigned int to[2] = {0,0};
int to_move = 0; // WHITE == 0   BLACK == 1
pthread_t g;
int change_ready = 0;
int turn = 1;
vec3 g_new_pos = {0.0, 0.0, 0.0};
int piece_to_move = 0;
int piece_taken = 0;
int white_pieces = 16;
int black_pieces = 16;
int moved = 0;

/*
  ORDER:
  King
  Queen
  Left Bishop
  Right Bishop
  Left Rook
  Right Rook
  Left Knight
  Right Knight
  Pawns from left to right
*/

int game_board[3][64] = {
{
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0
},
{
5,8,3,1,2,4,7,6, // WHITE
9,10,11,12,13,14,15,16,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
25,26,27,28,29,30,31,32,
21,23,19,17,18,20,24,22  // BLACK
},
{
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0
}
};

// Helper Functions

void set_game_to_graphics(unsigned int *from, unsigned int *to) {
  if (to_move == 0) {
    moved = 0;
  } else {
    moved = 1;
  }
  int temp = game_board[from[0]][from[1]];
  piece_to_move = temp;
  piece_to_move--; // OFFSET PIECE NUMBER FOR ARRAY INDEXING
  if (to_move == 1) {
    // BLACK IS MOVING, SUBTRACT BY 16 FOR CORRECT OFFSET
    piece_to_move--;
  }
  game_board[from[0]][from[1]] = 0;
  if (game_board[to[0]][to[1]] != 0) {
    piece_taken = game_board[to[0]][to[1]];
  }
  game_board[to[0]][to[1]] = temp;
  // {forward/back, up/down, left/right}
  glm_vec3_copy(graphics_lookup[to[0]][to[1]], g_new_pos);
  //fprintf(stderr, "\n***** g_new_pos: %f | %f | %f ******\n\n", 
  //g_new_pos[0], g_new_pos[1], g_new_pos[2]);
  change_ready = 1;
}

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

int switch_letters(char letter) {
  if (letter == 'A') {
    return 7;
  } else if (letter == 'B') {
    return 6;
  } else if (letter == 'C') {
    return 5;
  } else if (letter == 'D') {
    return 4;
  } else if (letter == 'E') {
    return 3;
  } else if (letter == 'F') {
    return 2;
  } else if (letter == 'G') {
    return 1;
  } else {
    return 0;
  }
}

void play() {
  while (1) {
    if (to_move == 0) {
      unsigned int to[2] = {0, 0};
      unsigned int from[2] = {0, 0};
      fprintf(stderr, "\n**********  MOVE %d **********\n\n", turn);
      print_game(&game);
      TYPE type = 0;
      printf("Enter piece to move:\n0: BISHOP\n1: ROOK\n2: QUEEN\n"
      "3: PAWN\n4: KNIGHT\n5: KING\nQuit (Any Other)\n");
      int error = scanf("%d", &piece);
      if (error != EOF) {
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
          change_ready = -1;
          exit(0);
        }
      } else {
        perror("scanf (piece input)");
        exit(1);
      }

      char file_from;
      unsigned int rank_from = 0;
      printf("Enter Level (0 - 2) and position (A - H)(1 - 8)"
      " of a piece\n[0-2] [A-H][1-8]\n");
      error = scanf("%u %c%u", &from[0], &file_from, &rank_from);
      if (error != EOF) {
        if (file_from > 72) {
          file_from = 'H';
        } else if (file_from < 65) {
          file_from = 'A';
        }
        if (from[0] > 2) {
          from[0] = 2;
        } else if (from[0] < 0) {
          from[0] = 0;
        }
        from[1] += switch_letters(file_from);
        from[1] += ((rank_from - 1) * 8);
      } else {
        perror("scanf (from input)");
        exit(1);
      }

      char file_to;
      unsigned int rank_to = 0;
      printf("Enter Level (0 - 2) and position (A - H)(1 - 8) to"
      " ending position\n[0-2] [A-H][1-8]\n");
      error = scanf("%u %c%u", &to[0], &file_to, &rank_to);
      if (error != EOF) {
        if (file_to > 72) {
          file_from = 'H';
        } else if (file_to < 65) {
          file_to = 'A';
        }
        if (to[0] > 2) {
          to[0] = 2;
        } else if (to[0] < 0) {
          to[0] = 0;
        }
        to[1] += switch_letters(file_to);
        to[1] += ((rank_to - 1) * 8);
      } else {
        perror("scanf (to input)");
        exit(1);
      }

      printf("from[0] %u\nfrom[1] %u\nto[0]"
      " %u\nto[1] %u\n", from[0], from[1], to[0], to[1]);
      make_move(&game, WHITE, type, from, to);
      // MOVE PIECE ON GAME BOARD FOR GRAPHICS RENDERING
      set_game_to_graphics(from, to);
      to_move = 1;
    } else {
      //MOVE com_move = level_zero_search(&game, turn, BLACK, 4, INT_MIN, INT_MAX);
      MOVE com_move = search(&game, BLACK, 4, INT_MIN, INT_MAX, turn);
      unsigned int *to = com_move.to;
      unsigned int *from = com_move.from;
      printf("BLACK'S MOVE\nfrom[0] %u\nfrom[1]"
      " %u\nto[0] %u\nto[1] %u\n", from[0], from[1], to[0], to[1]);
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

      to_move = 0;
      printf("Positions Searched: %llu\n", stuff);
      stuff = 0;
      //printf("Rating: %u\n", com_move.rating);
      make_move(&game, BLACK, type, from, to);
      //print_game(&game);
      turn++;

      // MOVE PIECE ON GAME BOARD FOR GRAPHICS RENDERING
      set_game_to_graphics(from, to);
    }
  }
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(1920, 1080, "3DChess\n", NULL, NULL);
  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, mouse_input);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  unsigned int piece_shader = init_shader_prog(
    DIR"/src/shaders/pieces/shader.vs",
    NULL,
    DIR"/src/shaders/pieces/shader.fs"
  );
  if (piece_shader == -1) {
    printf("Shader err\n");
    glfwTerminate();
    return -1;
  }

  unsigned int board_shader = init_shader_prog(
    DIR"/src/shaders/board/shader.vs",
    NULL,
    DIR"/src/shaders/board/shader.fs"
  );
  if (board_shader == -1) {
    printf("Shader err\n");
    glfwTerminate();
    return -1;
  }

  MODEL *cube = load_model(
    DIR"/blender/cube/cube.obj"
    );
  if (cube == NULL) {
    printf("Unable to load model\n");
    glfwTerminate();
    return -1;
  }

  MODEL *king = load_model(
    DIR"/blender/king/king.obj"
    );
  if (king == NULL) {
    printf("Unable to king model\n");
    glfwTerminate();
    return -1;
  }

  MODEL *queen = load_model(
    DIR"/blender/queen/queen.obj"
    );
  if (queen == NULL) {
    printf("Unable to queen model\n");
    glfwTerminate();
    return -1;
  }

  MODEL *bishop = load_model(
    DIR"/blender/bishop/bishop.obj"
    );
  if (bishop == NULL) {
    printf("Unable to bishop model\n");
    glfwTerminate();
    return -1;
  }

  MODEL *knight = load_model(
    DIR"/blender/knight/knight.obj"
    );
  if (knight == NULL) {
    printf("Unable to knight model\n");
    glfwTerminate();
    return -1;
  }

  MODEL *rook = load_model(
    DIR"/blender/rook/rook.obj"
    );
  if (rook == NULL) {
    printf("Unable to rook model\n");
    glfwTerminate();
    return -1;
  }

  MODEL *pawn = load_model(
    DIR"/blender/pawn/pawn.obj"
    );
  if (pawn == NULL) {
    printf("Unable to pawn model\n");
    glfwTerminate();
    return -1;
  }

  MODEL *board = load_model(
    DIR"/blender/board/chess_board.obj");

  vec3 piece_scale = { 0.1, 0.1, 0.1 };
  vec3 pawn_scale = { 0.075, 0.075, 0.075 };
  float outline_scale = 1.001;
  //vec3 outline_scale = { 0.1001, 0.1001, 0.1001 };
  versor white_rot = { 0.0, -0.7071, 0.0, 0.7071 };
  versor black_rot = { 0.0, 0.7071, 0.0, 0.7071 };

  /*ENTITY *board_ent = init_entity(board);
  vec3 board_pos = { -1.05, 0.0, -1.05 };
  glm_vec3_copy(piece_scale, board_ent->scale);
  glm_vec3_copy(board_pos, board_ent->translation);
  glm_quat_copy(board_rot, board_ent->rotation);*/

  versor board_rot = { 0.0, 0.7071, 0.0, 0.7071 };
  ENTITY *boards[3] = {
    init_entity(board),
    init_entity(board),
    init_entity(board)
  };
  vec3 board_pos[3] = {
    { -1.05, 0.7, -1.05 }, // TOP
    { -1.05, 0.0, -1.05 }, // MIDDLE
    { -1.05, -0.7, -1.05 }  // BOTTOM
  };
  for (int i = 0; i < 3; i++) {
    glm_vec3_copy(piece_scale, boards[i]->scale);
    glm_vec3_copy(board_pos[i], boards[i]->translation);
    glm_quat_copy(board_rot, boards[i]->rotation);
  }

  ENTITY *white[16] = {
    init_entity(king),
    init_entity(queen),
    init_entity(bishop),
    init_entity(bishop),
    init_entity(rook),
    init_entity(rook),
    init_entity(knight),
    init_entity(knight),
    init_entity(pawn),
    init_entity(pawn),
    init_entity(pawn),
    init_entity(pawn),
    init_entity(pawn),
    init_entity(pawn),
    init_entity(pawn),
    init_entity(pawn)
  };

  vec3 w_pos[16] = {
    { 0.0, 0.0, -1.2 }, // KING
    { 0.0, 0.0, -0.9 }, // QUEEN
    { 0.0, 0.0, -1.5 }, // BISHOP
    { 0.0, 0.0, -0.6 }, // BISHOP
    { 0.0, 0.0, -2.1 }, // ROOK
    { 0.0, 0.0, 0.0 }, // ROOK
    { 0.0, 0.0, -0.3 }, // KNIGHT
    { 0.0, 0.0, -1.8 }, // KNIGHT
    { -0.3, 0.0, -2.1 }, // PAWN
    { -0.3, 0.0, -1.8 },
    { -0.3, 0.0, -1.5 },
    { -0.3, 0.0, -1.2 },
    { -0.3, 0.0, -0.9 },
    { -0.3, 0.0, -0.6 },
    { -0.3, 0.0, -0.3 },
    { -0.3, 0.0, 0.0 },
  };

  for (int i = 0; i < 16; i++) {
    glm_vec3_copy(piece_scale, white[i]->scale);
    if (i == 6 || i == 7) { // KNIGHT
      glm_vec3_copy(white_rot, white[i]->rotation);
    }
    glm_vec3_copy(w_pos[i], white[i]->translation);
  }

  ENTITY *black[16] = {
    init_entity(king),
    init_entity(queen),
    init_entity(bishop),
    init_entity(bishop),
    init_entity(rook),
    init_entity(rook),
    init_entity(knight),
    init_entity(knight),
    init_entity(pawn),
    init_entity(pawn),
    init_entity(pawn),
    init_entity(pawn),
    init_entity(pawn),
    init_entity(pawn),
    init_entity(pawn),
    init_entity(pawn)
  };

  vec3 b_pos[16] = {
    { -2.1, 0.0, -1.2 }, // KING
    { -2.1, 0.0, -0.9 }, // QUEEN
    { -2.1, 0.0, -0.6 }, // BISHOP
    { -2.1, 0.0, -1.5 }, // BISHOP
    { -2.1, 0.0, 0.0 }, // ROOK
    { -2.1, 0.0, -2.1 }, // ROOK
    { -2.1, 0.0, -0.3 }, // KNIGHT
    { -2.1, 0.0, -1.8 }, // KNIGHT
    { -1.8, 0.0, -2.1 }, // PAWN
    { -1.8, 0.0, -1.8 },
    { -1.8, 0.0, -1.5 },
    { -1.8, 0.0, -1.2 },
    { -1.8, 0.0, -0.9 },
    { -1.8, 0.0, -0.6 },
    { -1.8, 0.0, -0.3 },
    { -1.8, 0.0, 0.0 },
  };

  int is_drawn_b[16] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
  };

  int is_drawn_w[16] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
  };

  for (int i = 0; i < 16; i++) {
    glm_vec3_copy(piece_scale, black[i]->scale);
    if (i == 6 || i == 7) { // KNIGHT
      glm_vec3_copy(black_rot, black[i]->rotation);
    }
    glm_vec3_copy(b_pos[i], black[i]->translation);
  }

  mat4 view = GLM_MAT4_IDENTITY_INIT;
  mat4 proj;
  glm_perspective(glm_rad(45.0), 800.0/600.0, 0.1, 100.0, proj);
  /*printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
         proj[0][0], proj[0][1], proj[0][2], proj[0][3],
         proj[1][0], proj[1][1], proj[1][2], proj[1][3],
         proj[2][0], proj[2][1], proj[2][2], proj[2][3],
         proj[3][0], proj[3][1], proj[3][2], proj[3][3]);*/
  glUseProgram(piece_shader);
  glUniformMatrix4fv(glGetUniformLocation(piece_shader, "projection"), 1,
                     GL_FALSE, (float *) proj);

  glUseProgram(board_shader);
  glUniformMatrix4fv(glGetUniformLocation(board_shader, "projection"), 1,
                     GL_FALSE, (float *) proj);

  glEnable(GL_DEPTH_TEST);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // START GAME THREAD
  init_pieceboard(game);
  pthread_create(&g, NULL, (void *)(&play), NULL);

  while (!glfwWindowShouldClose(window)) {
  //fprintf(stderr, "%f %f %f\n%f %f %f\npitch: %f\nyaw: %f\n", cam_pos[0], cam_pos[1], cam_pos[2], cam_front[0], cam_front[1], cam_front[2], pitch, yaw);
    if (change_ready == 1) {
      change_ready = 0;
      if (moved == 0) {
        // WHITE PIECE IS MOVING
        glm_vec3_copy(g_new_pos, w_pos[piece_to_move]);
        if (piece_taken != 0) {
          // BLACK PIECE HAS BEEN TAKEN
          piece_taken -= 16;
          piece_taken--;
          is_drawn_b[piece_taken] = 0;
          piece_taken = 0;
        }
      } else {
        // BLACK PIECE IS MOVING
        glm_vec3_copy(g_new_pos, b_pos[piece_to_move]);
        if (piece_taken != 0) {
          // WHITE PIECE HAS BEEN TAKEN
          piece_taken--;
          is_drawn_w[piece_taken] = 0;
          piece_taken = 0;
        }
      }
    } else if (change_ready == -1) {
      exit(0);
    }

    float curFrame = glfwGetTime();
    deltaTime = curFrame - lastFrame;
    lastFrame = curFrame;

    keyboard_input(window);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glUseProgram(board_shader);
    glUniformMatrix4fv(glGetUniformLocation(board_shader, "view"), 1,
                       GL_FALSE, (float *) view);
    for (int i = 0; i < 3; i++) {
      if (i == active_board) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      draw_entity(board_shader, boards[i]);
    }

    glUseProgram(piece_shader);
    glm_look(cam_pos, cam_front, cam_up, view);
    glUniformMatrix4fv(glGetUniformLocation(piece_shader, "view"), 1,
                       GL_FALSE, (float *) view);

    // WHITE ATTRIBUTES
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUniform3f(glGetUniformLocation(piece_shader, "col"), 0.0, 0.75, 1.0);
    for (int i = 0; i < 16; i++) {
      if (is_drawn_w[i]) {
        glm_vec3_scale(white[i]->scale, outline_scale, white[i]->scale);
        draw_entity(piece_shader, white[i]);
      }
    }
    glUniform3f(glGetUniformLocation(piece_shader, "col"), 1.0, 1.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (int i = 0; i < 16; i++) {
      if (is_drawn_w[i]) {
        if (i < 8) {
          glm_vec3_copy(piece_scale, white[i]->scale);
        } else {
          glm_vec3_copy(pawn_scale, white[i]->scale);
        }
        glm_vec3_copy(w_pos[i], white[i]->translation);
        draw_entity(piece_shader, white[i]);
      }
    }

    // BLACK ATTRIBUTES
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUniform3f(glGetUniformLocation(piece_shader, "col"), 1.0, 119.0/255.0, 0.0);
    for (int i = 0; i < 16; i++) {
      if (is_drawn_b[i]) {
        glm_vec3_scale(black[i]->scale, outline_scale, black[i]->scale);
        draw_entity(piece_shader, black[i]);
      }
    }
    glUniform3f(glGetUniformLocation(piece_shader, "col"), 0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (int i = 0; i < 16; i++) {
      if (is_drawn_b[i]) {
        if (i < 8) {
          glm_vec3_copy(piece_scale, black[i]->scale);
        } else {
          glm_vec3_copy(pawn_scale, black[i]->scale);
        }
        glm_vec3_copy(b_pos[i], black[i]->translation);
        draw_entity(piece_shader, black[i]);
      }
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}

void keyboard_input(GLFWwindow *window) {
  float cam_speed = 0.5 * deltaTime;
  vec3 movement = GLM_VEC3_ZERO_INIT;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    glm_vec3_scale_as(cam_front, cam_speed, movement);
    glm_vec3_add(movement, cam_pos, cam_pos);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    glm_vec3_scale_as(cam_front, -cam_speed, movement);
    glm_vec3_add(movement, cam_pos, cam_pos);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    glm_vec3_cross(cam_front, cam_up, movement);
    glm_vec3_scale_as(movement, cam_speed, movement);
    glm_vec3_add(movement, cam_pos, cam_pos);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    glm_vec3_cross(cam_front, cam_up, movement);
    glm_vec3_scale_as(movement, -cam_speed, movement);
    glm_vec3_add(movement, cam_pos, cam_pos);
  }
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    vec3 temp = {0.0, cam_speed, 0.0};
    glm_vec3_add(temp, cam_pos, cam_pos);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    vec3 temp = {0.0, cam_speed, 0.0};
    glm_vec3_sub(cam_pos, temp, cam_pos);
  }
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }

   // KEY CAMERA CONTROL
   // LOOK UP
  if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
    pitch += cam_speed * 50.0;

    if (pitch > 89.0f) {
      pitch = 89.0f;
    } else if (pitch < -89.0f) {
      pitch = -89.0f;
    }

    cam_front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    cam_front[1] = sin(glm_rad(pitch));
    cam_front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    glm_vec3_normalize(cam_front);

    vec3 world_up = { 0.0, 1.0, 0.0 };
    glm_vec3_cross(cam_front, world_up, cam_up);
    glm_vec3_cross(cam_up, cam_front, cam_up);
    glm_vec3_normalize(cam_front);
  }
  // LOOK DOWN
  if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
    pitch -= cam_speed * 50.0;

    if (pitch > 89.0f) {
      pitch = 89.0f;
    } else if (pitch < -89.0f) {
      pitch = -89.0f;
    }

    cam_front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    cam_front[1] = sin(glm_rad(pitch));
    cam_front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    glm_vec3_normalize(cam_front);

    vec3 world_up = { 0.0, 1.0, 0.0 };
    glm_vec3_cross(cam_front, world_up, cam_up);
    glm_vec3_cross(cam_up, cam_front, cam_up);
    glm_vec3_normalize(cam_front);
  }
  // LOOK LEFT
  if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
    yaw -= cam_speed * 50.0;

    cam_front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    cam_front[1] = sin(glm_rad(pitch));
    cam_front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    glm_vec3_normalize(cam_front);

    vec3 world_up = { 0.0, 1.0, 0.0 };
    glm_vec3_cross(cam_front, world_up, cam_up);
    glm_vec3_cross(cam_up, cam_front, cam_up);
    glm_vec3_normalize(cam_front);
  }
  // LOOK RIGHT
  if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
    yaw += cam_speed * 50.0;

    cam_front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    cam_front[1] = sin(glm_rad(pitch));
    cam_front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    glm_vec3_normalize(cam_front);

    vec3 world_up = { 0.0, 1.0, 0.0 };
    glm_vec3_cross(cam_front, world_up, cam_up);
    glm_vec3_cross(cam_up, cam_front, cam_up);
    glm_vec3_normalize(cam_front);
  }



  if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) && (pressed == 0)) {
    if (active_board == 0) {
      active_board = 2;
    } else {
      active_board--;
    }
    pressed = 1;
  } else if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && (pressed == 0)) {
    active_board = (active_board + 1) % 3;
    pressed = 1;
  } else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_PRESS &&
             glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_PRESS) {
    pressed = 0;
  }
}

void mouse_input(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = 0;
  }

  float xOffset = xpos -lastX;
  float yOffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  const float sensitivity = 0.1f;
  xOffset *= sensitivity;
  yOffset *= sensitivity;

  yaw += xOffset;
  pitch += yOffset;

  if (pitch > 89.0f) {
    pitch = 89.0f;
  } else if (pitch < -89.0f) {
    pitch = -89.0f;
  }

  cam_front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
  cam_front[1] = sin(glm_rad(pitch));
  cam_front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
  glm_vec3_normalize(cam_front);

  vec3 world_up = { 0.0, 1.0, 0.0 };
  glm_vec3_cross(cam_front, world_up, cam_up);
  glm_vec3_cross(cam_up, cam_front, cam_up);
  glm_vec3_normalize(cam_front);
}
