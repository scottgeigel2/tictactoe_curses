#ifndef __TUI_H__
#define __TUI_H__
#include "game.h"
#include <stdarg.h>
#include <stdbool.h>
#define tui_NOT_A_COORD -1
void tui_init();
void tui_deinit();
void tui_cls();
void tui_small_mode();
void tui_big_mode();
void tui_print_game(const struct GameState *game);
char tui_read_char();
void tui_print_message(const char *message, ...);
void tui_debug();
#endif
