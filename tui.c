#include "tui.h"
#include <stdarg.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>

//type defs
enum e_tile {
  e_tile_EMPTY_SMALL,
  e_tile_X_SMALL,
  e_tile_O_SMALL,
  e_tile_EMPTY_LARGE,
  e_tile_X_LARGE,
  e_tile_O_LARGE,
};


// private variables
struct termios set_no_echo();
bool big_mode = true;
bool debug_flag = false;
// private functions
void clear_no_echo(struct termios* oldt);

// private globals
const char X_Tile[3][7] = {
{" \\  / "}, // \   /
{"  ||  "},  //  | |
{" /  \\ "}  // /   \
// this line intentionally left blank
};

const char O_Tile[3][7] = {
{" / - \\"},  // / - \ .
{" |   |"},   // |   |
{" \\ - /"}   // \ - /
};

const char Empty_Tile[3][7] = {
{"......"},
{"......"},
{"......"}
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

static int x = 0;
static int y = 20;

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

char tui_read_char()
{
	// Read a single character
	char ch = getchar();
	return ch;
}

void tui_cls()
{
	puts("\033[2J\033[H"); // clear screen
  y = 20;
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
  if (tile_selection == e_tile_X_LARGE || tile_selection == e_tile_O_LARGE || tile_selection == e_tile_EMPTY_LARGE)
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
    else //empty
    {
        printf("\033[%d;%dH ", y, x);
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
  int x, y;
  int idx_base;
  int width_factor, height_factor, width_offset, height_offset;
  enum e_tile e_xtile, e_otile, e_empty_tile;

  if (big_mode)
  {
    e_xtile = e_tile_X_LARGE;
    e_otile = e_tile_O_LARGE;
    e_empty_tile = e_tile_EMPTY_LARGE;
    width_factor = 14;
    height_factor = 5;
    width_offset = 3;
    height_offset = 3;
  }
  else
  {
     e_xtile = e_tile_X_SMALL;
    e_otile = e_tile_O_SMALL;
    e_empty_tile = e_tile_EMPTY_SMALL;
    width_factor = 1;
    height_factor = 1;
    width_offset = 1;
    height_offset = 1;
  }

  puts("\033[2;1H");
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

    for (j = 0; j < 3; j++, idx_base++) {
      p_debug_buf += sprintf(p_debug_buf, "(%d %d)\t", x, y);

      y = (i * height_factor) + height_offset;
      x = (j * width_factor) + width_offset;
      bool selected_tile = ((i * 3) + j) == coord;
      if (board_get_idx(board, idx_base) == 'O') {
        print_tile(x, y, e_otile, selected_tile);
      } else if (board_get_idx(board, idx_base) == 'X') {
        print_tile(x, y, e_xtile, selected_tile);
      } else {
        print_tile(x, y, e_empty_tile, selected_tile);
      }
      y += 4;
    }
    p_debug_buf += sprintf(p_debug_buf, "\n");
  }
  tui_print_message(debug_buf);
}

void tui_print_message(const char *message, ...) {
  va_list args;

  va_start(args, message);
  static char buffer[256] = {0};
  vsnprintf(buffer, 80, message, args);
  printf("\033[%d;%dH\033[K%s", y, x, buffer);
  y++;
}

