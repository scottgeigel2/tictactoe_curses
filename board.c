#include "board.h"
#include <string.h>

void board_init(Board *self)
{
  memset(&self->board, ' ', 9);
}

void board_place_tile(Board *self, int coord, int player)
{
  self->board[coord] = player ? 'O' : 'X';
}

bool board_tile_available(const Board *self, int coord) {
  if (coord >= 0 && coord < 10) {
    return self->board[coord] == ' ';
  }
  return false;
}

int board_is_winner(const Board *self) {
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

    if (self->board[a] != ' ' && self->board[a] == self->board[b] && self->board[b] == self->board[c]) {
      return self->board[a] == 'O' ? 1 : 0; // 1 for 'O', 0 for 'X'
    }
  }
  // Check for stalemate
  bool is_stalemate = true;
  for (int i = 0; i < 9; i++) {
    if (self->board[i] == ' ') {
      is_stalemate = false;
      break;
    }
  }

  if (is_stalemate) {
    return 2; // Stalemate
  }

  return -1; // No winner yet
}

char board_get_idx(const Board *self, int idx)
{
  return self->board[idx];
}

