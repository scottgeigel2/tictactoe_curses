#include "tui.h"
#include <stdarg.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <assert.h> // TODO: this is being used for an assertion... consider an assertion header if
                    // we assert more

//type defs
enum e_tile {
  e_tile_EMPTY_SMALL,
  e_tile_X_SMALL,
  e_tile_O_SMALL,
  e_tile_S_SMALL, // TODO: this doesn't actually exist.... or does it? maybe in meta-meta-tictactoe?
  e_tile_EMPTY_LARGE,
  e_tile_X_LARGE,
  e_tile_O_LARGE,
  e_tile_S_LARGE
};


// private variables
struct termios set_no_echo();
bool big_mode = true;
bool debug_flag = false;
// private functions
void clear_no_echo(struct termios* oldt);
bool e_tile_is_large(enum e_tile tile);

// private globals
const char X_Tile[3][7] = {
{"\\\\  //"}, // \   /
{" || ||"},  //  | |
{"//  \\\\"}  // /   \
// this line intentionally left blank
};

const char O_Tile[3][7] = {
{"/ -- \\"},  // / - \ .
{"|    |"},   // |   |
{"\\ -- /"}   // \ - /
};

const char Empty_Tile[3][7] = {
{"......"},
{"......"},
{"......"}
};

const char Stalemate_Tile[3][7] = {
{"******"},
{"******"},
{"******"}
};


const char SmallBoard_gfx[5][6] = {
  {" | | "},
  {"-----"},
  {" | | "},
  {"-----"},
  {" | | "},
};

const char Board_gfx[16][41] = {
{"            ||            ||           "},
{"            ||            ||           "},
{"            ||            ||           "},
{"____________||____________||___________"},
{"____________||____________||___________"},
{"            ||            ||           "},
{"            ||            ||           "},
{"            ||            ||           "},
{"____________||____________||___________"},
{"____________||____________||___________"},
{"            ||            ||           "},
{"            ||            ||           "},
{"            ||            ||           "},
{"            ||            ||           "},

};

static int screen_x = 0;
static int screen_y = 20;

static struct termios _termios_backup;

void tui_init()
{
	_termios_backup = set_no_echo();
}

void tui_deinit()
{
	clear_no_echo(&_termios_backup);
}

struct termios set_no_echo()
{
	struct termios oldt, newt;

	// Get current terminal attributes
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;

	// Disable canonical mode and echo
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	return oldt;
}

void clear_no_echo(struct termios* oldt)
{
	// Restore old terminal attributes
	tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}

bool e_tile_is_large(enum e_tile tile)
{
  switch (tile)
  {
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
char tui_read_char()
{
	// Read a single character
	char ch = getchar();
	return ch;
}

void tui_cls()
{
	puts("\033[2J\033[H"); // clear screen
  screen_y = 20;
}

void tui_small_mode()
{
  big_mode = false;
}

void tui_big_mode()
{
  big_mode = true;
}


void print_tile(int x, int y, enum e_tile tile_selection, bool invert)
{
	if (invert)
	{
		printf("\033[7m");
	}
  if (e_tile_is_large(tile_selection))
  {
    for (int i = 0; i < 3; i++)
    {
      if (false && !debug_flag)
      {
        debug_flag = tui_read_char() == 'q';
      }
      if (tile_selection == e_tile_X_LARGE)
      {
        printf("\033[%d;%dH%s", y, x, X_Tile[i]);
      }
      else if(tile_selection == e_tile_O_LARGE)
      {
        printf("\033[%d;%dH%s", y, x, O_Tile[i]);
      }
      else if(tile_selection == e_tile_S_LARGE)
      {
        printf("\033[%d;%dH%s", y, x, Stalemate_Tile[i]);
      }
      else //empty
      {
        printf("\033[%d;%dH%s", y, x, Empty_Tile[i]);
      }
      y++;
    }
  }
  else // one of the small tyles
  {
    if (tile_selection == e_tile_X_SMALL)
    {
        printf("\033[%d;%dHX", y, x);
    }
    else if (tile_selection == e_tile_O_SMALL)
    {
        printf("\033[%d;%dHO", y, x);
    }
    else if (tile_selection == e_tile_S_SMALL)
    {
        printf("\033[%d;%dH*", y, x);
    }
    else //empty
    {
        printf("\033[%d;%dH.", y, x);
    }
  }
	if (invert)
	{
		printf("\033[0m");
	}

}


void tui_print_board(int coord, const Board *board) {
  char debug_buf [120];
  char *p_debug_buf = debug_buf;
  int i, j;
  int x = 1, y = 1;
  int idx_base;
  int tile_width, tile_height; // TODO: width has so much space padding.... why not height?
  int board_border_width, board_border_height; // TODO: currently much justification to have these separate,
                                               // but maybe one day i'll have a border who's height is not
                                               // equal to its width
  int tile_padding_left, tile_padding_right; // TODO: effectively the same number ATM, but maybe one
                                             // day I might need this? if not, just change it to a
                                             // simple padding var

  //TODO: also IDK, maybe vertical padding????


  enum e_tile e_xtile, e_otile, e_stalemate_tile, e_empty_tile;

  if (big_mode)
  {
    e_xtile = e_tile_X_LARGE;
    e_otile = e_tile_O_LARGE;
    e_stalemate_tile = e_tile_S_LARGE;
    e_empty_tile = e_tile_EMPTY_LARGE;
    tile_width = 12;
    tile_height = 3;
    board_border_width = 2;
    board_border_height = 2;
    tile_padding_left = 3;
    tile_padding_right = 3;
  }
  else
  {
    e_xtile = e_tile_X_SMALL;
    e_otile = e_tile_O_SMALL;
    e_stalemate_tile = e_tile_S_SMALL;
    e_empty_tile = e_tile_EMPTY_SMALL;
    tile_width = 1;
    tile_height = 1;
    board_border_width = 1;
    board_border_height = 1;
    tile_padding_left = 0;
    tile_padding_right = 0;
  }

  printf("\033[%d;%dH", x, y);
  if (big_mode)
  {
    for (i = 0; i < 16; i++) {
      puts(Board_gfx[i]);
    }
  }
  else
  {
    for (i = 0; i < 5; i++)
    {
      puts(SmallBoard_gfx[i]);
    }
  }

  debug_flag = false;
  for (i = 0; i < 3; i++) {
    idx_base = 3 * i;

    x = 1 + tile_padding_left;
    for (j = 0; j < 3; j++, idx_base++) {
      p_debug_buf += sprintf(p_debug_buf, "(%d %d)\t", x, y);

      bool selected_tile = ((i * 3) + j) == coord;
      if (board_get_idx(board, idx_base) == 'O') {
        print_tile(x, y, e_otile, selected_tile);
      } else if (board_get_idx(board, idx_base) == 'X') {
        print_tile(x, y, e_xtile, selected_tile);
      } else if (board_get_idx(board, idx_base) == '*') {
        print_tile(x, y, e_stalemate_tile, selected_tile);
      } else {
        print_tile(x, y, e_empty_tile, selected_tile);
      }
      x += tile_width + board_border_width;
    }
    y += tile_height + board_border_height;
    p_debug_buf += sprintf(p_debug_buf, "\n");
  }
  tui_print_message(debug_buf);
}

void tui_print_message(const char *message, ...) {
  va_list args;

  va_start(args, message);
  static char buffer[256] = {0};
  vsnprintf(buffer, 80, message, args);
  printf("\033[%d;%dH\033[K%s", screen_y, screen_x, buffer);
  screen_y++;
}

