#ifndef MR_DO_MAIN
#define MR_DO_MAIN

#define MAX_X 60 //tamanho de colunas do mapa principal
#define MAX_Y 23 //tamanho de linhas do mapa principal
#define MAX_GHOSTS 1
#define INTERVAL 500000
#include <stdio.h>
void init(void);
void draw_borders(void);
void timer_handler();

struct moving_element {
  int x, y, last_x, last_y;
};

struct mr_do {
  struct moving_element position;
	int state;
  char representation;
};

struct ghost {
  struct moving_element position;
  int state;
  char representation;
};

#endif
