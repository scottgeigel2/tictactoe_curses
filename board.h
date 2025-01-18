#ifndef __BOARD_H__
#define __BOARD_H__
#include <stdbool.h>
typedef struct {
  char board[9];
} Board;
void board_init(Board* self);
void board_place_tile(Board *self, int coord, int player);
bool board_tile_available(const Board *self, int coord);
int board_is_winner(const Board *self);
char board_get_idx(const Board *self, int idx);
#endif
