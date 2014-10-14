#ifndef MR_DO_SPRITES
#define MR_DO_SPRITES

#include "main.h"

void create_ghosts(WINDOW *w, sprite ghosts[MAX_GHOSTS], struct position position);
void create_fruits(WINDOW *w, sprite fruits[MAX_FRUITS]);
void create_rocks(WINDOW *w, sprite rocks[MAX_ROCKS]);
void find_fruits(chtype MAP[MAX_Y][MAX_X], sprite *fruits);
int wfind_fruits(WINDOW *w, sprite *fruits);
void print_fruits(WINDOW *w, sprite *fruits);

extern const sprite DEFAULT_GHOST;
extern const sprite DEFAULT_FRUIT;
extern const sprite DEFAULT_ROCK;
extern const sprite DEFAULT_SHOT;
extern const sprite DEFAULT_MR_DO;
extern const sprite DEFAULT_NEST;

#endif
