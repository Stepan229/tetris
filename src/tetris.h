#ifndef TETRIS_H_
#define TETRIS_H_

#include "./brick_game/tetris/backend.h"
#include "./gui/cli/frontend.h"

void game_loop();
UserAction_t get_signal(int user_input);

#endif  // TETRIS_H_