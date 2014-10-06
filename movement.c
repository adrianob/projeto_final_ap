#include "main.h"
#include "movement.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

void print_char(WINDOW *w, sprite* sprite){
  mvwaddch(w, sprite->position.last_y, sprite->position.last_x, ' ');
  mvwaddch(w, sprite->position.y, sprite->position.x, sprite->representation);
}

void move_sprite(WINDOW *w, sprite* sprite, int direction){
  sprite->position.last_x = sprite->position.x;
  sprite->position.last_y = sprite->position.y;
  switch (direction) {
    case UP_DIRECTION:
      if (sprite->position.y > 0) {
        sprite->position.y--;
      }
      break;
    case RIGHT_DIRECTION:
      if (sprite->position.x < MAX_X - 1) {
        sprite->position.x++;
      }
      break;
    case DOWN_DIRECTION:
      if (sprite->position.y < MAX_Y - 1) {
        sprite->position.y++;
      }
      break;
    case LEFT_DIRECTION:
      if (sprite->position.x > 0) {
        sprite->position.x--;
      }
  }

  sprite->direction = direction;
  print_char(w, sprite);
}

//verifica se existe uma parede no caminho ou eh fim do mapa
int can_go_to_direction(WINDOW *w, struct position* p, int direction){
  int next_ch;
  switch (direction) {
    case UP_DIRECTION:
      next_ch = mvwinch(w, p->y - 1, p->x );
      break;
    case DOWN_DIRECTION:
      next_ch = mvwinch(w, p->y + 1, p->x );
      break;
    case RIGHT_DIRECTION:
      next_ch = mvwinch(w, p->y, p->x + 1);
      break;
    case LEFT_DIRECTION:
      next_ch = mvwinch(w, p->y, p->x - 1 );
      break;
  }
  return !(next_ch == '#' || next_ch == -1);
}

void move_ghost(WINDOW *w, struct ghost* gh){
  if (gh->sprite.state) {
    if (!can_go_to_direction(w, &gh->sprite.position, gh->sprite.direction)){
      int d = rand() % 4 + 1;
      while(!can_go_to_direction(w, &gh->sprite.position, d)){
        d = rand() % 4 + 1;
      }
      move_sprite(w, &gh->sprite, d);
      }
    else{
        move_sprite(w, &gh->sprite, gh->sprite.direction);
    }
  }
}

void shoot(WINDOW *w, struct shot* s){
  move_if_possible(w, &s->sprite);
}

void move_if_possible(WINDOW *w, sprite* s){
  if (can_go_to_direction(w, &s->position, s->direction)) {
    move_sprite(w, s, s->direction);
  }
}
