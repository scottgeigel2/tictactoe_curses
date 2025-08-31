#include "tui.h"
#include "board.h"
#include <assert.h> // TODO: this is being used for an assertion... consider an assertion header if
#include <stdarg.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
// we assert more

// type defs
enum e_tile {
  e_tile_EMPTY_SMALL,
  e_tile_X_SMALL,
  e_tile_O_SMALL,
  e_tile_S_SMALL, // TODO: this doesn't actually exist.... or does it? maybe in
                  // meta-meta-tictactoe?
  e_tile_EMPTY_LARGE,
  e_tile_X_LARGE,
  e_tile_O_LARGE,
  e_tile_S_LARGE
};

// private variables
struct termios set_no_echo();
bool debug_flag = false;
// private functions
void clear_no_echo(struct termios *oldt);
bool e_tile_is_large(enum e_tile tile);
void tui_set_cursor(int x, int y);
void print_small_board(const struct GameState *game, const Board *board,
                       int base_x, int base_y, bool this_board_is_selected,
                       bool this_board_is_hovered);
// private globals
const char X_Tile[5][7] = {
    {"\\\\  //"}, {" \\\\// "}, {"  ||  "}, {" //\\\\ "}, {"//  \\\\"}
    // this line intentionally left blank
};

const char O_Tile[5][7] = {
    {"/ -- \\"}, {"|    |"}, {"|    |"}, {"|    |"}, {"\\ -- /"}};

const char Empty_Tile[5][7] = {
    {"......"}, {"......"}, {"......"}, {"......"}, {"......"}};

const char Stalemate_Tile[5][7] = {
    {"******"}, {"******"}, {"******"}, {"******"}, {"******"}};

const char SmallBoard_gfx[5][6] = {
    {" | | "}, {"-----"}, {" | | "}, {"-----"}, {" | | "},
};

const char Board_gfx[20][40] = {
    {"            ||            ||           "},
    {"            ||            ||           "},
    {"            ||            ||           "},
    {"            ||            ||           "},
    {"            ||            ||           "},
    {"____________||____________||___________"},
    {"____________||____________||___________"},
    {"            ||            ||           "},
    {"            ||            ||           "},
    {"            ||            ||           "},
    {"            ||            ||           "},
    {"            ||            ||           "},
    {"____________||____________||___________"},
    {"____________||____________||___________"},
    {"            ||            ||           "},
    {"            ||            ||           "},
    {"            ||            ||           "},
    {"            ||            ||           "},
    {"            ||            ||           "},
    {"            ||            ||           "},
};

static int screen_x = 0;
static int screen_y = 20;

static struct termios _termios_backup;

void tui_init() { _termios_backup = set_no_echo(); }

void tui_deinit() { clear_no_echo(&_termios_backup); }

struct termios set_no_echo() {
  struct termios oldt, newt;

  // Get current terminal attributes
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  // Disable canonical mode and echo
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  return oldt;
}

void clear_no_echo(struct termios *oldt) {
  // Restore old terminal attributes
  tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}

bool e_tile_is_large(enum e_tile tile) {
  switch (tile) {
  case e_tile_EMPTY_SMALL:
  case e_tile_X_SMALL:
  case e_tile_O_SMALL:
  case e_tile_S_SMALL:
    return false;
  case e_tile_EMPTY_LARGE:
  case e_tile_X_LARGE:
  case e_tile_O_LARGE:
  case e_tile_S_LARGE:
    return true;
  default:
    assert(false);
  }
}
char tui_read_char() {
  // Read a single character
  char ch = getchar();
  return ch;
}

void tui_cls() {
  puts("\033[2J\033[H"); // clear screen
  screen_y = 20;
}

void print_tile(int x, int y, enum e_tile tile_selection, bool invert) {
  if (invert) {
    printf("\033[7m");
  }
  if (e_tile_is_large(tile_selection)) {
    for (int i = 0; i < 5; i++) {
      if (false && !debug_flag) {
        debug_flag = tui_read_char() == 'q';
      }
      if (tile_selection == e_tile_X_LARGE) {
        printf("\033[%d;%dH%s", y, x, X_Tile[i]);
      } else if (tile_selection == e_tile_O_LARGE) {
        printf("\033[%d;%dH%s", y, x, O_Tile[i]);
      } else if (tile_selection == e_tile_S_LARGE) {
        printf("\033[%d;%dH%s", y, x, Stalemate_Tile[i]);
      } else // empty
      {
        printf("\033[%d;%dH%s", y, x, SmallBoard_gfx[i]);
      }
      y++;
    }
  } else // one of the small tyles
  {
    if (tile_selection == e_tile_X_SMALL) {
      printf("\033[%d;%dHX", y, x);
    } else if (tile_selection == e_tile_O_SMALL) {
      printf("\033[%d;%dHO", y, x);
    } else if (tile_selection == e_tile_S_SMALL) {
      printf("\033[%d;%dH*", y, x);
    } else // empty
    {
      printf("\033[%d;%dH.", y, x);
    }
  }
  if (invert) {
    printf("\033[0m");
  }
  fflush(stdout);
}

void print_meta_board(const struct GameState *game) {
  int i, j;
  int x = 2, y = 1;
  int idx_base;
  int tile_width,
      tile_height; // TODO: width has so much space padding.... why not height?
  int board_border_width,
      board_border_height; // TODO: currently much justification to have these
                           // separate, but maybe one day i'll have a border
                           // who's height is not equal to its width
  int tile_padding_left;   // TODO: effectively the same number ATM, but maybe
                           // one day I might need this? if not, just change it
                           // to a simple padding var

  // TODO: also IDK, maybe vertical padding????

  enum e_tile e_xtile, e_otile, e_stalemate_tile, e_empty_tile;
  e_xtile = e_tile_X_LARGE;
  e_otile = e_tile_O_LARGE;
  e_stalemate_tile = e_tile_S_LARGE;
  e_empty_tile = e_tile_EMPTY_LARGE;
  tile_width = 12;
  tile_height = 5;
  board_border_width = 2;
  board_border_height = 2;
  tile_padding_left = 3;

  int selected_tile = get_coord(game);

  for (i = 0; i < 3; i++) {
    x = 1 + tile_padding_left;
    idx_base = i * 3;
    for (j = 0; j < 3; j++, idx_base++) {
      // tui_read_char();
      bool meta_tile_hover =
          game->meta_idx == -1 && (i_to_coord(j, i) == selected_tile);
      tui_set_cursor(x, y);

      if (board_get_idx(&game->meta_board, idx_base) == 'O') {
        print_tile(x, y, e_otile, meta_tile_hover);
      } else if (board_get_idx(&game->meta_board, idx_base) == 'X') {
        print_tile(x, y, e_xtile, meta_tile_hover);
      } else if (board_get_idx(&game->meta_board, idx_base) == '*') {
        print_tile(x, y, e_stalemate_tile, meta_tile_hover);
      } else {
        bool tile_selected =
            (game->meta_idx != -1) && ((i * 3) + j) == selected_tile;
        if (meta_tile_hover) {
          printf("\033[7m");
        }
        print_tile(x, y, e_empty_tile, meta_tile_hover);
        print_small_board(game, &game->boards[idx_base], x, y, tile_selected,
                          meta_tile_hover);
        if (meta_tile_hover) {
          printf("\033[0m");
        }
      }
      x += tile_width + board_border_width;
    }
    y += tile_height + board_border_height;
  }
}

void print_small_board(const struct GameState *game, const Board *board,
                       int base_x, int base_y, bool this_board_is_selected,
                       bool this_board_is_hovered) {
  int i, j;
  int x = base_x, y = base_y;
  int idx_base;
  int tile_width,
      tile_height; // TODO: width has so much space padding.... why not height?
  int board_border_width,
      board_border_height; // TODO: currently much justification to have these
                           // separate, but maybe one day i'll have a border
                           // who's height is not equal to its width
  // TODO: also IDK, maybe vertical padding????

  enum e_tile e_xtile, e_otile, e_stalemate_tile, e_empty_tile;
  int coord = get_coord(game);

  e_xtile = e_tile_X_SMALL;
  e_otile = e_tile_O_SMALL;
  e_stalemate_tile = e_tile_S_SMALL;
  e_empty_tile = e_tile_EMPTY_SMALL;
  tile_width = 1;
  tile_height = 1;
  board_border_width = 1;
  board_border_height = 1;
  //  if (selected_tile) {
  //    printf("\033[7m");
  //  }
  bool this_board_is_being_played =
      game->meta_idx != -1 && (game->meta_idx == (board - game->boards));
  idx_base = 0;
  for (i = 0; i < 3; i++) {
    x = base_x;
    for (j = 0; j < 3; j++, idx_base++) {
      bool tile_is_selected =
          this_board_is_hovered ||
          (this_board_is_being_played && (i_to_coord(j, i) == coord));
      tui_set_cursor(x, y);

      if (board_get_idx(board, idx_base) == 'O') {
        print_tile(x, y, e_otile, tile_is_selected);
      } else if (board_get_idx(board, idx_base) == 'X') {
        print_tile(x, y, e_xtile, tile_is_selected);
      } else if (board_get_idx(board, idx_base) == '*') {
        print_tile(x, y, e_stalemate_tile, tile_is_selected);
      } else {
        print_tile(x, y, e_empty_tile, tile_is_selected);
      }
      x += tile_width + board_border_width;
    }
    y += tile_height + board_border_height;
  }
  //  if (selected_tile) {
  //    printf("\033[0m");
  //  }
}
void tui_print_game(const struct GameState *game) {
  int i;
  int x = 2, y = 1;

  for (i = 0; i < 21; i++) {
    tui_set_cursor(x, y);
    puts(Board_gfx[i]);
    y += 1;
  }
  debug_flag = false;
  print_meta_board(game);
}

void tui_print_message(const char *message, ...) {
  va_list args;

  va_start(args, message);
  static char buffer[256] = {0};
  vsnprintf(buffer, 80, message, args);
  printf("\033[%d;%dH\033[K%s", screen_y, screen_x, buffer);
  screen_y++;
}

void tui_set_cursor(int x, int y) { printf("\033[%d;%dH", y, x); }

void tui_debug() {
  tui_cls();
  int x = 1, y = 1;
  const int x_offset = 14;
  const int y_offset = 7;
  const int x_start = 4;
  tui_set_cursor(x, y);
  for (int i = 0; i < 21; i++) {
    puts(Board_gfx[i]);
  }

  // printf("wut\n");
  // print_tile(2, 1, e_tile_X_LARGE, false);
  x = x_start;
  for (int i = 0; i < 5; i++) {
    x = 4;
    tui_set_cursor(x, y);
    puts(SmallBoard_gfx[i]);
    y++;
  }

  y = 1;

  tui_read_char();
  x += x_offset;
  y += 0;
  print_tile(x, y, e_tile_X_LARGE, false);
  tui_read_char();
  x += x_offset;
  print_tile(x, y, e_tile_X_LARGE, false);

  tui_read_char();

  x = x_start;
  y += y_offset;

  print_tile(x, y, e_tile_O_LARGE, false);
  x += x_offset;
  tui_read_char();
  print_tile(x, y, e_tile_O_LARGE, false);
  x += x_offset;
  tui_read_char();
  print_tile(x, y, e_tile_O_LARGE, false);

  tui_read_char();

  x = x_start;
  y += y_offset;
  ;

  print_tile(x, y, e_tile_X_LARGE, false);
  tui_read_char();
  x += x_offset;
  print_tile(x, y, e_tile_X_LARGE, false);
  tui_read_char();
  x += x_offset;
  print_tile(x, y, e_tile_X_LARGE, false);

  // printf("huh\n");

  tui_read_char();
}
