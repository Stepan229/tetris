#include "frontend.h"

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void print_overlay(void) {
  print_rectangle(1, BOTTOM_MAP, 1, RIGHT_MAP);
  print_rectangle(0, BOTTOM_MAP + 1, 0, RIGHT_MAP + 18);
  // окно со следующей фигурой
  print_rectangle(19, 22, RIGHT_MAP + 5, RIGHT_MAP + 13);
  MVPRINTW(2, RIGHT_MAP + 3, "LEVEL");
  MVPRINTW(6, RIGHT_MAP + 3, "SCORE");
  MVPRINTW(10, RIGHT_MAP + 3, "HI-SCORE");
  MVPRINTW(14, RIGHT_MAP + 3, "SPEED");
  MVPRINTW(18, RIGHT_MAP + 3, "NEXT");
  // MVADDCH(3, 5, ACS_TTEE);

  // MVADDCH(10, 10, ACS_BOARD); // Печатаем символ "@" с установленным цветом
}

void print_stats(GameInfo_t *stats) {
  MVPRINTW(3, RIGHT_MAP + 8, "%d", stats->level);
  MVPRINTW(7, RIGHT_MAP + 8, "%d", stats->score);
  MVPRINTW(11, RIGHT_MAP + 8, "%d", stats->high_score);
  MVPRINTW(15, RIGHT_MAP + 8, "%d", stats->speed);
  if (stats->next != NULL) {
    for (int row = 0; row < 2; row++) {
      for (int col = 0; col < 4; col++) {
        if (stats->next[row][col] == 1)
          MVADDCH(20 + row, 20 + col, ACS_CKBOARD);
        // MVADDCH(20 + row, 20 + col, ACS_BOARD);
        else if (stats->field[row][col] == 0)
          MVADDCH(20 + row, 20 + col, ' ');
      }
    }
  }
}

void print_pause() { MVPRINTW(2, BOARDS_BEGIN + 3, "%s", "PAUSE"); }

void print_board(GameInfo_t *board) {
  for (int row = 0; row < ROWS_BOARD; row++) {
    for (int col = 0; col < COLS_BOARD; col++) {
      if (board->field[row][col] == 1 || board->field[row][col] == 2)
        MVADDCH(BOARDS_BEGIN + row, BOARDS_BEGIN + col, ACS_CKBOARD);
      else if (board->field[row][col] == 0)
        MVADDCH(BOARDS_BEGIN + row, BOARDS_BEGIN + col, ' ');
    }
  }
}

void print_game_over() {
  MVPRINTW(2, BOARDS_BEGIN + 1, "%s", "GAME OVER");
  GET_USER_INPUT;
  halfdelay(30);
}

GameInfo_t updateCurrentState() {
  GameInfo_t *gameInfo = change_GameInfo_t(NULL);
  State_t *state = get_State_t(NULL);
  if (*state == Game_over)
    print_game_over();
  else if (gameInfo->pause == True) {
    print_pause(gameInfo);
  } else {
    print_board(gameInfo);
    print_stats(gameInfo);
  }
  return *gameInfo;
}

GameInfo_t *change_GameInfo_t(GameInfo_t *gameInfo) {
  static GameInfo_t *Info;
  if (gameInfo == NULL) return Info;
  Info = gameInfo;
  return Info;
}

GameInfo_t *get_GameInfo_t(GameInfo_t *gameInfo) {
  static GameInfo_t *Info;
  if (gameInfo == NULL) return Info;
  Info = gameInfo;
  return Info;
}
