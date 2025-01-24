#ifndef INCLUDE_TICTACTOE_CURSES_GAME_H_
#define INCLUDE_TICTACTOE_CURSES_GAME_H_
#include "board.h"
#include <stdbool.h>

enum e_direction { UP, DOWN, LEFT, RIGHT, SELECT };
struct GameState {
  int x;
  int y;
  bool selected; // suspect i don't need
  int player;
  int meta_idx;
  Board meta_board;
  Board boards[9];
};

bool game_tile_available(struct GameState *game);
bool game_cursor(struct GameState *game, enum e_direction dir);
int get_coord(const struct GameState *state);
int i_to_coord(int x, int y);
#endif // INCLUDE_TICTACTOE_CURSES_GAME_H_
