#ifndef MR_DO_MOVEMENT
#define MR_DO_MOVEMENT

#include "main.h"
#include "movement.h"
void move_right(struct moving_element* position){
  if (position->x < MAX_X - 1) {
    position->x++;
  }
}

void move_left(struct moving_element* position){
  if (position->x > 0) {
    position->x--;
  }
}

void move_up(struct moving_element* position){
  if (position->y > 0) {
    position->y--;
  }
}

void move_down(struct moving_element* position){
  if (position->y < MAX_Y - 1) {
    position->y++;
  }
}

#endif
