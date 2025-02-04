#include "tui.h"
#include <stdarg.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>

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


void print_tile(int x, int y, const char tile[3][7], bool invert)
{
	if (invert)
	{
		printf("\033[7m");
	}

	for (int i = 0; i < 3; i++)
	{
    if (!debug_flag)
    {
      debug_flag = tui_read_char() == 'q';
    }

		printf("\033[%d;%dH%s", y, x, tile[i]);
		y++;
	}

	if (invert)
	{
		printf("\033[0m");
	}

}


void tui_print_board(int coord, const Board *board) {
  int i, j;
  int x, y;
  int idx_base;
  int width_factor, height_factor, width_offset, height_offset;
  const char (*p_xtile)[3][7], (*p_otile)[3][7], (*p_empty_tile)[3][7];
  const char (*p_board)[16][41];

  if (big_mode)
  {
    p_board = &Board_gfx;
    p_xtile = &X_Tile;
    p_otile = &O_Tile;
    p_empty_tile = &Empty_Tile;
    width_factor = 14;
    height_factor = 5;
    width_offset = 3;
    height_offset = 3;
  }
  else
  {
    p_board = &SmallBoard_gfx;
    p_xtile = &"o";
    p_otile = &"x";
    p_empty_tile = &" ";
    width_factor = 14;
    height_factor = 5;
    width_offset = 3;
    height_offset = 3;
  }

  puts("\033[2;1H");
  for (i = 0; i < 16; i++) {
    puts((*p_board)[i]);
  }

  debug_flag = false;
  for (i = 0; i < 3; i++) {
    idx_base = 3 * i;

    for (j = 0; j < 3; j++, idx_base++) {
      y = (i * height_factor) + height_offset;
      x = (j * width_factor) + width_offset;
      bool selected_tile = ((i * 3) + j) == coord;
      if (board_get_idx(board, idx_base) == 'O') {
        print_tile(x, y, *p_otile, selected_tile);
      } else if (board_get_idx(board, idx_base) == 'X') {
        print_tile(x, y, *p_xtile, selected_tile);
      } else {
        print_tile(x, y, *p_empty_tile, selected_tile);
      }
      x += 5;
      y += 4;
    }
  }
}

void tui_print_message(const char *message, ...) {
  va_list args;

  va_start(args, message);
  char buffer[80] = {0};
  vsnprintf(buffer, 80, message, args);
  printf("\033[%d;%dH\033[K%s", y, x, buffer);
  y++;
}

