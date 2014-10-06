#ifndef MR_DO_MOVEMENT
#define MR_DO_MOVEMENT

void print_char(WINDOW *w, sprite* sprite);
void move_ghost(WINDOW *w, struct ghost* gh);
void shoot(WINDOW *w, struct shot* s);
int can_go_to_direction(WINDOW *w, struct position* p, int direction);
void move_sprite(WINDOW *w, sprite* sprite, int direction);
void move_if_possible(WINDOW *w, sprite* s);
void check_collision(WINDOW *w, struct mr_do* md, struct ghost* gh, struct shot* s);

#endif
