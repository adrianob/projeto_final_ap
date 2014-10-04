#ifndef MR_DO_MAIN
#define MR_DO_MAIN

#define MAX_X 60 //tamanho de colunas do mapa principal
#define MAX_Y 23 //tamanho de linhas do mapa principal
#define MAX_GHOSTS 1
#define INTERVAL 150000
#define UP_DIRECTION 1
#define DOWN_DIRECTION 2
#define RIGHT_DIRECTION 3
#define LEFT_DIRECTION 4

#include <stdio.h>
#include <ncurses.h>
void config(void);
void draw_map(WINDOW *w, char MAP[MAX_Y][MAX_X]);
void timer_handler(void);
void config_timer(void);
void play_level_one(void);
void show_menu(void);

struct position {
  int x, y, last_x, last_y;
};

typedef struct {
  struct position position;
  int state;//vivo ou morto
  int representation;
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

#endif
