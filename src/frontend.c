#include <psuedo_legal_moves.h>
#include <frontend.h>

// GRAPHICS MECHANICS
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

int pressed = 0;
unsigned int active_board = 1;

// GAME MECHANICS
BOARD_ARGS game;
int piece = 0;
unsigned int from[2] = {0,0};
unsigned int to[2] = {0,0};
int to_move = 0;
pthread_t g;
int change_ready = 0;
int turn = 0;

void play() {
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
      change_ready = 1;
    } else {
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
      change_ready = 0;
    }
  }
}

int main() {
  init_pieceboard(game);
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
    { 0.0, 0.0, -0.9 }, // KING
    { 0.0, 0.0, -1.2 }, // QUEEN
    { 0.0, 0.0, -0.6 }, // BISHOP
    { 0.0, 0.0, -1.5 }, // BISHOP
    { 0.0, 0.0, 0.0 }, // ROOK
    { 0.0, 0.0, -2.1 }, // ROOK
    { 0.0, 0.0, -0.3 }, // KNIGHT
    { 0.0, 0.0, -1.8 }, // KNIGHT
    { -0.3, 0.0, 0.0 }, // PAWN
    { -0.3, 0.0, -0.3 },
    { -0.3, 0.0, -0.6 },
    { -0.3, 0.0, -0.9 },
    { -0.3, 0.0, -1.2 },
    { -0.3, 0.0, -1.5 },
    { -0.3, 0.0, -1.8 },
    { -0.3, 0.0, -2.1 },
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
    { -1.8, 0.0, 0.0 }, // PAWN
    { -1.8, 0.0, -0.3 },
    { -1.8, 0.0, -0.6 },
    { -1.8, 0.0, -0.9 },
    { -1.8, 0.0, -1.2 },
    { -1.8, 0.0, -1.5 },
    { -1.8, 0.0, -1.8 },
    { -1.8, 0.0, -2.1 },
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
  while (!glfwWindowShouldClose(window)) {
    /*
    if (change_ready == 1) {
      change_ready = 0;
      
    }*/
    float curFrame = glfwGetTime();
    deltaTime = curFrame - lastFrame;
    lastFrame = curFrame;

    keyboard_input(window);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUniform3f(glGetUniformLocation(piece_shader, "col"), 0.0, 0.75, 1.0);
    for (int i = 0; i < 16; i++) {
      glm_vec3_scale(white[i]->scale, outline_scale, white[i]->scale);
      draw_entity(piece_shader, white[i]);
    }
    glUniform3f(glGetUniformLocation(piece_shader, "col"), 1.0, 1.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (int i = 0; i < 16; i++) {
      if (i < 8) {
        glm_vec3_copy(piece_scale, white[i]->scale);
      } else {
        glm_vec3_copy(pawn_scale, white[i]->scale);
      }
      draw_entity(piece_shader, white[i]);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUniform3f(glGetUniformLocation(piece_shader, "col"), 1.0, 119.0/255.0, 0.0);
    for (int i = 0; i < 16; i++) {
      glm_vec3_scale(black[i]->scale, outline_scale, black[i]->scale);
      draw_entity(piece_shader, black[i]);
    }
    glUniform3f(glGetUniformLocation(piece_shader, "col"), 0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (int i = 0; i < 16; i++) {
      if (i < 8) {
        glm_vec3_copy(piece_scale, black[i]->scale);
      } else {
        glm_vec3_copy(pawn_scale, black[i]->scale);
      }
      draw_entity(piece_shader, black[i]);
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
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
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
