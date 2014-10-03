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
  position->current_direction = 3;
}

void move_left(WINDOW *w, struct moving_element* position, int c){
  position->last_x = position->x;
  position->last_y = position->y;
  if (position->x > 0) {
    position->x--;
  }
  print_char(w, position, c);
  position->current_direction = 4;
}

void move_up(WINDOW *w, struct moving_element* position, int c){
  position->last_x = position->x;
  position->last_y = position->y;
  if (position->y > 0) {
    position->y--;
  }
  print_char(w, position, c);
  position->current_direction = 1;
}

void move_down(WINDOW *w, struct moving_element* position, int c){
  position->last_x = position->x;
  position->last_y = position->y;
  if (position->y < MAX_Y - 1) {
    position->y++;
  }
  print_char(w, position, c);
  position->current_direction = 2;
}

//@TODO fazer algo menos tosco
int can_go_up(WINDOW *w, struct moving_element* m){
  int next_ch = mvwinch(w, m->y - 1, m->x );
  return !(next_ch == '#' || next_ch == -1);
}

int can_go_down(WINDOW *w, struct moving_element* m){
  int next_ch = mvwinch(w, m->y + 1, m->x );
  return !(next_ch == '#' || next_ch == -1);
}

int can_go_left(WINDOW *w, struct moving_element* m){
  int next_ch = mvwinch(w, m->y, m->x - 1 );
  return !(next_ch == '#' || next_ch == -1);
}

int can_go_right(WINDOW *w, struct moving_element* m){
  int next_ch = mvwinch(w, m->y, m->x + 1);
  return !(next_ch == '#' || next_ch == -1);
}

void move_ghost(WINDOW *w, struct ghost* gh){
  switch (gh->position.current_direction) {
    case 1: //cima
      if (can_go_up(w, &gh->position)) {
        move_up(w, &gh->position, gh->position.representation);
      }
      else if (can_go_down(w, &gh->position)) {
        move_down(w, &gh->position, gh->position.representation);
      }
      else if (can_go_right(w, &gh->position)) {
        move_right(w, &gh->position, gh->position.representation);
      }
      else if (can_go_left(w, &gh->position)) {
        move_left(w, &gh->position, gh->position.representation);
      }
      break;
    case 2: //baixo
      if (can_go_down(w, &gh->position)) {
        move_down(w, &gh->position, gh->position.representation);
      }
      else if (can_go_up(w, &gh->position)) {
        move_up(w, &gh->position, gh->position.representation);
      }
      else if (can_go_right(w, &gh->position)) {
        move_right(w, &gh->position, gh->position.representation);
      }
      else if (can_go_left(w, &gh->position)) {
        move_left(w, &gh->position, gh->position.representation);
      }
      break;
    case 3: //direita
      if (can_go_right(w, &gh->position)) {
        move_right(w, &gh->position, gh->position.representation);
      }
      else if (can_go_down(w, &gh->position)) {
        move_down(w, &gh->position, gh->position.representation);
      }
      else if (can_go_up(w, &gh->position)) {
        move_up(w, &gh->position, gh->position.representation);
      }
      else if (can_go_left(w, &gh->position)) {
        move_left(w, &gh->position, gh->position.representation);
      }
      break;
    case 4: //esquerda
      if (can_go_left(w, &gh->position)) {
        move_left(w, &gh->position, gh->position.representation);
      }
      else if (can_go_down(w, &gh->position)) {
        move_down(w, &gh->position, gh->position.representation);
      }
      else if (can_go_right(w, &gh->position)) {
        move_right(w, &gh->position, gh->position.representation);
      }
      else if (can_go_up(w, &gh->position)) {
        move_up(w, &gh->position, gh->position.representation);
      }
      break;
  }
}

void shoot(WINDOW *w, struct shot* s){
  switch (s->position.current_direction) {
    case 1:
      if (can_go_up(w, &s->position)) {
        move_up(w, &s->position, s->position.representation);
      }
      break;
    case 2:
      if (can_go_right(w, &s->position)) {
        move_right(w, &s->position, s->position.representation);
      }
      break;
    case 3:
      if (can_go_down(w, &s->position)) {
        move_down(w, &s->position, s->position.representation);
      }
      break;
    case 4:
      if (can_go_left(w, &s->position)) {
        move_left(w, &s->position, s->position.representation);
      }
      break;
  }
  
}

#endif
