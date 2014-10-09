#ifndef MR_DO_MAIN
#define MR_DO_MAIN

#define MAX_X 60 //tamanho de colunas do mapa principal
#define MAX_Y 23 //tamanho de linhas do mapa principal
#define MAX_GHOSTS 10
#define INTERVAL 100000
#define GHOST_INTERVAL 3000000
#define UP_DIRECTION 1
#define DOWN_DIRECTION 2
#define RIGHT_DIRECTION 3
#define LEFT_DIRECTION 4
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#include <stdio.h>
#include <ncurses.h>
void config(void);
void draw_map(WINDOW *w, chtype MAP[MAX_Y][MAX_X]);
void timer_handler(int i);
void config_timer(void);
void play_level_one(void);
void show_menu(void);
struct position find_char(chtype MAP[MAX_Y][MAX_X], int ch);
void refresh_windows(WINDOW *info_window, WINDOW *game_window, WINDOW *border_window);

struct position {
  int x, y, last_x, last_y;
};

typedef struct {
  struct position position;
  int alive;//vivo ou morto
  chtype representation;
  int direction;
  /*
    1 - cima
    2 - baixo
    3 - direita
    4 - esquerda
   * */
} sprite;

struct mr_do {
  sprite sprite;
};

struct ghost {
  sprite sprite;
};

struct shot {
  sprite sprite;
};

struct game_state {
  int score;
};

void create_ghosts(WINDOW *w, struct ghost ghosts[MAX_GHOSTS], struct position position);

extern const sprite DEFAULT_GHOST;
extern const sprite DEFAULT_SHOT;
extern const sprite DEFAULT_MR_DO;
extern const sprite DEFAULT_NEST;

#endif
