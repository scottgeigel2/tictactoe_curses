#include "tui.h"
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>

// private functions
struct termios set_no_echo();
void clear_no_echo(struct termios* oldt);

// private globals
const char X_Tile[3][12] = {
{"\\   / "}, // \   /
{" | |  "},  //  | |
{"/   \\ "}  // /   \
// this line intentionally left blank
};

const char O_Tile[4][12] = {
{"/ - \\"},  // / - \ .
{"|   |"},   // |   |
{"\\ - /"}   // \ - /
};

const char Empty_Tile[3][12] = {
{"......"},
{"......"},
{"......"}
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


struct termios _termios_backup;

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
}

void print_tile(int x, int y, const char tile[3][12], bool invert)
{
	if (invert)
	{
		printf("\033[7m");
	}

	for (int i = 0; i < 3; i++)
	{

		printf("\033[%d;%dH%s", y, x, tile[i]);
		y++;
	}

	if (invert)
	{
		printf("\033[0m");
	}

}


void tui_print_board(int coord, const char board[9]) {
  int i, j;
  int x, y;
  int idx_base;

  puts("\033[2;1H");
  for (i = 0; i < 16; i++) {
    puts(Board_gfx[i]);
  }

  for (i = 0; i < 3; i++) {
    idx_base = 3 * i;

    for (j = 0; j < 3; j++, idx_base++) {
      y = (i * 5) + 3;
      x = (j * 14) + 3;
      bool selected_tile = ((i * 3) + j) == coord;
      if (board[idx_base] == 'O') {
        print_tile(x, y, O_Tile, selected_tile);
      } else if (board[idx_base] == 'X') {
        print_tile(x, y, X_Tile, selected_tile);
      } else {
        print_tile(x, y, Empty_Tile, selected_tile);
      }
      x += 5;
      y += 4;
    }
  }
}

void tui_print_message(const char *message) {
  const int x = 0;
  const int y = 20;
  printf("\033[%d;%dH\033[K%s", y, x, message);
}

