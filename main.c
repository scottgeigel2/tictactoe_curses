#include <stdio.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

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


bool tile_available(int coord)
{
	if (coord >= 0 && coord < 10)
	{
		return board[coord] == ' ';
	}
	return false;
}

void place_tile(int coord, int player)
{
	board[coord] = player? 'O' : 'X';
}

int is_winner(){
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
    for (int i = 0; i < 8; i++)
    {
        int a = winning_combinations[i][0];
        int b = winning_combinations[i][1];
        int c = winning_combinations[i][2];

        if (board[a] != ' ' && board[a] == board[b] && board[b] == board[c])
        {
            return board[a] == 'O' ? 1 : 0; // 1 for 'O', 0 for 'X'
        }
    }

    // Check for stalemate
    bool is_stalemate = true;
    for (int i = 0; i < 9; i++)
    {
        if (board[i] == ' ')
        {
            is_stalemate = false;
            break;
        }
    }

    if (is_stalemate)
    {
        return 2; // Stalemate
    }

    return -1; // No winner yet
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
	int player = 0;
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
		if (c == ' '  )
		{
			if (tile_available(coord))
			{
				place_tile(coord, player);
				player = (player + 1) % 2;
			}
			else 
			{
				goto player_input;
			}
		}

		int status = is_winner();

		if (status >= 0)
		{
			cls();
			game_over = true;
		}
		switch (status) 
		{
			case 2:
				printf("Draw\n");
				break;
			case 1:
				printf("O wins\n");
				break;
			case 0:
				printf("X wins\n");
				break;
			default:
				break;
		}

	}
}
