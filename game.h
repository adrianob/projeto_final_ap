#ifndef MR_DO_GAME
#define MR_DO_GAME

#include "main.h"

int timer_ready;

void config(void);
void timer_handler(int i);
void config_timer(void);
void play(void);
void continue_play(void);
void refresh_windows(WINDOW *info_window, WINDOW *game_window, WINDOW *border_window);
void check_state(WINDOW *info, struct sprite_list sl);

#endif
