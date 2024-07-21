#include "backend.h"

void move_figure(GameInfo_t *info_game, UserAction_t *signal, State_t *state,
                 PositionFigure_t *position_fig) {
  switch (*signal) {
    case Right:
      if (move_right(info_game, position_fig)) *state = Attaching;
      break;
    case Left:
      if (move_left(info_game, position_fig)) *state = Attaching;
      break;
    case Down:
      if (move_down(info_game, position_fig)) *state = Attaching;
      break;
    case Action:
      if (rotate(info_game, position_fig)) *state = Attaching;
      break;
    default:
      break;
  }
}

int move_right(GameInfo_t *info_game, PositionFigure_t *pos_fig) {
  int no_move = False;
  int flag_attaching = False;
  for (int row = 0; row < ROWS_BOARD; row++) {
    if (info_game->field[row][COLS_BOARD - 1] == 1) {
      no_move = True;
      break;
    }
  }
  if (no_move == False) {
    int **board = copy_board(info_game);
    for (int row = 0; row < ROWS_BOARD - 1; row++) {
      for (int col = COLS_BOARD - 2; col >= 0; col--) {
        // Проверка на последниюю строку
        if (check_lower_block(row, col, board)) {
          flag_attaching = True;
          break;
        } else if (board[row][col] == 1 && board[row][col + 1] == 2) {
          no_move = True;
        }
        // Проверка на правую границу и блок
        else if (board[row][col] == 1) {
          info_game->field[row][col] = 0;
          info_game->field[row][col + 1] = 1;
        }
      }
    }
    if (no_move == True || flag_attaching == True) {
      copy_board_in_game(board, info_game);
    } else
      pos_fig->x++;
    remove_matrix(&board);
  }
  return flag_attaching;
}

int move_left(GameInfo_t *info_game, PositionFigure_t *pos_fig) {
  int flag_attaching = False;
  int no_move = False;
  for (int row = 0; row < ROWS_BOARD; row++) {
    if (info_game->field[row][0] == 1) {
      no_move = True;
      break;
    }
  }
  if (no_move == False) {
    int **board = copy_board(info_game);
    for (int row = ROWS_BOARD - 2; row >= 0; row--) {
      for (int col = 0; col < COLS_BOARD; col++) {
        if (check_lower_block(row, col, board)) {
          flag_attaching = True;
          break;
        } else if (board[row][col] == 1 && board[row][col - 1] == 2) {
          no_move = True;
        }
        // Проверка на левую границу и блок
        else if (board[row][col] == 1) {
          info_game->field[row][col] = 0;
          info_game->field[row][col - 1] = 1;
        }
      }
    }
    if (flag_attaching == True || no_move == True) {
      copy_board_in_game(board, info_game);
    } else
      pos_fig->x--;
    remove_matrix(&board);
  }

  return flag_attaching;
}

// Возвращает False, если нет ничего снизу (не state Attaching), если есть блок
// - True
int move_down(GameInfo_t *info_game, PositionFigure_t *pos_fig) {
  int flag_attaching = False;
  int no_move = False;
  int **board = copy_board(info_game);
  for (int row = ROWS_BOARD - 2; row >= 0; row--) {
    for (int col = 0; col < COLS_BOARD; col++) {
      if (check_lower_block(row, col, board) && board[row][col] == 1) {
        no_move = True;
        flag_attaching = True;
        break;
      } else if (board[row][col] == 1) {
        info_game->field[row][col] = 0;
        info_game->field[row + 1][col] = 1;
        if (row + 2 < ROWS_BOARD - 3 && check_lower_block(row + 1, col, board))
          flag_attaching = True;
      }
    }
  }
  // MVPRINTW1(14, 12 + 13, "%d", flag_attaching);
  if (no_move == True) {
    copy_board_in_game(board, info_game);
  } else
    pos_fig->y++;
  remove_matrix(&board);
  return flag_attaching;
}

int check_lower_block(int row, int col, int **board) {
  int lower_block = False;
  if (row == ROWS_BOARD - 2 && board[row + 1][col] == 1) {
    lower_block = True;
  }
  // Проверка на нахождение блока снизу
  if (board[row + 1][col] == 2 && board[row][col] == 1) {
    lower_block = True;
  }
  return lower_block;
}

int **copy_board(GameInfo_t *info_game) {
  int **board;
  create_matrix(20, 10, &board);
  for (int row = 0; row < ROWS_BOARD; row++) {
    for (int col = 0; col < COLS_BOARD; col++)
      board[row][col] = info_game->field[row][col];
  }
  return board;
}

void copy_board_in_game(int **board, GameInfo_t *game_board) {
  for (int row = 0; row < ROWS_BOARD; row++) {
    for (int col = 0; col < COLS_BOARD; col++) {
      game_board->field[row][col] = board[row][col];
    }
  }
}

int rotate(GameInfo_t *info_game, PositionFigure_t *position_fig) {
  int flag_attach = False;
  if (position_fig->num_fig_field == 1)
    flag_attach = rotate_stick(info_game, position_fig);
  else if (position_fig->num_fig_field != 4)
    flag_attach = rotate_figures(info_game, position_fig);
  return flag_attach;
}

int rotate_stick(GameInfo_t *info_game, PositionFigure_t *position_fig) {
  int center_x = position_fig->x, center_y = position_fig->y - 1;
  int **figure_copy;
  int flag_cancel = False;
  create_matrix(5, 5, &figure_copy);
  int **board_copy = copy_board(info_game);
  int row_end = 3;
  for (int row = -2; row < row_end; row++) {
    for (int col = -2; col < 3; col++) {
      if (center_y + row < 0 || center_y + row > 19) {
        flag_cancel = True;
        break;
      } else if (info_game->field[center_y + row][center_x + col] == 2) {
        flag_cancel = True;
        break;
      } else if (info_game->field[center_y + row][center_x + col] != 2) {
        figure_copy[row + 2][col + 2] =
            info_game->field[center_y + row][center_x + col];
      }
    }
  }
  if (flag_cancel == False) {
    for (int y = -2; y < 3; y++) {
      for (int x = -2; x < 3; x++) {
        int new_x = center_x + center_y - (center_y + y);
        int new_y = -center_x + center_y + (center_x + x);
        int new_block = figure_copy[y + 2][x + 2];
        if (new_block == 1 &&
            (new_x < 0 || new_x > 9 || board_copy[new_x][new_y] == 2)) {
          flag_cancel = True;
          break;
        }
        board_copy[new_y][new_x] = new_block;
      }
    }
  }
  if (flag_cancel == False) copy_board_in_game(board_copy, info_game);
  remove_matrix(&board_copy);
  remove_matrix(&figure_copy);
  return 0;
}

int rotate_figures(GameInfo_t *info_game, PositionFigure_t *position_fig) {
  int flag_attach = False, flag_cancel = False;
  int **figure_copy;
  int center_x = position_fig->x, center_y = position_fig->y;
  create_matrix(3, 3, &figure_copy);
  int x = -1, y = -1;
  for (int row = -1; row < 2; row++) {
    for (int col = -1; col < 2; col++) {
      if (center_y + row < 0 || center_y + row > 19 || center_x + col < 0 ||
          center_x + col > 9) {
        flag_cancel = True;
        break;
      } else if (info_game->field[center_y + row][center_x + col] == 2) {
        flag_cancel = True;
        break;
      } else {
        figure_copy[row + 1][col + 1] =
            info_game->field[center_y + row][center_x + col];
      }
    }
  }
  if (flag_cancel == False) {
    do {
      int new_x = center_x + center_y - (center_y + y);
      int new_y = -center_x + center_y + (center_x + x);
      info_game->field[new_y][new_x] = figure_copy[y + 1][x + 1];
      if (check_lower_block(new_x, new_y, info_game->field)) flag_attach = True;
      if (y == -1 && x < 1)
        x++;
      else if (x == 1 && y < 1)
        y++;
      else if (y == 1 && x > -1)
        x--;
      else if (x == -1 && y > -1)
        y--;
    } while (!(x == -1 && y == -1));
  }
  remove_matrix(&figure_copy);
  return flag_attach;
}