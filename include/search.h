#ifndef search_h
  #define search_h
  #include <pthread.h>
  #define MAX_NUM_LEGAL_MOVES (204)

  #ifndef legal_moves_h
    #include <legal_moves.h>
  #endif
  #ifndef frontend_h
    #include <frontend.h>
  #endif
  //#ifndef evaluate_h
  //  #include <evaluate.h>
  //#endif
  #ifndef main_h
    #include <main.h>
  #endif

  // SEARCH.H FILES
  MOVE search(BOARD_ARGS *args, SIDE to_move, unsigned int depth, int alpha,
              int beta, unsigned int turn);
  void make_move(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
                 unsigned int *from, unsigned int *to);
  int ms_bit_lookup(uint64_t *input);
  int get_max_min(int is_max, int one, int two);
  MOVE level_zero_search(BOARD_ARGS *args, unsigned int turn, SIDE to_move,
                         unsigned int depth, int alpha, int beta);
  void s_th_wrapper(void *arg);
  void make_temp_copy(BOARD_ARGS *source, BOARD_ARGS *dest);
  void print_moves();

  #ifndef evaluate_h
  MOVE evaluate(BOARD_ARGS *args, SIDE to_move, uint32_t p_flags,
                uint32_t e_flags, int turn);
  #endif
  #ifndef legal_h
    void get_legal(SIDE player, unsigned int *pos, TYPE type,
                   BOARD_ARGS args, uint64_t *output, uint32_t *flags);
  #endif
  #ifndef log2_h
    unsigned int log2_lookup(uint64_t);
  #endif
  #ifndef frontend_h
    void print_game(BOARD_ARGS *args);
  #endif

  // SEARCH.H GLOBALS
  MOVE move_list[MAX_NUM_LEGAL_MOVES];
  pthread_t threads[MAX_NUM_LEGAL_MOVES];
  MOVE parent_move_list[MAX_NUM_LEGAL_MOVES];
  pthread_mutex_t lock;
#endif
