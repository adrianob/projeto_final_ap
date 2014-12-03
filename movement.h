#ifndef MR_DO_MOVEMENT
#define MR_DO_MOVEMENT

#include "path.h"
void move_ghost(WINDOW *w, SPRITE *gh, NODE map_node[MAX_Y][MAX_X], struct position destiny);
void move_shot(WINDOW *w, SPRITE *s);
int can_go_to_direction(WINDOW *w, SPRITE sp, enum direction direction);
int move_sprite(WINDOW *w, SPRITE *sprite, enum direction direction);
void move_ghosts(WINDOW *w, SPRITE *ghosts);
int next_char(WINDOW *w, struct position p, enum direction direction);
enum direction get_keyboard_direction(chtype ch);
int valid_key(chtype ch);

#endif
