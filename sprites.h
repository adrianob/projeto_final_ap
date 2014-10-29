#ifndef MR_DO_SPRITES
#define MR_DO_SPRITES

#include "main.h"
#include "lists.h"
struct game_state game_state;

extern const SPRITE DEFAULT_GHOST;
extern const SPRITE DEFAULT_FRUIT;
extern const SPRITE DEFAULT_ROCK;
extern const SPRITE DEFAULT_SHOT;
extern const SPRITE DEFAULT_MR_DO;
extern const SPRITE DEFAULT_NEST;
extern const SPRITE DEFAULT_WALL;
extern const SPRITE DEFAULT_SPACE;

void make_lists(chtype (*MAP)[MAX_X], struct sprite_list *sl);
void check_sprite_collision(struct sprite_list *sl);
void create_rocks(WINDOW *w, SPRITE *rocks);
void create_ghost(struct sprite_list *sl);
void create_shot(struct sprite_list *sl);
int collided(SPRITE *current, SPRITE *sp);
#endif
