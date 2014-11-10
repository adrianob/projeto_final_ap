#ifndef MR_DO_MOVEMENT
#define MR_DO_MOVEMENT

void move_ghost(WINDOW *w, SPRITE *gh);
//void move_rock(WINDOW *w, sprite *rk);
//void move_rocks(WINDOW *w, sprite *rocks);
void move_shot(WINDOW *w, SPRITE *s);
int can_go_to_direction(WINDOW *w, SPRITE sp, int direction);
int can_fall(WINDOW *w, struct position* p, int direction);
int move_sprite(WINDOW *w, SPRITE *sprite, int direction);
void move_ghosts(WINDOW *w, SPRITE *ghosts);
int next_char(WINDOW *w, struct position p, int direction);
enum direction get_keyboard_direction(chtype ch);
int valid_key(chtype ch);

#endif
