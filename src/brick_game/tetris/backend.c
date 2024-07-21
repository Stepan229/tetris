#include "backend.h"

void init_GameInfo(GameInfo_t *game) {
  create_matrix(20, 10, &game->field);
  create_matrix(2, 4, &game->next);
  // game->next = NULL;
  game->score = 0;
  game->high_score = read_high_score();
  game->level = 0;
  game->speed = 0;
  game->pause = 0;
}

void create_matrix(int rows, int columns, int ***matrix) {
  int **temp_matrix = (int **)calloc(rows, sizeof(int *));
  temp_matrix[0] = (int *)calloc(rows * columns, sizeof(int));
  for (int i = 1; i < rows; i++) {
    temp_matrix[i] = temp_matrix[0] + i * columns;
  }
  *matrix = temp_matrix;
}
//! Создание фигуры (NEXT)
void remove_GameInfo(GameInfo_t *game) {
  remove_matrix(&game->field);
  remove_matrix(&game->next);
}

void remove_matrix(int ***matrix) {
  free(*matrix[0]);
  free(*matrix);
}

int read_high_score(void) {
  int score = 0;
  FILE *ptrFile = fopen(HI_SCORE_DIR, "r");
  if (ptrFile) {  // Проверка на  существование файла
    fscanf(ptrFile, "%d", &score);
    fclose(ptrFile);
  }
  return score;
}

void sigact(State_t *state, GameInfo_t *info_game, UserAction_t *signal,
            PositionFigure_t *pos_fig) {
  if (*signal == Pause) {
    pause_game(info_game, signal);
  } else if (*signal == Terminate)
    *state = Game_over;
  switch (*state) {
    case Start_s:
      switch (*signal) {
        case Start:
          *state = Spawn;
          break;
        case Terminate:
          *state = Game_over;
          break;
        default:
          *state = Start_s;
          break;
      }
      break;
    case Spawn:
      create_figure(info_game, pos_fig);
      *state = Moving;
      break;
    case Moving:
      if (*signal != Terminate && info_game->pause != True) {
        move_figure(info_game, signal, state, pos_fig);
        if (*state != Attaching) {
          *state = Shifting;
        }
      }
      break;
    case Shifting:
      if ((*signal != Terminate && *signal != Pause) ||
          info_game->pause != True) {
        if (time_move_down(info_game, pos_fig))
          *state = Attaching;
        else
          *state = Moving;
      }
      break;
    case Attaching:
      if (!attaching(info_game))
        *state = Spawn;
      else
        *state = Game_over;
      break;
    case Game_over:
      ending_game(info_game);
      break;
    default:
      break;
  }
}

void create_figure(GameInfo_t *info_game, PositionFigure_t *position_figure) {
  srand(time(NULL));
  position_figure->x = 4;
  position_figure->y = 1;

  if (position_figure->num_next_field == 0) {
    position_figure->num_fig_field = rand() % 7;
    position_figure->num_next_field = rand() % 7;
    int **fig_field = NULL;
    create_matrix(2, 4, &fig_field);
    read_file_figures(position_figure->num_next_field, &info_game->next);
    read_file_figures(position_figure->num_fig_field, &fig_field);
    for (int row = 0; row < ROWS_FIGURE; row++) {
      for (int col = 0; col < COLS_FIGURE; col++) {
        info_game->field[row][col + 3] = fig_field[row][col];
      }
    }
    remove_matrix(&fig_field);
  } else {
    for (int row = 0; row < ROWS_FIGURE; row++) {
      for (int col = 0; col < COLS_FIGURE; col++) {
        info_game->field[row][col + 3] = info_game->next[row][col];
      }
    }
    position_figure->num_fig_field = position_figure->num_next_field;
    position_figure->num_next_field = rand() % 7;
    read_file_figures(position_figure->num_next_field, &info_game->next);
  }
}

void read_file_figures(int num_fig, int ***figure) {
  char levelname[50];
  char lines[50];
  int **matrix = *figure;
  // create_matrix(2, 4, &matrix);
  sprintf(levelname, FIGURE_DIR "%d.txt", num_fig + 1);
  FILE *file = fopen(levelname, "r");
  if (file) {
    for (int row = 0; row < ROWS_FIGURE; row++) {
      fscanf(file, "%s", lines);
      for (int col = 0; col < COLS_FIGURE; col++) {
        matrix[row][col] = lines[col] == '1' ? 1 : 0;
      }
    }
    fclose(file);
    // *figure = matrix;
  }
  // remove_matrix(&matrix);
  // free(matrix[0]);
  // free(matrix);
}

State_t *get_State_t(State_t *state) {
  static State_t *Info;
  if (state == NULL) return Info;
  Info = state;
  return Info;
}

time_t *get_timeout(time_t *time) {
  static time_t timeout;
  if (time == NULL) return &timeout;
  timeout = *time;
  return &timeout;
}

UserAction_t *get_UserAction(UserAction_t *sig) {
  static UserAction_t *Sig;
  if (sig == NULL) return Sig;
  Sig = sig;
  return Sig;
}

// Если игра закончена возвращает True
int attaching(GameInfo_t *info_game) {
  int game_over = False;
  int del_rows = 0;
  for (int row = 0; row < ROWS_BOARD; row++) {
    int point_in_row = 0;
    for (int col = 0; col < COLS_BOARD; col++) {
      if (info_game->field[row][col] == 1) info_game->field[row][col] = 2;
      if (info_game->field[row][col] == 2) point_in_row++;
      if ((row == 1) && info_game->field[row][col] == 2) game_over = True;
    }
    // проверка на заполняемость строки
    if (point_in_row == 10) {
      del_full_row(row, info_game);
      del_rows++;
    }
  }
  switch (del_rows) {
    case 1:
      info_game->score += 100;
      break;
    case 2:
      info_game->score += 300;
      break;
    case 3:
      info_game->score += 700;
      break;
    case 4:
      info_game->score += 1500;
      break;
    default:
      break;
  }
  speeding_up(info_game);
  return game_over;
}

// удаление заполнившейся строки
void del_full_row(int full_row, GameInfo_t *info_game) {
  // зануление строки full_row
  memset(info_game->field[full_row], 0, sizeof(int) * COLS_BOARD);
  for (int row = full_row - 1; row >= 0; row--) {
    for (int col = 0; col < COLS_BOARD; col++) {
      info_game->field[row + 1][col] = info_game->field[row][col];
    }
  }
}

void ending_game(GameInfo_t *game_info) {
  // int old_hi_score = read_high_score();
  if (game_info->high_score < game_info->score)
    write_high_score(game_info->score);
  // remove_GameInfo(game_info);
}

void write_high_score(int record) {
  FILE *ptrFile = fopen(HI_SCORE_DIR, "w");
  if (ptrFile) {  // Проверка на  существование файла
    fprintf(ptrFile, "%d", record);
    fclose(ptrFile);
  }
}

int time_move_down(GameInfo_t *info_game, PositionFigure_t *pos_fig) {
  time_t *start_time = get_timeout(NULL), now_time = clock();
  int attach = False;
  if ((now_time - *start_time) >= 3000 - info_game->level * 290) {
    attach = move_down(info_game, pos_fig);
    get_timeout(&now_time);
  }
  return attach;
}

void speeding_up(GameInfo_t *info_game) {
  info_game->level = info_game->score / SCORE_SPEED_UP + 1;
  info_game->speed = info_game->level * 100;
}

void pause_game(GameInfo_t *info_game, UserAction_t *signal) {
  if (*signal == Pause && !info_game->pause) {
    info_game->pause = True;
  } else if (*signal == Pause && info_game->pause)
    info_game->pause = False;
}
