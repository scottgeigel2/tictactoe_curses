#include "game.h"
#include "board.h"

int get_coord(const struct GameState *state);
bool game_tile_available(struct GameState *game) {
  int const coord = get_coord(game);

  if (game->meta_idx == -1) {
    // playing on the meta board
    return board_tile_available(&game->meta_board, coord);
  } else {
    return board_tile_available(&game->boards[game->meta_idx], coord);
  }
}

bool game_cursor(struct GameState *game, enum e_direction dir) {
  bool moved = false;
  switch (dir) {
  case UP:
    if (game->y > 0) {
      game->y--;
      moved = true;
    }
    break;
  case DOWN:
    if (game->y < 2) {
      game->y++;
      moved = true;
    }
    break;
  case LEFT:
    if (game->x > 0) {
      game->x--;
      moved = true;
    }
    break;
  case RIGHT:
    if (game->x < 2) {
      game->x++;
      moved = true;
    }
    break;
  case SELECT:
    if (game_tile_available(game)) {
      game->selected = true;
      moved = true;
      if (game->meta_idx == -1) {
        game->meta_idx = get_coord(game);
      } else {
        board_place_tile(&game->boards[game->meta_idx], get_coord(game),
                         game->player);
        if (board_is_winner(&game->boards[game->meta_idx]) != -1) {
          board_place_tile(&game->meta_board, game->meta_idx, game->player);
        }

        if (board_tile_available(&game->meta_board, get_coord(game))) {
          game->meta_idx = get_coord(game);
        } else {
          game->meta_idx = -1;
        }
      }
    }
    break;
  }
  return moved;
}

int get_coord(const struct GameState *state) {
  return (state->y * 3) + state->x;
}

int i_to_coord(int x, int y) { return (y * 3) + x; }
