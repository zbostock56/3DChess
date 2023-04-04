#include <psuedo_legal_moves.h>
#include <frontend.h>
#include <string.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <entity_str.h>

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

#if 0
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
#endif

#if 1
#include <glad/glad.h>
#include <glfw3.h>
#define DIR "/home/zbostock/Projects/3DChess"

void keyboard_input(GLFWwindow *window);
void mouse_input(GLFWwindow *widnow, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow *, int, int);

vec3 col_point = { 0.0, 0.0, 0.0 };
int enable_gravity;

vec3 cam_front = { 0.0, 0.0, 1.0 };
vec3 cam_pos = { 0.0, 0.0, 0.0 };
vec3 cam_up = { 0.0, 1.0, 0.0 };
float pitch = 0.0;
float yaw = 0.0;
int firstMouse = 1;

float deltaTime = 0.0;
float lastFrame = 0.0;
double lastX;
double lastY;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "ZACK IS LAME\n", NULL, NULL);
  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, mouse_input);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  unsigned int shader = init_shader_prog(
    DIR"/src/shaders/shader.vs",
    NULL,
    DIR"/src/shaders/shader.fs"
  );
  if (shader == -1) {
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
  ENTITY *board_ent = init_entity(board);
  vec3 board_pos = { -1.05, 0.0, -1.05 };
  glm_vec3_copy(piece_scale, board_ent->scale);
  glm_vec3_copy(board_pos, board_ent->translation);

  ENTITY *king_ent = init_entity(king);
  vec3 king_pos = { 0.0, 0.0, -0.9 };
  glm_vec3_copy(king_pos, king_ent->translation);
  glm_vec3_copy(piece_scale, king_ent->scale);

  ENTITY *queen_ent = init_entity(queen);
  vec3 queen_pos = { 0.0, 0.0, -1.2 };
  glm_vec3_copy(queen_pos, queen_ent->translation);
  glm_vec3_copy(piece_scale, queen_ent->scale);

  ENTITY *bishop_ent = init_entity(bishop);
  vec3 bishop_pos = { 0.0, 0.0, -0.6 };
  glm_vec3_copy(bishop_pos, bishop_ent->translation);
  glm_vec3_copy(piece_scale, bishop_ent->scale);

  ENTITY *bishop2_ent = init_entity(bishop);
  vec3 bishop2_pos = { 0.0, 0.0, -1.5 };
  glm_vec3_copy(bishop2_pos, bishop2_ent->translation);
  glm_vec3_copy(piece_scale, bishop2_ent->scale);

  ENTITY *rook_ent = init_entity(rook);
  vec3 rook_pos = { 0.0, 0.0, 0.0 };
  glm_vec3_copy(rook_pos, rook_ent->translation);
  glm_vec3_copy(piece_scale, rook_ent->scale);

  ENTITY *rook2_ent = init_entity(rook);
  vec3 rook2_pos = { 0.0, 0.0, -2.1 };
  glm_vec3_copy(rook2_pos, rook2_ent->translation);
  glm_vec3_copy(piece_scale, rook2_ent->scale);

  ENTITY *knight_ent = init_entity(knight);
  vec3 knight_pos = { 0.0, 0.0, -0.3 };
  glm_vec3_copy(knight_pos, knight_ent->translation);
  glm_vec3_copy(piece_scale, knight_ent->scale);

  ENTITY *knight2_ent = init_entity(knight);
  vec3 knight2_pos = { 0.0, 0.0, -1.8 };
  glm_vec3_copy(knight2_pos, knight2_ent->translation);
  glm_vec3_copy(piece_scale, knight2_ent->scale);

  ENTITY *p1_ent = init_entity(pawn);
  vec3 p1_pos = { -0.3, 0.0, 0.0 };
  glm_vec3_copy(p1_pos, p1_ent->translation);
  glm_vec3_copy(piece_scale, p1_ent->scale);

  ENTITY *p2_ent = init_entity(pawn);
  vec3 p2_pos = { -0.3, 0.0, -0.3 };
  glm_vec3_copy(p2_pos, p2_ent->translation);
  glm_vec3_copy(piece_scale, p2_ent->scale);

  ENTITY *p3_ent = init_entity(pawn);
  vec3 p3_pos = { -0.3, 0.0, -0.6 };
  glm_vec3_copy(p3_pos, p3_ent->translation);
  glm_vec3_copy(piece_scale, p3_ent->scale);

  ENTITY *p4_ent = init_entity(pawn);
  vec3 p4_pos = { -0.3, 0.0, -0.9 };
  glm_vec3_copy(p4_pos, p4_ent->translation);
  glm_vec3_copy(piece_scale, p4_ent->scale);

  ENTITY *p5_ent = init_entity(pawn);
  vec3 p5_pos = { -0.3, 0.0, -1.2 };
  glm_vec3_copy(p5_pos, p5_ent->translation);
  glm_vec3_copy(piece_scale, p5_ent->scale);

  ENTITY *p6_ent = init_entity(pawn);
  vec3 p6_pos = { -0.3, 0.0, -1.5 };
  glm_vec3_copy(p6_pos, p6_ent->translation);
  glm_vec3_copy(piece_scale, p6_ent->scale);

  ENTITY *p7_ent = init_entity(pawn);
  vec3 p7_pos = { -0.3, 0.0, -1.8 };
  glm_vec3_copy(p7_pos, p7_ent->translation);
  glm_vec3_copy(piece_scale, p7_ent->scale);

  ENTITY *p8_ent = init_entity(pawn);
  vec3 p8_pos = { -0.3, 0.0, -2.1 };
  glm_vec3_copy(p8_pos, p8_ent->translation);
  glm_vec3_copy(piece_scale, p8_ent->scale);

  mat4 model = GLM_MAT4_IDENTITY_INIT;
  mat4 view = GLM_MAT4_IDENTITY_INIT;
  mat4 proj;
  glm_perspective(glm_rad(45.0), 800.0/600.0, 0.1, 100.0, proj);
  /*printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
         proj[0][0], proj[0][1], proj[0][2], proj[0][3],
         proj[1][0], proj[1][1], proj[1][2], proj[1][3],
         proj[2][0], proj[2][1], proj[2][2], proj[2][3],
         proj[3][0], proj[3][1], proj[3][2], proj[3][3]);*/
  glUseProgram(shader);
  printf("proj: %d\n", glGetUniformLocation(shader, "projection"));
  printf("view: %d\n", glGetUniformLocation(shader, "view"));
  printf("model: %d\n", glGetUniformLocation(shader, "model"));
  fflush(stdout);
  glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1,
                     GL_FALSE, (float *) proj);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  while (!glfwWindowShouldClose(window)) {
    float curFrame = glfwGetTime();
    deltaTime = curFrame - lastFrame;
    lastFrame = curFrame;

    keyboard_input(window);

    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glUseProgram(shader);

    glm_look(cam_pos, cam_front, cam_up, view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1,
                       GL_FALSE, (float *) view);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1,
                       GL_FALSE, (float *) model);
    draw_entity(shader, board_ent);
    draw_entity(shader, king_ent);
    draw_entity(shader, queen_ent);
    draw_entity(shader, bishop_ent);
    draw_entity(shader, bishop2_ent);
    draw_entity(shader, rook_ent);
    draw_entity(shader, rook2_ent);
    draw_entity(shader, knight_ent);
    draw_entity(shader, knight2_ent);
    draw_entity(shader, p1_ent);
    draw_entity(shader, p2_ent);
    draw_entity(shader, p3_ent);
    draw_entity(shader, p4_ent);
    draw_entity(shader, p5_ent);
    draw_entity(shader, p6_ent);
    draw_entity(shader, p7_ent);
    draw_entity(shader, p8_ent);

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
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
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

#endif
