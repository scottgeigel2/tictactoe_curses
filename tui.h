#ifndef __TUI_H__
#define __TUI_H__
#include <stdbool.h>
#include "board.h"
void tui_init();
void tui_deinit();
void tui_cls();
void tui_print_board(int coord, const Board *board);
char tui_read_char();
void tui_print_message(const char *message);
#endif
