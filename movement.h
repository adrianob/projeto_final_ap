#ifndef MR_DO_MOVEMENT
#define MR_DO_MOVEMENT
struct game_state game_state;

void print_char(WINDOW *w, sprite* sprite);
void debug_print(WINDOW *w, sprite* md, sprite* nest);
void move_ghost(WINDOW *w, sprite *gh);
void move_rock(WINDOW *w, sprite *rk);
void shoot(sprite *shot, struct position position, int direction);
void move_shot(WINDOW *w, sprite* s);
int can_go_to_direction(WINDOW *w, sprite *sp, int direction);
int can_fall(WINDOW *w, struct position* p, int direction);
void move_sprite(WINDOW *w, sprite* sprite, int direction);
void move_if_possible(WINDOW *w, sprite* s);
void check_collision(WINDOW *w, sprite *sp1, sprite *sp2);
void check_shot_collision(WINDOW *w, sprite *sp1, sprite *sp2);
void move_ghosts(WINDOW *w, sprite ghosts[MAX_GHOSTS]);
void move_rocks(WINDOW *w, sprite rocks[MAX_ROCKS]);
int next_char(WINDOW *w, struct position *p, int direction);
void check_fruit_collision(WINDOW *w, sprite* sprite, int direction);

#endif
