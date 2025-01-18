#include "tui.h"
#include "board.h"
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
    default:
      continue;
    }
  }

  return c;
}

int main() {
  Board board;
  bool game_over = false;
  int x = 0;
  int y = 0;
  int player = 0;
  tui_init();
  board_init(&board);
  while (!game_over) {
    int coord = (y * 3) + x;
    tui_cls();
    tui_print_board(coord, &board);
    tui_print_message(player ? "O's Turn" : "X's Turn");

    bool play_made = false;
    while (!play_made) {
      char c = getPlayerMove();
      if (c == 'j') {
        if ((y + 1) >= 3) {
          continue;
        }
        play_made = true;
        y++;
      }
      if (c == 'k') {
        if ((y - 1) < 0) {
          continue;
        }
        play_made = true;
        y--;
      }
      if (c == 'l') {
        if ((x + 1) >= 3) {
          continue;
        }
        play_made = true;
        x++;
      }
      if (c == 'h') {
        if ((x - 1) < 0) {
          continue;
        }
        play_made = true;
        x--;
      }
      if (c == ' ') {
        if (board_tile_available(&board, coord)) {
          board_place_tile(&board, coord, player);
          player = (player + 1) % 2;
          play_made = true;
        } else {
          continue;
        }
      }
    }

    int status = board_is_winner(&board);

    if (status >= 0) {
      game_over = true;
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
  tui_deinit();
}
