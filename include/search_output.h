#ifndef search_output_h
  #define search_output_h
  #include <time.h>
  #include <sys/time.h>
  //#ifndef log2_h
  //  #include <log2.h>
  //#endif
  #ifndef legal_h
    #include <legal_moves.h>
  #endif
  #ifndef search_h
    #include <search.h>
  #endif
  #ifndef main_h
    #include <main.h>
  #endif

  // SEARCH_OUTPUT.H FILES
  void search_output(BOARD_ARGS *args, SIDE to_move);
  void translate_position(BOARD_ARGS *args, int arr_pos);
  void reset_possible_moves();
  void output_to_file(BOARD_ARGS *args, SIDE to_move);
  void write_to_file(int result, FILE *samples, FILE *labels,
                     FILE *all, FILE *chess_poss, FILE *from_move_only);
  void print_game(BOARD_ARGS *args);
  unsigned int random_val_gen();

  #ifndef log2_h
    unsigned int log2_lookup(uint64_t);
  #endif
  #ifndef search_h
    int ms_bit_lookup(uint64_t *input);
    void make_move(BOARD_ARGS *args, SIDE to_move, TYPE p_type,
                   unsigned int *from, unsigned int *to);
  #endif
  #ifndef legal_h
    void get_legal(SIDE player, unsigned int *pos, TYPE type,
                   BOARD_ARGS args, uint64_t *output, uint32_t *flags);
  #endif
#endif
