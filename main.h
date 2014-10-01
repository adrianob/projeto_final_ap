#ifndef MR_DO_MAIN
#define MR_DO_MAIN

#define MAX_X 80 //tamanho de colunas do mapa principal
#define MAX_Y 30 //tamanho de linhas do mapa principal
#define MAX_GHOSTS 1
#define INTERVAL 500000
void init(void);
void draw_borders(void);
void timer_handler();
struct moving_element {
  int x, y;
};

struct mr_do {
  struct moving_element position;
	int last_x, last_y, state;
  char representation;
};

struct ghost {
  struct moving_element position;
  int state;
  char representation;
};

#endif
