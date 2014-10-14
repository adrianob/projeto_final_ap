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

void debug_print(WINDOW *w, sprite* md, sprite* nest){
  mvwaddch(w, nest->position.y, nest->position.x, nest->representation);
  if (md->alive) {
    mvwaddch(w, md->position.y, md->position.x, md->representation);
  }
  else{
    mvwaddch(w, md->position.y, md->position.x, ' ');
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
  if (mvwinch(w, sprite->position.y, sprite->position.x) ==  CH_FRUIT) {
    //@TODO pegar soh o char com um bitmap
    if ((sprite->representation ) == CH_MR_DO) {
      game_state.score += 50;
    }
    if ((sprite->representation ) == CH_SHOT) {
      sprite->alive = 0;
      mvwaddch(w, sprite->position.last_y, sprite->position.last_x, ' ');
    }
  }
}

//verifica se existe uma parede no caminho ou eh fim do mapa
int can_go_to_direction(WINDOW *w, struct position* p, int direction){
  int next_ch = next_char(w, p, direction);
  return !(next_ch == CH_WALL || next_ch == CH_ROCK || next_ch == -1);
}

int can_fall(WINDOW *w, struct position* p, int direction){
  int next_ch = next_char(w, p, direction);
  return (next_ch == ' ');
}

int next_char(WINDOW *w, struct position *p, int direction){
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

void move_ghost(WINDOW *w, sprite *gh){
  if (!can_go_to_direction(w, &gh->position, gh->direction)){
    int d = rand() % 4 + 1;
    while(!can_go_to_direction(w, &gh->position, d)){
      d = rand() % 4 + 1;
    }

    move_sprite(w, gh, d);
  }
  else{
    move_sprite(w, gh, gh->direction);
  }
}

void move_rock(WINDOW *w, sprite *rk){
  if (can_fall(w, &rk->position, rk->direction)){
    move_sprite(w, rk, rk->direction);
  }
  else{
    rk->position.last_x = rk->position.x;
    rk->position.last_y = rk->position.y;
    print_char(w, rk);
  }
}

void shoot(WINDOW *w, sprite* s){
  move_if_possible(w, s);
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

void move_ghosts(WINDOW *w, sprite ghosts[MAX_GHOSTS]){
  for (int i = 0; i < MAX_GHOSTS; i++) {
    if(ghosts[i].alive){
      move_ghost(w, &ghosts[i]);
    }
  }
}

void move_rocks(WINDOW *w, sprite rocks[MAX_ROCKS]){
  for (int i = 0; i < MAX_ROCKS; i++) {
    if(rocks[i].alive){
      move_rock(w, &rocks[i]);
    }
  }
}
