#include <stdio.h>

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

void cls()
{
	puts("\033[2J\033[H"); // clear screen
}

void print_tile(int x, int y, const char tile[3][12])
{
	for (int i = 0; i < 3; i++)
	{
	
		printf("\033[%d;%dH%s", y, x, tile[i]);	
		y++;
	}

	getchar();
}
void print_board()
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
//			cls();
//			printf("i = %d| j = %d|\nx = %d| y = %d\n'%c'",i,j,x,y, board[idx_base]);
//			getchar();
//			cls();
//			printf("\033[%d;%dH", y, x); // position the cursor
			if (board[idx_base] == 'O') {
				print_tile(x, y, O_Tile);
			} else if (board[idx_base] == 'X') {
				print_tile(x, y, X_Tile);
			} else {
				print_tile(x, y, Empty_Tile);
			}
			x += 5;
			y += 4;
		}
	}

}

int main () {
//	printf(X_Tile);
//	printf("\n");
//	printf(O_Tile);
//	printf("\n");
//
	cls();
	print_board();
	getchar();
	cls();
	board[0] = board[2] = board[7] = 'X';
	board[1] = board[3] = board[6] = 'O';
	print_board();
}
