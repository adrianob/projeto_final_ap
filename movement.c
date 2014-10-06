#include "main.h"
#include "movement.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

void print_char(WINDOW *w, sprite* sprite){
  if (sprite->state) {
    mvwaddch(w, sprite->position.last_y, sprite->position.last_x, ' ');
    mvwaddch(w, sprite->position.y, sprite->position.x, sprite->representation);
  }
  else{
    mvwaddch(w, sprite->position.y, sprite->position.x, ' ');
  }
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
  if (!can_go_to_direction(w, &gh->sprite.position, gh->sprite.direction)){
    int d = rand() % 4 + 1;
    while(!can_go_to_direction(w, &gh->sprite.position, d))
      d = rand() % 4 + 1;

    move_sprite(w, &gh->sprite, d);
    }
    else{
      move_sprite(w, &gh->sprite, gh->sprite.direction);
    }
}

void shoot(WINDOW *w, struct shot* s){
  move_if_possible(w, &s->sprite);
}

void move_if_possible(WINDOW *w, sprite* s){
  if (can_go_to_direction(w, &s->position, s->direction)) {
    move_sprite(w, s, s->direction);
  }
  else{
    s->state = 0;
    print_char(w, s);
  }
}

//se colidir morre o segundo sprite
void check_collision(WINDOW *w, sprite *sp1, sprite *sp2){
  if (sp1->position.x == sp2->position.x 
      && sp1->position.y == sp2->position.y
      && sp1->state && sp2->state){
    sp2->state = 0;
  }
}

void check_shot_collision(WINDOW *w, sprite *sp1, sprite *sp2){
  int ghost[2] = {sp2->position.x,sp2->position.y};
  int ghost_last[2] = {sp2->position.last_x,sp2->position.last_y};
  int shot[2]  = {sp1->position.x,sp1->position.y};
  int shot_last[2]  = {sp1->position.last_x,sp1->position.last_y};

  if ((sp1->state && sp2->state )&& ((shot[0] == ghost[0] && shot[1] == ghost[1]) || ((shot_last[0] == ghost[0] && shot_last[1] == ghost[1]) && (shot[0] == ghost_last[0] && shot[1] == ghost_last[1])))){
    sp2->state = 0;
    sp1->state = 0;
    print_char(w, sp1);
  }
}
