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
  //#ifndef log2_h
  //  #include <log2.h>
  //#endif
  #define MAX_KING_POS (8)

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

  #define check_level() {\
    make_temp_copy(&args, &copy);\
    make_move(&copy, player, KING, args.k_pos[player], coord);\
    calc_sa(BISHOP, enemy_t, copy.boards, coord, sa[BISHOP]);\
    calc_sa(ROOK, enemy_t, copy.boards, coord, sa[ROOK]);\
    slider_check_detect(sa[BISHOP], bishop_king, BISHOP_PL, coord, args.d_sliders[enemy_t], d_check, &double_check);\
    slider_check_detect(sa[ROOK], rook_king, ROOK_PL, coord, args.hv_sliders[enemy_t], hv_check, &double_check);\
    jump_check_detect(coord, KNIGHT_PL[pos[1]], kn_enemies, jn_check, &double_check);\
    jump_check_detect(coord, p_attack, kp_enemies, jp_check, &double_check);\
    pl_move_level_poss[TOP] = d_check[TOP] | hv_check[TOP] | jp_check[TOP] | jn_check[TOP];\
    pl_move_level_poss[MIDDLE] = d_check[MIDDLE] | hv_check[MIDDLE] | jp_check[MIDDLE] | jn_check[MIDDLE];\
    pl_move_level_poss[BOTTOM] = d_check[BOTTOM] | hv_check[BOTTOM] | jp_check[BOTTOM] | jn_check[BOTTOM];\
  }\

  #ifndef search_h
    void make_temp_copy(BOARD_ARGS *, BOARD_ARGS *);
    void make_move(BOARD_ARGS *, SIDE, TYPE, unsigned int *, unsigned int *);
  #endif
  #ifndef frontend_h
    void printf_bitboards(uint64_t *b);
    void printf_bitboard(uint64_t b);
  #endif
#endif
