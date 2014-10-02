#ifndef MR_DO_MOVEMENT
#define MR_DO_MOVEMENT

void print_char(WINDOW *w, struct moving_element* position, int c);
void move_right(WINDOW *w, struct moving_element* position, int c);
void move_left(WINDOW *w, struct moving_element* position, int c);
void move_up(WINDOW *w, struct moving_element* position, int c);
void move_down(WINDOW *w, struct moving_element* position, int c);

#endif
