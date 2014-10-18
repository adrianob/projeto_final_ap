#ifndef MR_DO_SPRITES
#define MR_DO_SPRITES

#include "main.h"

void create_ghosts(WINDOW *w, sprite *ghosts, struct position position);
void create_fruits(WINDOW *w, sprite *fruits);
void create_rocks(WINDOW *w, sprite *rocks);
void find_fruits(chtype (*MAP)[MAX_X], sprite *fruits);
int wfind_fruits(WINDOW *w, sprite *fruits);
void print_fruits(WINDOW *w, sprite *fruits);
void print_rocks(WINDOW *w, sprite *rocks);

extern const sprite DEFAULT_GHOST;
extern const sprite DEFAULT_FRUIT;
extern const sprite DEFAULT_ROCK;
extern const sprite DEFAULT_SHOT;
extern const sprite DEFAULT_MR_DO;
extern const sprite DEFAULT_NEST;
extern const sprite DEFAULT_WALL;
extern const sprite DEFAULT_SPACE;

#endif
