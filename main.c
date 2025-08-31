#include "board.h"
#include "game.h"
#include "tui.h"
#include <stdbool.h>
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
    default:;
    }
    c = tui_read_char();
  }

  return c;
}

// returns true if a selection move was made
bool make_move(struct GameState *state) {
  bool play_made = false;
  bool move_was_selection_type = false;
  while (!play_made) {
    char c = getPlayerMove();
    enum e_direction dir;
    switch (c) {
    case 'j':
      dir = DOWN;
      break;
    case 'k':
      dir = UP;
      break;
    case 'l':
      dir = RIGHT;
      break;
    case 'h':
      dir = LEFT;
      break;
    case ' ':
      dir = SELECT;
      move_was_selection_type = true;
      break;
    default:
      continue;
      break;
    }
    play_made = game_cursor(state, dir);
  }
  return move_was_selection_type;
}

void handle_game_end(struct GameState *state, Board *current_board, int status,
                     int coord) {
  if (state->meta_idx == -1) {
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
  } else {
    switch (status) {
    case 2:
      current_board->board[coord] =
          '*'; // TODO: this feels wrong. i kind of didn't want to
               // have a "mark stalemate" in Board... perhaps i'm
               // overthinking it. or idk ... i could like make an
               // extensions module for Board?
      break;
    case 1:
      board_place_tile(current_board, coord, state->player);
      break;
    case 0:
      board_place_tile(current_board, coord, state->player);
      break;
    default:
      break;
    }
  }
}
void switch_player(struct GameState *state) {
  state->player = (state->player + 1) % 2;
}

int main() {
  struct GameState state = {
      .x = 0, .y = 0, .selected = false, .player = 0, .meta_idx = -1};
  bool game_over = false;
  int game_status;

  for (int i = 0; i < 9; i++) {
    board_init(&state.boards[i]);
  }

  board_init(&state.meta_board);
  tui_init();

  // tui_debug();
  while (!game_over) {
    bool this_player_was_meta = state.meta_idx != -1;
    tui_cls();
    tui_print_game(&state);

    tui_print_message(state.player ? "O's Turn\n" : "X's Turn\n");

    bool select_move = make_move(&state);
    game_status = board_is_winner(&state.meta_board);
    game_over = game_status != -1;
    if (!game_over && (select_move && this_player_was_meta)) {
      switch_player(&state);
    }
  }
  tui_cls();
  tui_print_game(&state);
  handle_game_end(&state, &state.meta_board, game_status, -1);
  tui_print_message("Game Over!!!");
  tui_deinit();
}
