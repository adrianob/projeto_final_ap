#ifndef MR_DO_MOVEMENT
#define MR_DO_MOVEMENT

#include "main.h"
#include "movement.h"
#include <ncurses.h>
#include <unistd.h>

void print_char(WINDOW *w, struct moving_element* position, int c){
  mvwaddch(w, position->last_y, position->last_x, ' ');
  mvwaddch(w, position->y, position->x, c);
}

void move_right(WINDOW *w, struct moving_element* position, int c){
  position->last_x = position->x;
  position->last_y = position->y;
  if (position->x < MAX_X - 1) {
    position->x++;
  }
  print_char(w, position, c);
}

void move_left(WINDOW *w, struct moving_element* position, int c){
  position->last_x = position->x;
  position->last_y = position->y;
  if (position->x > 0) {
    position->x--;
  }
  print_char(w, position, c);
}

void move_up(WINDOW *w, struct moving_element* position, int c){
  position->last_x = position->x;
  position->last_y = position->y;
  if (position->y > 0) {
    position->y--;
  }
  print_char(w, position, c);
}

void move_down(WINDOW *w, struct moving_element* position, int c){
  position->last_x = position->x;
  position->last_y = position->y;
  if (position->y < MAX_Y - 1) {
    position->y++;
  }
  print_char(w, position, c);
}

#endif
