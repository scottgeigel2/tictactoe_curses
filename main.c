#include "tui.h"
#include "board.h"
#include <stdio.h>
#include <stdbool.h>

struct GameState
{
  int x;
  int y;
  bool selected;
  int player;
  int meta_board;
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

void handle_game_end(struct GameState *state, Board *current_board, int status, int coord)
{
  if (state->meta_board == -1) {
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
          //TODO: what do the rules say about draws?
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
void switch_player(struct GameState *state)
{
  state->player = (state->player + 1) % 2;
}

int get_coord(struct GameState *state)
{
  return (state->y * 3) + state->x;
}

int main() {
  struct GameState state = {.x = 0, .y =0, .selected = false, .player = 0, .meta_board = -1};
  Board meta_board;
  Board boards[9];
  Board *current_board = &meta_board;
  bool game_over = false;
  char msg_buf[80] = {0};

  for (int i = 0; i < 9; i++) {
    board_init(&boards[i]);
  }
  board_init(&meta_board);
  tui_init();
  while (!game_over) {
    int coord = get_coord(&state);
    tui_cls();
    tui_print_board(coord, current_board);
    sprintf(msg_buf, "%s\tmeta %d current_board %p",state.player ? "O's Turn" : "X's Turn", state.meta_board, current_board);
    //tui_print_message(state.player ? "O's Turn" : "X's Turn");
    tui_print_message(msg_buf);

    make_move(&state);
    if (state.selected) {
      if (board_tile_available(current_board, coord)) {
        if (state.meta_board == -1) {
          //we need to go deeper
          state.meta_board = coord;
          current_board = &boards[coord];
          state.selected = false; //squash the selection to go into the meta boards
        }
        else {
          board_place_tile(current_board, coord, state.player);
          switch_player(&state);
        }
      } else {
        state.selected = false;
        sprintf(msg_buf, "tile %d not available", coord);
        tui_print_message(msg_buf);
        tui_read_char();
        continue;
      }
    }

    if (state.selected)
    {
      int status = board_is_winner(current_board);

      if (status >= 0) {
        game_over = true;
        tui_cls();
        current_board = &meta_board;
        handle_game_end(&state, current_board, status, coord);
        tui_print_board(tui_NOT_A_COORD, current_board);
      }
      state.meta_board = -1;
      state.x = 0;
      state.y = 0;
      current_board = &meta_board;
    }
    state.selected = false;
  }
  tui_deinit();
}
