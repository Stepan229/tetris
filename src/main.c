#include "tetris.h"

int main() {
  initscr();
  halfdelay(1);
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  setlocale(LC_ALL, "");
  print_overlay();
  game_loop();
  endwin();
  return 0;
}

void game_loop() {
  GameInfo_t info_game;
  static UserAction_t signal = -1;
  PositionFigure_t pos_fig = {0};
  get_UserAction(&signal);
  init_GameInfo(&info_game);
  change_GameInfo_t(&info_game);
  info_game.pause = 0;
  static State_t state = Start_s;
  get_State_t(&state);
  int break_flag = TRUE;
  int button = -1;
  time_t start_time = clock();
  get_timeout(&start_time);
  while (break_flag) {
    if (state == Start_s || state == Moving) button = GET_USER_INPUT;
    if (signal == Terminate || state == Game_over) break_flag = False;
    signal = get_signal(button);
    sigact(&state, &info_game, &signal, &pos_fig);
    get_State_t(&state);
    get_GameInfo_t(&info_game);
    updateCurrentState();
  }
}

UserAction_t get_signal(int user_input) {
  UserAction_t *rc = get_UserAction(NULL);
  switch (user_input) {
    case ENTER:
      *rc = Start;
      break;
    case KEY_DOWN:
      *rc = Down;
      break;
    case KEY_RIGHT:
      *rc = Right;
      break;
    case KEY_LEFT:
      *rc = Left;
      break;
    case KEY_UP:
      *rc = Up;
      break;
    case KEY_ESCAPE:
      *rc = Terminate;
      break;
    case KEY_SPACE:
      *rc = Action;
      break;
    case KEY_P:
      *rc = Pause;
      break;
    case KEY_p:
      *rc = Pause;
      break;
    default:
      *rc = -1;
      break;
  }
  return *rc;
}

// void check_board(GameInfo_t *info_game){
//     for (int row = 0; row < ROWS_BOARD; row++){
//         for (int col = 0; col < COLS_BOARD; col++)
//              MVPRINTW(5 + row, 40+col, "%d ", info_game->field[row][col]);
//     }
// }

// void show_center_fig(PositionFigure_t *pos){
//     MVPRINTW(9, RIGHT_MAP + 13, "%d", pos->x);
//     MVPRINTW(10, RIGHT_MAP + 13, "%d", pos->y);
//     MVPRINTW(11, RIGHT_MAP + 13, "%d", pos->num_fig_field);
//     MVPRINTW(12, RIGHT_MAP + 13, "%d", pos->num_fig_field);
// }