#include <search.h>
// SEARCH.H GLOBALS
MOVE move_list[MAX_NUM_LEGAL_MOVES];
pthread_t threads[MAX_NUM_LEGAL_MOVES];
MOVE parent_move_list[MAX_NUM_LEGAL_MOVES];
pthread_mutex_t lock;

/*
  ORDER OF EVENTS:
  -> Determine which side it is to move
  -> get legal moves for position
  -> Iterate over all pieces, starting with the king, and
     move to all of their legal moves, checking for mate,
     double check and check.
     -> To iterate over legals (each piece)
        -> iterate over levels
          -> iterate over each space of each level
  -> Once depth or checkmate has been hit, return the eval
     up the tree to be used in choosing the best move
*/

/*
* Title: print_moves()
* Params:
* NONE
* Description:
* Helper function to print out the move information
* in a more readable format
*/

void print_moves() {
  MOVE cur;
  MOVE parent_cur;
  for (int i = 0; i < MAX_NUM_LEGAL_MOVES; i++) {
    cur = move_list[i];
    parent_cur = parent_move_list[i];
    if (cur.rating != 0) {
      printf("INDEX NUMBER: %d\n", i);
      printf("SEARCH RETURNED MOVE:\nTO MOVE: %d\nFROM[0]: "
              "%d\nFROM[1]: %d\nTO[0]: %d\nTO[1]: %d\nSCORE: %d\nRATING: %u\n",
              cur.to_move, cur.from[0], cur.from[1], cur.to[0], cur.to[1],
              cur.score, cur.rating);
      printf("PARENT MOVE:\nTO MOVE: %d\nFROM[0]: "
              "%d\nFROM[1]: %d\nTO[0]: %d\nTO[1]: %d\nSCORE: %d\nRATING: %u\n\n",
              parent_cur.to_move, parent_cur.from[0], parent_cur.from[1],
              parent_cur.to[0], parent_cur.to[1], parent_cur.score,
              parent_cur.rating);
    }
  }
}

/*
* Title: level_zero_search()
* Params:
* -> BOARD_ARGS *args
*   -> pointer to a stuct which holds bitboards
*      which detail the current board position
* -> unsigned int turn
*   -> simple integer to note which move number
*      the game is on
* -> SIDE to_move
*   -> Enum to note which player's turn it is
*      to move
* -> unsigned int depth
*   -> non-negative number that represents how many
*      levels are left to search in the minimax
*      algorithm
* -> int alpha
*   -> Alpha-Beta pruning number for the maximizing
*      player
* -> int beta
*   -> Alpha-Beta pruning number for the minimizing
*      player
* Description:
* Start of the multi-core minimax algorithm. Currently
* in a state of disarray since there is issues with
* resolving the best move returned by all of the threads.
* Might be expanded upon later. Spawns threads which use
* other helper functions to reach the actual minimax
* search function
*/

MOVE level_zero_search(BOARD_ARGS *args, unsigned int turn, SIDE to_move,
                       unsigned int depth, int alpha,
                       int beta) {
  SIDE enemy = to_move == WHITE ? BLACK : WHITE;
  uint32_t enemy_flags = 0;
  uint32_t player_flags = 0;
  uint64_t junk[3];
  unsigned int current_position[2];
  unsigned int to_position[2];
  uint64_t legals[3];
  S_INFO info;
  BOARD_ARGS copy;
  MOVE current;
  current.to[0] = 0;
  current.to[1] = 0;
  current.from[0] = 0;
  current.from[1] = 0;
  int thread_num = -1;
  get_legal(enemy, args->k_pos[enemy], KING, *args, junk, &enemy_flags);
  get_legal(to_move, args->k_pos[to_move], KING, *args, junk, &player_flags);
  if (player_flags & MATE) {
    current.score = -1;
    return current;
  } else if (enemy_flags & MATE) {
    current.score = -2;
    return current;
  } else {
    for (int i = 0; i < PIECE_TYPES; i++) {
        /*
          SELECT PIECE TYPE
        */
        for (int j = 0; j < NUM_LEVELS; j++) {
          /*
            SELECT LEVEL
          */
          current_position[0] = j;
          /*
            FIND MOST SIG BIT OF PIECES BOARD, THEN REMOVE FROM BITBOARD
          */
          uint64_t t_piece_board = args->piece_boards[to_move][i][j];
          while (t_piece_board) {
            unsigned int ms_bit[2] = {j, ms_bit_lookup(&t_piece_board)};
            current_position[1] = ms_bit[1];
            current.from[0] = current_position[0];
            current.from[1] = current_position[1];
            get_legal(to_move, ms_bit, i, *args, legals, &player_flags);
            for (int k = 0; k < NUM_LEVELS; k++) {
            /*
              SELECT LEGAL MOVE
            */
            to_position[0] = k;
            /*
              GET SIG BIT FOR LEGAL MOVES BIT BOARD, THEN MAKE ALL MOVES
            */
            uint64_t t_legal_board = legals[k];
            while (t_legal_board) {
              unsigned int ms_bit_poss_legal[2] = {k, ms_bit_lookup(&t_legal_board)};
              to_position[1] = ms_bit_poss_legal[1];
              current.to[0] = to_position[0];
              current.to[1] = to_position[1];
              make_temp_copy(args, &copy);
              make_move(&copy, to_move, i, ms_bit, ms_bit_poss_legal);
              info.args = &copy;
              info.to_move = enemy;
              info.depth = depth - 1;
              info.alpha = alpha;
              info.beta = beta;
              info.turn = turn;
              thread_num++;
              info.id = thread_num;
              parent_move_list[thread_num].to[0] = current.to[0];
              parent_move_list[thread_num].to[1] = current.to[1];
              parent_move_list[thread_num].from[0] = current.from[0];
              parent_move_list[thread_num].from[1] = current.from[1];
              parent_move_list[thread_num].args = &copy;
              pthread_create(threads + thread_num, NULL,
                              (void *) s_th_wrapper, (void *) &info);
            }
          }
        }
      }
    }
  }
  printf("\n\n%d threads spawned\n", thread_num + 1);
  int j = -1;

  while (j < thread_num) {
    j++;
    int s = pthread_join(threads[j], NULL);
    if (s != 0) {
       printf("ERROR DETECTED IN THREAD %d | %d\n", j, s);
    }
    printf("Thread %d joined with code %d\n", j, s);
  }
  printf("\nLISTING ALL MOVES:\n\n");
  print_moves();
  // FIND BEST MOVE
  MOVE best;
  MOVE cur;
  if (to_move == WHITE) {
    best.score = INT_MIN;
  } else {
    best.score = INT_MAX;
  }
  best.to[0] = 0;
  best.to[1] = 0;
  best.from[0] = 0;
  best.from[1] = 0;
  best.rating = 0;
  int best_index = 0;
  while (thread_num > -1) {
    cur = move_list[thread_num];
    if ((to_move == WHITE && cur.score >= best.score) ||
       (to_move == BLACK && cur.score <= best.score )) {
      if (cur.score == INT_MIN || cur.score == INT_MAX) {
        best.score = cur.score;
        best.from[0] = cur.from[0];
        best.from[1] = cur.from[1];
        best.to[0] = cur.to[0];
        best.to[1] = cur.to[1];
        best.rating = cur.rating;
        best_index = thread_num;
      } else if (cur.rating > best.rating) {
        best.score = cur.score;
        best.from[0] = cur.from[0];
        best.from[1] = cur.from[1];
        best.to[0] = cur.to[0];
        best.to[1] = cur.to[1];
        best.rating = cur.rating;
        best_index = thread_num;
      }
    }
    printf("MOVE CHOSEN AS BEST: %d\n", best_index);
    thread_num--;
  }

  MOVE b_ret = evaluate(parent_move_list[best_index].args, to_move,
                        player_flags, enemy_flags, turn);
  b_ret.args = parent_move_list[best_index].args;
  b_ret.from[0] = parent_move_list[best_index].from[0];
  b_ret.from[1] = parent_move_list[best_index].from[1];
  b_ret.to[0] = parent_move_list[best_index].to[0];
  b_ret.to[1] = parent_move_list[best_index].to[1];
  b_ret.to_move = to_move;
  return b_ret;
}

/*
* Title: s_th_wrapper()
* Params:
* -> void *arg
*   -> pointer to a struct containing
*      info that the thread uses to know
*      more details about what move
*      combination it is to execute on.
*      Generally, just a medium of transferring
*      data when the thread is spawned
* Description:
* Helper function to have the threads call the correct
* arguments with the minimax function
*/

void s_th_wrapper(void *arg) {
  S_INFO *args = (S_INFO *) arg;
  printf("spawning thread with id: %d\n", args->id);
  move_list[args->id] = search(args->args, args->to_move, args->depth,
                         args->alpha, args->beta, args->turn);
  printf("hit with %d\n", args->id);
  pthread_exit(0);
}

/*
* Title: search()
* Params:
* -> BOARD_ARGS *args
*   -> pointer to a struct which contains
*      bitboards which describe the current
*      board position
* -> SIDE to_move
*   -> Enum which represents which one of
*      the player's turns it is to move
* -> unsigned int depth
*   -> non-negative number to represent how
*      many levels are left for the algorithm
*      to search
* -> int alpha
*   -> Alpha-beta number for the maximizing
*      player
* -> int beta
*   -> Alpha-beta number for the minimizing
*      player
* -> unsigned int turn
*   -> Number to represent the turn number
*      for the game
* Description:
* Main minimax search function to determine
* the best move for the computer to play.
* Searches all legal move paths recursively
* then returns the best moves found from that
* branch, eventually narrowing down to the best
* move.
*/

unsigned long long stuff = 0;
MOVE search (BOARD_ARGS *args, SIDE to_move, unsigned int depth,
            int alpha, int beta, unsigned int turn) {
  SIDE enemy = to_move == WHITE ? BLACK : WHITE;
  uint32_t player_flags = 0;
  uint32_t enemy_flags = 0;
  MOVE best;
  MOVE current;
  if (to_move == WHITE) {
    best.score = INT_MIN;
  } else {
    best.score = INT_MAX;
  }
  best.args = args;
  best.to_move = to_move;
  best.to[0] = 0xBAADF00D;
  best.to[1] = 0xBAADF00D;
  best.from[0] = 0xBAADF00D;
  best.from[1] = 0xBAADF00D;
  best.rating = 0;
  unsigned int current_position[2];
  unsigned int to_position[2];
  uint64_t junk[3];
  /*
    BREAKS WHEN KING BEING EVALUATED ON BLACK AFTER MOVING TO TOP LEVEL
  */
  get_legal(enemy, args->k_pos[enemy], KING, *args, junk, &enemy_flags);
  for (int i = 0; i < PIECE_TYPES; i++) {
    /*
      SELECT PIECE TYPE
    */
    for (int j = 0; j < NUM_LEVELS; j++) {
      /*
        SELECT LEVEL
      */
      current_position[0] = j;
      /*
        FIND MOST SIG BIT OF PIECES BOARD, THEN REMOVE FROM BITBOARD
      */
      uint64_t t_piece_board = args->piece_boards[to_move][i][j];
      while (t_piece_board) {
        unsigned int ms_bit[2] = {j, ms_bit_lookup(&t_piece_board)};
        current_position[1] = ms_bit[1];
        current.from[0] = current_position[0];
        current.from[1] = current_position[1];
        uint64_t legals[3];
        get_legal(to_move, ms_bit, i, *args, legals, &player_flags);
        if (depth == 0) {
          return evaluate(args, to_move, player_flags, enemy_flags, turn);
        } else if ((player_flags & MATE) == 1 || (enemy_flags & MATE) == 1) {
          current = evaluate(args, to_move, player_flags, enemy_flags, turn);
          if ((to_move == WHITE && current.score >= best.score) ||
              (to_move == BLACK && current.score <= best.score)) {
              best.score = current.score;
              best.from[0] = current_position[0];
              best.from[1] = current_position[1];
              best.to[0] = to_position[0];
              best.to[1] = to_position[1];
              best.rating = current.rating;
          }
        } else {
          for (int k = 0; k < NUM_LEVELS; k++) {
            /*
              SELECT LEGAL MOVE
            */
            to_position[0] = k;
            /*
              GET SIG BIT FOR LEGAL MOVES BIT BOARD, THEN MAKE ALL MOVES
            */
            uint64_t t_legal_board = legals[k];
            while (t_legal_board) {
              unsigned int ms_bit_poss_legal[2] = {k, ms_bit_lookup(&t_legal_board)};
              to_position[1] = ms_bit_poss_legal[1];
              current.to[0] = to_position[0];
              current.to[1] = to_position[1];
              BOARD_ARGS copy;
              make_temp_copy(args, &copy);
              make_move(&copy, to_move, i, ms_bit, ms_bit_poss_legal);
              pthread_mutex_lock(&lock);
              //stuff++;
              pthread_mutex_unlock(&lock);
              current = search(&copy, enemy, depth - 1, alpha, beta, turn + 1);
              if ((to_move == WHITE && current.score >= best.score) ||
                (to_move == BLACK && current.score <= best.score )) {
                if ((enemy_flags & MATE)) {
                  best.score = current.score;
                  best.from[0] = current_position[0];
                  best.from[1] = current_position[1];
                  best.to[0] = to_position[0];
                  best.to[1] = to_position[1];
                  best.rating = INT_MAX;
                } else if (current.rating > best.rating) {
                  best.score = current.score;
                  best.from[0] = current_position[0];
                  best.from[1] = current_position[1];
                  best.to[0] = to_position[0];
                  best.to[1] = to_position[1];
                  best.rating = current.rating;
                } else if ((enemy_flags & CHECK) || (enemy_flags & D_CHECK)) {
                  best.score = current.score;
                  best.from[0] = current_position[0];
                  best.from[1] = current_position[1];
                  best.to[0] = to_position[0];
                  best.to[1] = to_position[1];
                  best.rating = current.rating * 10;
                }
              }
                // ALPHA-BETA PRUNING
                /*
                  IF MAXIMIZING PLAYER (WHITE):
                    ALPHA = MAX(ALPHA, EVAL)
                    IF BETA <= ALPHA:
                      BREAK
                  ELSE:
                    BETA = MIN(BETA, EVAL)
                    IF BETA <= ALPHA:
                      BREAK
                */
               #if 0
               if (to_move == WHITE) {
                  // WHITE = 0; BLACK = 1
                  if (player_flags & MATE) {
                    alpha = get_max_min(to_move, alpha,
                            ((best.score * 5) + best.rating));
                  } else {
                    alpha = get_max_min(to_move, alpha, best.score);
                  }
                } else {
                  if (player_flags & MATE) {
                    beta = get_max_min(to_move, beta,
                            ((best.score * 5) - best.rating));
                  } else {
                    beta = get_max_min(to_move, beta, best.score);
                  }
                }
                if (beta <= alpha) {
                  break;
                }
              #endif
                if (to_move == WHITE) {
                  // WHITE = 0; BLACK = 1
                  if (player_flags & MATE) {
                    alpha = get_max_min(to_move, alpha,
                            ((best.score * 5)));
                  } else {
                    alpha = get_max_min(to_move, alpha, best.score);
                  }
                } else {
                  if (player_flags & MATE) {
                    beta = get_max_min(to_move, beta,
                            ((best.score * 5)));
                  } else {
                    beta = get_max_min(to_move, beta, best.score);
                  }
                }
                if (beta <= alpha) {
                  break;
                }
              }
            }
          }
        }
      }
    }
  return best;
}

/*
* Title: make_move()
* Params:
* -> BOARD_ARGS *args
*   -> pointer to a struct which contains
*      bitboards which describe the current
*      board position
* -> SIDE to_move
*   -> Enum which represents which one of
*      the player's turns it is to move
* -> TYPE p_type
*   -> Enum to represent which piece type
*      is being moved
* -> unsigned int *from
*   -> pair of values to represent the position
*      to which the piece started from
*      {LEVEL, BITPOSITION}
* -> unsigned int *to
*   -> pair of values to represent the position
*      to which the piece ended at
*      {LEVEL, BITPOSITION}
* Description:
* Helper function to move the pieces on the
* bitboard and adjust the requisite information
* accordingly based on the move
*/

void make_move(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
                     unsigned int *from, unsigned int *to) {
  /*
    To/From : {Level, Bitposition}
  */

  SIDE enemy_t = to_move == WHITE ? BLACK : WHITE;
  uint64_t old_pos = LEVELS[from[1]];
  uint64_t new_pos = LEVELS[to[1]];

  args->boards[to_move][from[0]] ^= old_pos;
  args->piece_boards[to_move][p_type][from[0]] ^= old_pos;
  args->boards[to_move][to[0]] |= new_pos;
  args->piece_boards[to_move][p_type][to[0]] |= new_pos;

  if (p_type == ROOK || p_type == QUEEN) {
    args->hv_sliders[to_move][from[0]] ^= old_pos;
    args->hv_sliders[to_move][to[0]] |= new_pos;
  }
  if (p_type == BISHOP || p_type == QUEEN) {
    args->d_sliders[to_move][from[0]] ^= old_pos;
    args->d_sliders[to_move][to[0]] |= new_pos;
  }
  if (p_type == KING) {
    args->k_pos[to_move][0] = to[0];
    args->k_pos[to_move][1] = to[1];
  }

  if (new_pos & args->boards[enemy_t][to[0]]) {
    args->boards[enemy_t][to[0]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][BISHOP][to[0]]) {
    args->piece_boards[enemy_t][BISHOP][to[0]] ^= new_pos;
    args->d_sliders[enemy_t][to[0]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][ROOK][to[0]]) {
    args->piece_boards[enemy_t][ROOK][to[0]] ^= new_pos;
    args->hv_sliders[enemy_t][to[0]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][QUEEN][to[0]]) {
    args->piece_boards[enemy_t][QUEEN][to[0]] ^= new_pos;
    args->hv_sliders[enemy_t][to[0]] ^= new_pos;
    args->d_sliders[enemy_t][to[0]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][PAWN][to[0]]) {
    args->piece_boards[enemy_t][PAWN][to[0]] ^= new_pos;
  }
  if (new_pos & args->piece_boards[enemy_t][KNIGHT][to[0]]) {
    args->piece_boards[enemy_t][KNIGHT][to[0]] ^= new_pos;
  }
}

/*
* Title: ms_bit_lookup()
* Params:
* -> uint64_t *input
*   -> pointer to a bitboard where
*      the most significant bit position
*      is looked up using bitwise operations
* Description:
* Simple helper function to determine what the
* decimal place value was associated with the
* most significant bit
*/

int ms_bit_lookup(uint64_t *input) {
  int ms_bit_poss = log2_lookup(*input);
  *input &= ~(ONE << ms_bit_poss);
  return ms_bit_poss;
}

/*
* Title: make_temp_copy()
* Params:
* -> BOARD_ARGS *source
*   -> pointer to a struct of bitboards
*      which represents the board state
*      before a move it to be made
* -> BOARD_ARGS *dest
*   -> pointer to a struct of bitboards
*      which represents the board state
*      after the move it made
* Description:
* helper function to assist in not clobbering
* the data stored in the BOARD_ARGS struct between
* calls to the next level of depth in search()
*/

void make_temp_copy(BOARD_ARGS *source, BOARD_ARGS *dest) {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
    dest->boards[i][j] = source->boards[i][j];
    }
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 6; j++) {
      for (int k = 0; k < 3; k++) {
        dest->piece_boards[i][j][k] = source->piece_boards[i][j][k];
      }
    }
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
    dest->hv_sliders[i][j] = source->hv_sliders[i][j];
    }
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
    dest->d_sliders[i][j] = source->d_sliders[i][j];
    }
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
    dest->k_pos[i][j] = source->k_pos[i][j];
    }
  }
}

/*
* Title: get_max_min()
* Params:
* -> int is_max
*   -> Simple integer (1 or 0) to denote
*      if the player is white or black
* -> int one
*   -> first number in the comparison
* -> int two
*   -> second number in the comparison
* Description:
* Simple helper to determine which number
* is higher for Alpha-Beta pruning
*/

int get_max_min(int is_max, int one, int two) {
  if (!is_max) {
    // FINDING FOR WHITE
    if (one > two)
      return one;
    else
      return two;
  } else {
    // FINDING FOR BLACK
    if (one < two)
      return one;
    else
      return two;
  }
}

