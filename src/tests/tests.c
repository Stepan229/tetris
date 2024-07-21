#include "test.h"

START_TEST(tetris_base_test_1) {
  GameInfo_t info_game;
  static UserAction_t signal = -1;
  PositionFigure_t pos_fig;
  // get_UserAction(&signal);
  init_GameInfo(&info_game);
  // change_GameInfo_t(&info_game);
  info_game.pause = 0;
  static State_t state = Start_s;
  // get_State_t(&state);
  // time_t start_time = clock();
  // get_timeout(&start_time);

  if (state == Start_s)
    signal = Start;
  else {
    signal = Down;
    sigact(&state, &info_game, &signal, &pos_fig);
    signal = Down;
    sigact(&state, &info_game, &signal, &pos_fig);
    signal = Down;
    sigact(&state, &info_game, &signal, &pos_fig);
    signal = Action;
    sigact(&state, &info_game, &signal, &pos_fig);
    signal = Left;
  }
  signal = Pause;
  sigact(&state, &info_game, &signal, &pos_fig);
  signal = Pause;
  sigact(&state, &info_game, &signal, &pos_fig);
  remove_GameInfo(&info_game);
}

START_TEST(valgrind) {
  GameInfo_t info_game;
  // static UserAction_t signal = -1;
  PositionFigure_t pos_fig = {0};
  // get_UserAction(&signal);
  init_GameInfo(&info_game);
  // change_GameInfo_t(&info_game);
  info_game.pause = 0;
  // static State_t state = Start_s;
  // get_State_t(&state);
  // int break_flag = True;
  time_t start_time = clock();
  get_timeout(&start_time);
  // for (int i = 0; i < 20; i++){
  //     signal = Start;
  //     sigact(&state, &info_game, &signal, &pos_fig);
  // }
  // int **matrix = NULL;
  // create_matrix(2,4,&matrix);
  // read_file_figures(2, &matrix);
  create_figure(&info_game, &pos_fig);
  // move_down(&info_game, &pos_fig);
  // remove_matrix(&info_game.next);
  // remove_matrix(&info_game.field);
  // remove_matrix(&matrix);
  remove_GameInfo(&info_game);
}

START_TEST(tetris_base_test_2) {
  GameInfo_t info_game;
  int **board_copy;
  UserAction_t signal = Start;
  PositionFigure_t pos_fig = {0};
  init_GameInfo(&info_game);
  State_t state = Start_s;

  time_t start_time = clock();
  get_timeout(&start_time);
  state = Spawn;
  sigact(&state, &info_game, &signal, &pos_fig);
  ck_assert_int_eq(state, Moving);
  board_copy = copy_board(&info_game);
  signal = Left;
  sigact(&state, &info_game, &signal, &pos_fig);
  signal = Left;
  sigact(&state, &info_game, &signal, &pos_fig);
  signal = Left;
  sigact(&state, &info_game, &signal, &pos_fig);
  // for (int row = 0; row < 2; row++){
  //     for (int col = 0; col < 5; col++){
  //         ck_assert_int_eq(info_game.field[row][col],
  //         board_copy[row][col+2]);
  //     }
  // }
  ck_assert_int_eq(info_game.field[0][0], board_copy[0][0 + 2]);

  remove_matrix(&board_copy);
  remove_GameInfo(&info_game);
}

START_TEST(tetris_base_test_3) {
  GameInfo_t info_game;
  int **board_copy;
  static UserAction_t signal = -1;
  PositionFigure_t pos_fig = {0};
  init_GameInfo(&info_game);
  info_game.pause = 0;
  static State_t state = Start_s;
  int break_flag = True;
  time_t start_time = clock();
  get_timeout(&start_time);

  for (int i = 0; i < 3 && break_flag == True; i++) {
    if (state == Start_s) {
      state = Spawn;
      sigact(&state, &info_game, &signal, &pos_fig);
      board_copy = copy_board(&info_game);
      // check_board(&info_game_1);
    } else {
      for (int i = 0; i < 7; i++) {
        signal = Left;
        sigact(&state, &info_game, &signal, &pos_fig);
      }
    }
  }

  // for (int row = 0; row < 2; row++){
  //     for (int col = 0; col < 7; col++){
  ck_assert_int_eq(info_game.field[1][1], board_copy[1][1 + 3]);

  //     }
  // }
  remove_GameInfo(&info_game);
  remove_matrix(&board_copy);
}

START_TEST(tetris_base_test_4) {
  GameInfo_t info_game;
  int **board_copy;
  static UserAction_t signal = -1;
  PositionFigure_t pos_fig = {0};
  init_GameInfo(&info_game);
  info_game.pause = 0;
  static State_t state = Start_s;
  sigact(&state, &info_game, &signal, &pos_fig);

  time_t start_time = clock();
  get_timeout(&start_time);

  for (int i = 0; i < 3; i++) {
    if (state == Start_s) {
      state = Spawn;
      sigact(&state, &info_game, &signal, &pos_fig);
      board_copy = copy_board(&info_game);
      // check_board(&info_game_1);
    } else {
      signal = Right;
      sigact(&state, &info_game, &signal, &pos_fig);
      signal = Right;
      sigact(&state, &info_game, &signal, &pos_fig);
    }
  }
  // for (int row = 0; row < 2; row++){
  //     for (int col = 3; col < 7; col++){
  ck_assert_int_eq(info_game.field[1][1], board_copy[1][1 - 2]);

  //     }
  // }
  remove_GameInfo(&info_game);
  remove_matrix(&board_copy);
}

START_TEST(tetris_base_test_6) {
  GameInfo_t info_game;
  static UserAction_t signal = Start;
  PositionFigure_t pos_fig = {0};
  init_GameInfo(&info_game);
  info_game.pause = 0;
  static State_t state = Spawn;
  sigact(&state, &info_game, &signal, &pos_fig);

  time_t start_time = clock();
  get_timeout(&start_time);
  state = Moving;
  for (int i = 0; i < 250; i++) {
    signal = Down;
    if (state != Game_over) sigact(&state, &info_game, &signal, &pos_fig);
  }
  remove_GameInfo(&info_game);
}

START_TEST(tetris_base_test_5) {
  GameInfo_t info_game;
  int **board_copy;
  static UserAction_t signal = -1;
  PositionFigure_t pos_fig = {0};
  init_GameInfo(&info_game);
  info_game.pause = 0;
  static State_t state = Start_s;
  int break_flag = True;
  time_t start_time = clock();
  get_timeout(&start_time);

  for (int i = 0; i < 3 && break_flag == True; i++) {
    if (signal == Terminate || state == Game_over) break_flag = False;
    if (state == Start_s) {
      state = Spawn;
      sigact(&state, &info_game, &signal, &pos_fig);
      board_copy = copy_board(&info_game);
      // check_board(&info_game_1);
    } else {
      for (int i = 0; i < 7; i++) {
        signal = Right;
        sigact(&state, &info_game, &signal, &pos_fig);
      }
    }
  }
  // for (int row = 0; row < 2; row++){
  //     for (int col = 3; col < 7; col++){
  ck_assert_int_eq(info_game.field[1][1], board_copy[1][1 - 4]);

  //     }
  // }
  remove_GameInfo(&info_game);
  remove_matrix(&board_copy);
}

START_TEST(tetris_move_test_1) {
  GameInfo_t info_game;
  PositionFigure_t pos_fig;
  init_GameInfo(&info_game);
  info_game.pause = 0;
  add_figure_in_board(&info_game, &pos_fig, 3);
  for (int i = 0; i < 17; i++) move_down(&info_game, &pos_fig);

  move_down(&info_game, &pos_fig);
  ck_assert_int_eq(move_down(&info_game, &pos_fig), 1);
  remove_GameInfo(&info_game);
}

START_TEST(tetris_move_test_2) {
  GameInfo_t info_game;
  PositionFigure_t pos_fig;
  init_GameInfo(&info_game);
  info_game.pause = 0;
  add_figure_in_board(&info_game, &pos_fig, 1);
  for (int i = 0; i < 19; i++) move_down(&info_game, &pos_fig);
  attaching(&info_game);
  add_figure_in_board(&info_game, &pos_fig, 3);
  for (int i = 0; i < 17; i++) move_down(&info_game, &pos_fig);
  ck_assert_int_eq(move_down(&info_game, &pos_fig), 1);
  remove_GameInfo(&info_game);
}

START_TEST(tetris_move_test_3) {
  GameInfo_t info_game;
  PositionFigure_t pos_fig = {0};
  init_GameInfo(&info_game);
  info_game.pause = 0;
  add_figure_in_board(&info_game, &pos_fig, 1);
  for (int i = 0; i < 19; i++) move_down(&info_game, &pos_fig);
  attaching(&info_game);
  add_figure_in_board(&info_game, &pos_fig, 3);
  rotate(&info_game, &pos_fig);
  for (int i = 0; i < 17; i++) move_down(&info_game, &pos_fig);
  ck_assert_int_eq(move_down(&info_game, &pos_fig), 1);
  remove_GameInfo(&info_game);
}

START_TEST(tetris_move_test_4) {
  GameInfo_t info_game;
  PositionFigure_t pos_fig = {0};
  init_GameInfo(&info_game);
  info_game.pause = 0;
  add_figure_in_board(&info_game, &pos_fig, 1);
  move_down(&info_game, &pos_fig);
  move_down(&info_game, &pos_fig);
  move_down(&info_game, &pos_fig);
  rotate(&info_game, &pos_fig);
  rotate(&info_game, &pos_fig);
  for (int i = 0; i < 19; i++) move_down(&info_game, &pos_fig);
  attaching(&info_game);
  add_figure_in_board(&info_game, &pos_fig, 3);
  rotate(&info_game, &pos_fig);
  for (int i = 0; i < 17; i++) move_down(&info_game, &pos_fig);
  ck_assert_int_eq(move_down(&info_game, &pos_fig), 1);
  remove_GameInfo(&info_game);
}

START_TEST(tetris_move_test_5) {
  GameInfo_t info_game;
  PositionFigure_t pos_fig;
  init_GameInfo(&info_game);
  info_game.pause = 0;
  add_figure_in_board(&info_game, &pos_fig, 1);
  move_down(&info_game, &pos_fig);
  move_down(&info_game, &pos_fig);
  move_down(&info_game, &pos_fig);
  rotate(&info_game, &pos_fig);
  for (int i = 0; i < 3; i++) move_left(&info_game, &pos_fig);
  rotate(&info_game, &pos_fig);

  for (int i = 0; i < 19; i++) move_down(&info_game, &pos_fig);
  attaching(&info_game);
  add_figure_in_board(&info_game, &pos_fig, 3);
  rotate(&info_game, &pos_fig);
  for (int i = 0; i < 17; i++) move_down(&info_game, &pos_fig);
  ck_assert_int_eq(move_down(&info_game, &pos_fig), 1);
  remove_GameInfo(&info_game);
}

START_TEST(tetris_score_test_1) {
  GameInfo_t info_game;
  init_GameInfo(&info_game);
  info_game.pause = 0;
  attaching(&info_game);
  State_t state = Attaching;
  UserAction_t sig = Down;
  PositionFigure_t pos_fig = {0};
  for (int row = 17; row < 20; row++) {
    for (int col = 0; col < 10; col++) info_game.field[row][col] = 1;
  }
  sigact(&state, &info_game, &sig, &pos_fig);
  // check_board(&info_game);
  // printf("\n%d\n", info_game.score);
  ck_assert_int_eq(info_game.score, 700);
  ck_assert_int_eq(info_game.level, 2);
  sig = Terminate;
  sigact(&state, &info_game, &sig, &pos_fig);
  // remove_GameInfo(&info_game);
}

Suite *suite_tetris_base(void) {
  Suite *s = suite_create("suite_tetris_base");
  TCase *tc = tcase_create("tetris_base_tc");

  tcase_add_test(tc, valgrind);
  tcase_add_test(tc, tetris_base_test_1);
  tcase_add_test(tc, tetris_base_test_2);
  tcase_add_test(tc, tetris_base_test_3);
  tcase_add_test(tc, tetris_base_test_4);
  tcase_add_test(tc, tetris_base_test_5);
  tcase_add_test(tc, tetris_base_test_6);

  tcase_add_test(tc, tetris_move_test_1);
  tcase_add_test(tc, tetris_move_test_2);
  tcase_add_test(tc, tetris_move_test_3);
  tcase_add_test(tc, tetris_move_test_4);
  tcase_add_test(tc, tetris_move_test_5);

  tcase_add_test(tc, tetris_score_test_1);

  suite_add_tcase(s, tc);
  return s;
}

void add_figure_in_board(GameInfo_t *info_game,
                         PositionFigure_t *position_figure, int num_fig) {
  position_figure->x = 4;
  position_figure->y = 1;
  position_figure->num_fig_field = num_fig;
  int **fig_field;
  create_matrix(ROWS_FIGURE, COLS_FIGURE, &fig_field);
  read_file_figures(position_figure->num_fig_field, &fig_field);
  for (int row = 0; row < ROWS_FIGURE; row++) {
    for (int col = 0; col < COLS_FIGURE; col++) {
      info_game->field[row][col + 3] = fig_field[row][col];
    }
  }
  remove_matrix(&fig_field);
}