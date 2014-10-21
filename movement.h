#ifndef MR_DO_MOVEMENT
#define MR_DO_MOVEMENT
struct game_state game_state;

void move_ghost(WINDOW *w, sprite *gh);
//void move_rock(WINDOW *w, sprite *rk);
//void move_rocks(WINDOW *w, sprite *rocks);
void move_shot(WINDOW *w, sprite* s);
int can_go_to_direction(WINDOW *w, sprite sp, int direction);
int can_fall(WINDOW *w, struct position* p, int direction);
int move_sprite(WINDOW *w, sprite* sprite, int direction);
void move_ghosts(WINDOW *w, sprite *ghosts);
int next_char(WINDOW *w, struct position p, int direction);
int get_keyboard_direction(chtype ch);

#endif
