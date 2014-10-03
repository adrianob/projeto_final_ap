#ifndef MR_DO_MOVEMENT
#define MR_DO_MOVEMENT

void print_char(WINDOW *w, struct sprite* sprite);
void move_ghost(WINDOW *w, struct ghost* gh);
void shoot(WINDOW *w, struct shot* s);
int can_go_to_direction(WINDOW *w, struct position* p, int direction);
void move_sprite(WINDOW *w, struct sprite* sprite, int direction);
void move_if_possible(WINDOW *w, struct sprite* s);

#endif
