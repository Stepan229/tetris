#ifndef DEFINES_H_
#define DEFINES_H_

#include <locale.h>
#include <ncurses.h>

#include "../../brick_game/tetris/backend.h"

#define ACS_HLINE NCURSES_ACS('q')

#define BOARDS_BEGIN 2

#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)

#define RIGHT_MAP BOARDS_BEGIN + 10
#define BOTTOM_MAP BOARDS_BEGIN + 20

#define KEY_ESCAPE 27
#define KEY_SPACE 32
#define KEY_P 80
#define KEY_p 112
#define ENTER 10

#define GET_USER_INPUT getch()

#define ROWS_BOARD 20
#define COLS_BOARD 10

void print_overlay(void);
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void print_stats(GameInfo_t *stats);

GameInfo_t updateCurrentState();
GameInfo_t *change_GameInfo_t(GameInfo_t *gameInfo);
GameInfo_t *get_GameInfo_t(GameInfo_t *gameInfo);

#endif  // DEFINES_H_