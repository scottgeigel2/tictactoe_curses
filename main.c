#include <stdio.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

const char X_Tile[3][12] = {
{"\\   / "}, // \   /
{" | |"},    //  | |  
{"/   \\ "}  // /   \
// this line intentionally left blank
};

const char O_Tile[4][12] = {
{"/ - \\"},  // / - \ .
{"|   |"}, // |   |
{"\\ - /"}   // \ - /
};

const char Empty_Tile[3][12] = {
{"......"},
{"......"},
{"......"}
};


const char Board_gfx[16][43] = {
{"            ||            ||           "},
{"            ||            ||           "},
{"            ||            ||           "},
{"____________||____________||___________"},
{"____________||____________||___________"},
{"                                       "},
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

};

char board[9] = "         ";

char read_char()
{
	struct termios oldt, newt;
	char ch;

	// Get current terminal attributes
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;

	// Disable canonical mode and echo
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	// Read a single character
	ch = getchar();

	// Restore old terminal attributes
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}

void cls()
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
void print_board(int coord)
{
	int i,j,k,l;
	int x,y;
	int idx_base;

	cls();
	puts("\033[2;1H");
	for (i = 0; i < 13; i++)
	{
		puts(Board_gfx[i]);
	}

	for (i = 0; i < 3; i++)
	{
		idx_base = 3 * i;
	
		for (j = 0; j < 3; j++,idx_base++)
		{
			y = i * 6 + 2;
			x = j * 14 + 3;
  			bool selected_tile = ((i * 3) + j) == coord;
			if (board[idx_base] == 'O') 
			{
				print_tile(x, y, O_Tile, selected_tile);
			} else if (board[idx_base] == 'X')
			{
				print_tile(x, y, X_Tile, selected_tile);
			} else 
			{
				print_tile(x, y, Empty_Tile, selected_tile);
			}
			x += 5;
			y += 4;
		}
	}

}

int main () {
//
	char debug_last_read = 0;
	bool game_over = false;
	int x = 0;
	int y = 0;
	while (!game_over)
	{
		int coord = (y * 3) + x;
		cls();
		print_board(coord);
player_input:
		char c = read_char();
		debug_last_read = c;

		if (c == 'j') 
		{
			if ((y+1) >= 3)
			{
				goto player_input;
			}
			y++;
		}
		if (c == 'k') 
		{
			if ((y-1) < 0)
			{
				goto player_input;
			}
			y--;
		}
		if (c == 'l') 
		{
			if ((x+1) >= 3)
			{
				goto player_input;
			}
			x++;
		}
		if (c == 'h') 
		{
			if ((x-1) < 0)
			{
				goto player_input;
			}
			x--;
		}
	}

	cls();
	board[0] = board[2] = board[7] = 'X';
	board[1] = board[3] = board[6] = 'O';
	print_board(0);
}
