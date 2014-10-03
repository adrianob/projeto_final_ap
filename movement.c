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


//@TODO fazer algo menos tosco
int can_go_up(WINDOW *w, struct ghost* gh){
  int next_ch = mvwinch(w, gh->position.y - 1, gh->position.x );
  return !(next_ch == '#' || next_ch == -1);
}

int can_go_down(WINDOW *w, struct ghost* gh){
  int next_ch = mvwinch(w, gh->position.y + 1, gh->position.x );
  return !(next_ch == '#' || next_ch == -1);
}

int can_go_left(WINDOW *w, struct ghost* gh){
  int next_ch = mvwinch(w, gh->position.y, gh->position.x - 1 );
  return !(next_ch == '#' || next_ch == -1);
}

int can_go_right(WINDOW *w, struct ghost* gh){
  int next_ch = mvwinch(w, gh->position.y, gh->position.x + 1);
  return !(next_ch == '#' || next_ch == -1);
}

void move_ghost(WINDOW *w, struct ghost* gh){
  switch (gh->current_direction) {
    case 1: //cima
      if (can_go_up(w, gh)) {
        move_up(w, &gh->position, gh->representation);
        gh->current_direction = 1;
      }
      else if (can_go_down(w, gh)) {
        move_down(w, &gh->position, gh->representation);
        gh->current_direction = 2;
      }
      else if (can_go_right(w, gh)) {
        move_right(w, &gh->position, gh->representation);
        gh->current_direction = 3;
      }
      else if (can_go_left(w, gh)) {
        move_left(w, &gh->position, gh->representation);
        gh->current_direction = 4;
      }
      break;
    case 2: //baixo
      if (can_go_down(w, gh)) {
        move_down(w, &gh->position, gh->representation);
        gh->current_direction = 2;
      }
      else if (can_go_up(w, gh)) {
        move_up(w, &gh->position, gh->representation);
        gh->current_direction = 1;
      }
      else if (can_go_right(w, gh)) {
        move_right(w, &gh->position, gh->representation);
        gh->current_direction = 3;
      }
      else if (can_go_left(w, gh)) {
        move_left(w, &gh->position, gh->representation);
        gh->current_direction = 4;
      }
      break;
    case 3: //direita
      if (can_go_right(w, gh)) {
        move_right(w, &gh->position, gh->representation);
        gh->current_direction = 3;
      }
      else if (can_go_down(w, gh)) {
        move_down(w, &gh->position, gh->representation);
        gh->current_direction = 2;
      }
      else if (can_go_up(w, gh)) {
        move_up(w, &gh->position, gh->representation);
        gh->current_direction = 1;
      }
      else if (can_go_left(w, gh)) {
        move_left(w, &gh->position, gh->representation);
        gh->current_direction = 4;
      }
      break;
    case 4: //esquerda
      if (can_go_left(w, gh)) {
        move_left(w, &gh->position, gh->representation);
        gh->current_direction = 4;
      }
      else if (can_go_down(w, gh)) {
        move_down(w, &gh->position, gh->representation);
        gh->current_direction = 2;
      }
      else if (can_go_right(w, gh)) {
        move_right(w, &gh->position, gh->representation);
        gh->current_direction = 3;
      }
      else if (can_go_up(w, gh)) {
        move_up(w, &gh->position, gh->representation);
        gh->current_direction = 1;
      }
      break;
  }
 
}
#endif
