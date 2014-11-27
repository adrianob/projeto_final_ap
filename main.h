#ifndef MR_DO_MAIN
#define MR_DO_MAIN

#define MAX_X 60 //tamanho de colunas do mapa principal
#define MAX_Y 23 //tamanho de linhas do mapa principal
#define MAX_GHOSTS 10
#define MAX_FRUITS 32
#define INTERVAL 100000
#define GHOST_INTERVAL 3000000
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

//define o caractere e a cor de cada sprite
#define CH_MR_DO (123 | A_ALTCHARSET | COLOR_PAIR(1))
#define CH_FRUIT (240 | A_ALTCHARSET | COLOR_PAIR(2))
#define CH_WALL  (97  | A_ALTCHARSET | COLOR_PAIR(4))
#define CH_NEST  (110 | A_ALTCHARSET)
#define CH_GHOST (214 | A_ALTCHARSET)
#define CH_SHOT  (183 | A_ALTCHARSET)
#define CH_SPACE  ' '

#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>
#include <locale.h>

#define MAX_NAME 40
#define TOP_SCORES 5
//guarda a posição atual e anterior na tela
struct position {
  int x, y, last_x, last_y;
};

enum direction{
  up,
  down,
  right,
  left
};

//abstração usada para todos elementos mostrados na tela
//um struct sprite_node é uma lista encadeada de SPRITE, que guarda informações sobre cada elemento do jogo
typedef struct sprite_node {
  struct sprite_node *next;
  struct position position;
  int alive;//vivo ou morto
  chtype representation;
  enum direction direction;
} SPRITE;

//usado para guardar informações sobre o estado do jogo em si
struct game_state {
  int score;
  int level;
  int saved_game;
};

//guarda ponteiros para cada tipo de sprite usado no jogo, que por sua vez é uma lista encadeado com 1 ou mais elementos
struct sprite_list {
  SPRITE *walls,
         *fruits,
         *ghosts,
         *spaces,
         *mr_do,
         *nest,
         *shot;
};

typedef int (*compfn)(const void*, const void*);

struct score {
  char name[MAX_NAME];
  int score;
};

extern int timer_ready;
struct game_state game_state;

#endif
