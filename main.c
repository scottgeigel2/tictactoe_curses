#include "tui.h"
#include <stdbool.h>

char board[9] = "         ";
void place_tile(int coord, int player) { board[coord] = player ? 'O' : 'X'; }

bool tile_available(int coord) {
  if (coord >= 0 && coord < 10) {
    return board[coord] == ' ';
  }
  return false;
}

int is_winner() {
  // Winning combinations
  int winning_combinations[8][3] = {
      {0, 1, 2}, // Row 1
      {3, 4, 5}, // Row 2
      {6, 7, 8}, // Row 3
      {0, 3, 6}, // Column 1
      {1, 4, 7}, // Column 2
      {2, 5, 8}, // Column 3
      {0, 4, 8}, // Diagonal 1
      {2, 4, 6}  // Diagonal 2
  };

  // Check for a winner
  for (int i = 0; i < 8; i++) {
    int a = winning_combinations[i][0];
    int b = winning_combinations[i][1];
    int c = winning_combinations[i][2];

    if (board[a] != ' ' && board[a] == board[b] && board[b] == board[c]) {
      return board[a] == 'O' ? 1 : 0; // 1 for 'O', 0 for 'X'
    }
  }

  // Check for stalemate
  bool is_stalemate = true;
  for (int i = 0; i < 9; i++) {
    if (board[i] == ' ') {
      is_stalemate = false;
      break;
    }
  }

  if (is_stalemate) {
    return 2; // Stalemate
  }

  return -1; // No winner yet
}


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
  bool game_over = false;
  int x = 0;
  int y = 0;
  int player = 0;
  tui_init();
  while (!game_over) {
    int coord = (y * 3) + x;
    tui_cls();
    tui_print_board(coord, board);
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
        if (tile_available(coord)) {
          place_tile(coord, player);
          player = (player + 1) % 2;
          play_made = true;
        } else {
          continue;
        }
      }
    }

    int status = is_winner();

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
