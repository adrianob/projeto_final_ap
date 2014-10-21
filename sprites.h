#ifndef MR_DO_SPRITES
#define MR_DO_SPRITES

#include "main.h"
#include "lists.h"
struct game_state game_state;

extern const sprite DEFAULT_GHOST;
extern const sprite DEFAULT_FRUIT;
extern const sprite DEFAULT_ROCK;
extern const sprite DEFAULT_SHOT;
extern const sprite DEFAULT_MR_DO;
extern const sprite DEFAULT_NEST;
extern const sprite DEFAULT_WALL;
extern const sprite DEFAULT_SPACE;

void make_lists(chtype (*MAP)[MAX_X], struct sprite_list *sl);
void check_sprite_collision(struct sprite_list *sl, sprite *sprite);
void check_ghosts_collision(struct sprite_list *sl, sprite *sp);
void create_rocks(WINDOW *w, sprite *rocks);
void create_ghost(struct sprite_list *sl);
void create_shot(struct sprite_list *sl);
int collided(sprite *current, sprite *sp);
#endif
