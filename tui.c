#include "tui.h"
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>


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

char board[9] = "         ";

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

char read_char()
{
	// Read a single character
	char ch = getchar();
	return ch;
}

void tui_cls()
{
	puts("\033[2J\033[H"); // clear screen
}

void tui_print_tile(int x, int y, const char tile[3][12], bool invert)
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





