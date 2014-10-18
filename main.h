#ifndef MR_DO_MAIN
#define MR_DO_MAIN

#define MAX_X 60 //tamanho de colunas do mapa principal
#define MAX_Y 23 //tamanho de linhas do mapa principal
#define MAX_GHOSTS 10
#define MAX_FRUITS 32
#define MAX_ROCKS 8
#define INTERVAL 100000
#define GHOST_INTERVAL 3000000
#define ROCK_INTERVAL 100000
#define UP_DIRECTION 1
#define DOWN_DIRECTION 2
#define RIGHT_DIRECTION 3
#define LEFT_DIRECTION 4
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#define CH_MR_DO (123 | A_ALTCHARSET | COLOR_PAIR(1))
#define CH_FRUIT (213 | A_ALTCHARSET | COLOR_PAIR(2))
#define CH_ROCK  (164 | A_ALTCHARSET | COLOR_PAIR(3))
#define CH_WALL  (97  | A_ALTCHARSET | COLOR_PAIR(4))
#define CH_NEST  (110 | A_ALTCHARSET)
#define CH_GHOST (214 | A_ALTCHARSET)
#define CH_SHOT  (183 | A_ALTCHARSET)

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <ncurses.h>
#include <locale.h>

struct position {
  int x, y, last_x, last_y;
};

typedef struct sprite {
  struct sprite *next;
  struct position position;
  int alive;//vivo ou morto
  chtype representation;
  int direction;
  int falling;
  /*
    1 - cima
    2 - baixo
    3 - direita
    4 - esquerda
   * */
} sprite;

struct game_state {
  int score;
  int level;
};

struct sprite_list {
  sprite *walls, *fruits , *ghosts, *spaces, *mr_do, *nest, *shot;
};

void config(void);
void timer_handler(int i);
void config_timer(void);
void play(void);
void refresh_windows(WINDOW *info_window, WINDOW *game_window, WINDOW *border_window);
void check_state(WINDOW *info, struct sprite_list sl);

#endif
