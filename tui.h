#ifndef __TUI_H__
#define __TUI_H__
#include <stdbool.h>
void tui_init();
void tui_deinit();
void tui_cls();
void tui_print_tile(int x, int y, const char tile[3][12], bool invert);
#endif
