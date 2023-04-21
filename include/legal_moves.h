#ifndef legal_h
  #define legal_h
  #ifndef search_h
    #include <search.h>
  #endif
  //#ifndef frontend_h
  //  #include <frontend.h>
  //#endif
  #ifndef main_h
    #include <main.h>
  #endif

  // LEGAL_MOVES.H FUNCTIONS
  void get_legal(SIDE player, unsigned int *pos, TYPE type,
                 BOARD_ARGS args, uint64_t *output, uint32_t *flags);
  void calc_sa(TYPE type, SIDE enemy_t, uint64_t (*boards)[3],
                   unsigned int *pos, uint64_t *sa);
  void slider_check_detect(uint64_t *sk, uint64_t sk_xray, uint64_t *pl,
                           unsigned int *pos, uint64_t *sliders,
                           uint64_t *output, unsigned int *double_check);
  void jump_check_detect(unsigned int *pos, uint64_t pl, uint64_t *piece_board,
                         uint64_t *output, unsigned int *double_check);
  #ifndef search_h
    void make_temp_copy(BOARD_ARGS *, BOARD_ARGS *);
    void make_move(BOARD_ARGS *, SIDE, TYPE, unsigned int *, unsigned int *);
  #endif
  #ifndef frontend_h
    void printf_bitboards(uint64_t *b);
    void printf_bitboard(uint64_t b);
  #endif
#endif
