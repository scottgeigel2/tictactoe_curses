#include "tui.h"
#include "board.h"
#include <stdbool.h>

struct GameState
{
  int x;
  int y;
  bool selected;
  int player;
};

char getPlayerMove() {
  char c = tui_read_char();
  while (c != 0) {
    switch (c) {
    case 'j':
    case 'k':
    case 'l':
    case 'h':
    case ' ':
      return c;
    default:
      continue;
    }
  }

  return c;
}

void make_move(struct GameState *state)
{
  bool play_made = false;
  while (!play_made) {
    char c = getPlayerMove();
    if (c == 'j') {
      if ((state->y + 1) >= 3) {
        continue;
      }
      play_made = true;
      state->y++;
    }
    if (c == 'k') {
      if ((state->y - 1) < 0) {
        continue;
      }
      play_made = true;
      state->y--;
    }
    if (c == 'l') {
      if ((state->x + 1) >= 3) {
        continue;
      }
      play_made = true;
      state->x++;
    }
    if (c == 'h') {
      if ((state->x - 1) < 0) {
        continue;
      }
      play_made = true;
      state->x--;
    }
    if (c == ' ')
    {
      state->selected = true;
      play_made = true;
    }
  }
}

void switch_player(struct GameState *state)
{
  state->player = (state->player + 1) % 2;
}

int get_coord(struct GameState *state)
{
  return (state->y * 3) + state->x;
}

int main() {
  struct GameState state = {.x = 0, .y =0, .selected = false, .player = 0};
  Board meta_board;
  Board boards[9];
  bool game_over = false;

  tui_init();
  board_init(&meta_board);
  while (!game_over) {
    int coord = get_coord(&state);
    tui_cls();
    tui_print_board(coord, &meta_board);
    tui_print_message(state.player ? "O's Turn" : "X's Turn");

    make_move(&state);
    if (state.selected) {
      if (board_tile_available(&meta_board, coord)) {
        board_place_tile(&meta_board, coord, state.player);
        switch_player(&state);
      } else {
        state.selected = false;
        continue;
      }
    }

    if (state.selected)
    {
      int status = board_is_winner(&meta_board);

      if (status >= 0) {
        game_over = true;
        tui_cls();
        tui_print_board(tui_NOT_A_COORD, &meta_board);
      }
      switch (status) {
      case 2:
        tui_print_message("Draw\n");
        break;
      case 1:
        tui_print_message("O wins\n");
        break;
      case 0:
        tui_print_message("X wins\n");
        break;
      default:
        break;
      }
    }
    state.selected = false;
  }
  tui_deinit();
}
