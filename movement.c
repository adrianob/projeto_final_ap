#include "main.h"
#include "movement.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

void print_char(WINDOW *w, sprite* sprite){
  if (sprite->alive) {
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
  check_fruit_collision(w, sprite, direction);
  sprite->direction = direction;
  print_char(w, sprite);
}

void check_fruit_collision(WINDOW *w, sprite* sprite, int direction){
  if (mvwinch(w, sprite->position.y, sprite->position.x) ==  (213 | A_ALTCHARSET | COLOR_PAIR(4))) {
    //@TODO pegar soh o char com um bitmap
    if ((sprite->representation ) == (ACS_PI | COLOR_PAIR(2))) {
      game_state.score += 50;
    }
    if ((sprite->representation ) == (183 | A_ALTCHARSET)) {
      sprite->alive = 0;
      mvwaddch(w, sprite->position.last_y, sprite->position.last_x, ' ');
    }
  }
}

//verifica se existe uma parede no caminho ou eh fim do mapa
int can_go_to_direction(WINDOW *w, struct position* p, int direction){
  int next_ch = next_char(w, p, direction);
  return !(next_ch == (97 | A_ALTCHARSET) || next_ch == (199 | A_ALTCHARSET | COLOR_PAIR(3)) || next_ch == -1);
}

int can_fall(WINDOW *w, struct position* p, int direction){
  int next_ch = next_char(w, p, direction);
  return !(next_ch == (97 | A_ALTCHARSET) || next_ch == (ACS_PI | COLOR_PAIR(2)) || next_ch == -1);
}

int next_char(WINDOW *w, struct position* p, int direction){
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
  return next_ch;
}

void move_ghost(WINDOW *w, struct ghost* gh){
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

void move_fruit(WINDOW *w, struct fruit* fr){
  if (can_fall(w, &fr->sprite.position, fr->sprite.direction)){
    move_sprite(w, &fr->sprite, fr->sprite.direction);
  }
  else{
    print_char(w, &fr->sprite);
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
    s->alive = 0;
    print_char(w, s);
  }
}

//se colidir morre o segundo sprite
void check_collision(WINDOW *w, sprite *sp1, sprite *sp2){
  if (sp1->position.x == sp2->position.x
      && sp1->position.y == sp2->position.y
      && sp1->alive && sp2->alive){
    sp2->alive = 0;
  }
}

void check_shot_collision(WINDOW *w, sprite *sp1, sprite *sp2){
  int ghost[2] = {sp2->position.x,sp2->position.y};
  int shot[2]  = {sp1->position.x,sp1->position.y};
  int shot_last[2]  = {sp1->position.last_x,sp1->position.last_y};

  if ((sp1->alive && sp2->alive ) &&
      ((shot[0] == ghost[0] && shot[1] == ghost[1]) ||
       ((shot_last[0] == ghost[0] && shot_last[1] == ghost[1])))){
    game_state.score += 10;
    sp2->alive = 0;
    sp1->alive = 0;
    print_char(w, sp1);
  }
}

void move_ghosts(WINDOW *w, struct ghost ghosts[MAX_GHOSTS]){
  for (int i = 0; i < MAX_GHOSTS; i++) {
    if(ghosts[i].sprite.alive){
      move_ghost(w, &ghosts[i]);
    }
  }
}

void move_fruits(WINDOW *w, struct fruit fruits[MAX_FRUITS]){
  for (int i = 0; i < MAX_FRUITS; i++) {
    if(fruits[i].sprite.alive){
      move_fruit(w, &fruits[i]);
    }
  }
}
