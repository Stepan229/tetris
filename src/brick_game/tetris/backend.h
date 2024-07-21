// @file
#ifndef BACKEND_H_
#define BACKEND_H_



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FIGURE_DIR "./brick_game/tetris/figures/figure_"
#define HI_SCORE_DIR "./bin/high-score.txt"

#define ROWS_FIGURE 2
#define COLS_FIGURE 4

#define ROWS_BOARD 20
#define COLS_BOARD 10
#define SCORE_SPEED_UP 600

#define True 1
#define False 0

/// \brief Структура содержащая необходимую ВСЮ информацию для передачи в
/// frontend
typedef struct {
  int **field;     ///< Матрица для игрового поля
  int **next;      ///< Матрица для следующей фигуры
  int score;       ///< Количество очков
  int high_score;  ///< Рекорд хранящийся в памяти
  int level;       ///< Уровень
  int speed;       ///< Скорость уровня
  int pause;       ///< 0 - играем, 1 - пауза
} GameInfo_t;

typedef enum {
  Start,      //< Начало
  Pause,      //< Пауза
  Terminate,  //< Завершение игры
  Left,       //< Влево
  Right,      //< Вправо
  Up,         //< Вверх
  Down,       //< Вниз
  Action      //< Вращение фигуры
} UserAction_t;

typedef enum { Start_s, Spawn, Moving, Attaching, Shifting, Game_over } State_t;

/// \brief Структура содержит данные положения фигуры на поле и номер фигуры
typedef struct {
  int x;
  int y;
  int num_fig_field;
  int num_next_field;
} PositionFigure_t;

void check_board(GameInfo_t *info_game);

State_t *get_State_t(State_t *state);
void init_GameInfo(GameInfo_t *game);

/**

* \brief Очистка GameInfo
*/
void remove_GameInfo(GameInfo_t *game);

//! \brief Чтение рекорда из файла /
int read_high_score(void);
/// \brief Запись рекорда в файла /
void write_high_score(int record);
/// brief Чтение фигуры из файла /
void read_file_figures(int num_fig, int ***figure);

/// Создание матрицы для игрового поля и фигуры
void create_matrix(int rows, int columns, int ***matrix);

//! Создание фигуры (NEXT)
void create_figure(GameInfo_t *info_game, PositionFigure_t *position_figure);

/// Конечный автомат
void sigact(State_t *state, GameInfo_t *info_game, UserAction_t *signal,
            PositionFigure_t *pos_fig);

/// Хранение структуры UserAction_t
UserAction_t *get_UserAction(UserAction_t *sig);

/// Соединение фигуры с чем-либо
int attaching(GameInfo_t *info_game);

/// Функции движения
void move_figure(GameInfo_t *info_game, UserAction_t *signal, State_t *state,
                 PositionFigure_t *pos_fig);
int move_right(GameInfo_t *info_game, PositionFigure_t *pos_fig);
int move_left(GameInfo_t *info_game, PositionFigure_t *pos_fig);
int move_down(GameInfo_t *info_game, PositionFigure_t *pos_fig);
int rotate(GameInfo_t *info_game, PositionFigure_t *position_fig);
/// @brief Поворот фигуры "палочка"
int rotate_stick(GameInfo_t *info_game, PositionFigure_t *position_fig);
/// @brief Поворот фигур отличных от "палочки"
int rotate_figures(GameInfo_t *info_game, PositionFigure_t *position_fig);
/// @brief Копирование игрового поля в отдельный массив
int **copy_board(GameInfo_t *info_game);

/// @brief Копирование массива в структуру с игровам полем
void copy_board_in_game(int **board, GameInfo_t *game_board);

/// @brief Просмотр нижнего блока под фигурой
/// Если есть блок снизу, то возвращает True
int check_lower_block(int row, int col, int **board);

/// @brief Удаление заполневшейся строки
void del_full_row(int full_row, GameInfo_t *info_game);
/// @brief Функция для завершения игры
void ending_game(GameInfo_t *game_info);
/// @brief Вспомогательная функция для показа центра фигуры в интерфейсе
void show_center_fig(PositionFigure_t *pos);

time_t *get_timeout(time_t *time);
/// @brief Движение фигуры вниз по времени
int time_move_down(GameInfo_t *info_game, PositionFigure_t *pos_fig);
/// @brief Уменьшение паузы между движением фигуры вниз
void speeding_up(GameInfo_t *info_game);
/// @brief Пауза
void pause_game(GameInfo_t *info_game, UserAction_t *signal);
/// @brief Удаление любой матрицы и очистка памяти
void remove_matrix(int ***matrix);

#endif