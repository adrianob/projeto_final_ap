#ifndef MR_DO_MOVEMENT
#define MR_DO_MOVEMENT
struct game_state game_state;

void print_char(WINDOW *w, sprite* sprite);
void move_ghost(WINDOW *w, struct ghost* gh);
void shoot(WINDOW *w, struct shot* s);
int can_go_to_direction(WINDOW *w, struct position* p, int direction);
void move_sprite(WINDOW *w, sprite* sprite, int direction);
void move_if_possible(WINDOW *w, sprite* s);
void check_collision(WINDOW *w, sprite *sp1, sprite *sp2);
void check_shot_collision(WINDOW *w, sprite *sp1, sprite *sp2);
void move_ghosts(WINDOW *w, struct ghost ghosts[MAX_GHOSTS]);
int next_char(WINDOW *w, struct position* p, int direction);
void check_fruit_collision(WINDOW *w, sprite* sprite, int direction);

#endif
