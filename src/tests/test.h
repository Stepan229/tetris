#ifndef S21_TETRIS_TEST_H_
#define S21_TETRIS_TEST_H_

#include <check.h>
#include <stdio.h>

#include "../brick_game/tetris/backend.h"

Suite *suite_tetris_base(void);

void run_tests(void);
void run_testcase(Suite *testcase);
void check_board(GameInfo_t *info_game);
void add_figure_in_board(GameInfo_t *info_game,
                         PositionFigure_t *position_figure, int num_fig);

#endif  // S21_TETRIS_TEST_H_