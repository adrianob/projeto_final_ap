/*controla movimentação dos sprites na tela*/
#include "main.h"
#include "movement.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

//retorna o caractere adjacente a posição dada na direção dada
int next_char(WINDOW *w, struct position p, enum direction direction){
  int next_ch;
  switch (direction) {
    case up:
      next_ch = mvwinch(w, p.y - 1, p.x );
      break;
    case down:
      next_ch = mvwinch(w, p.y + 1, p.x );
      break;
    case right:
      next_ch = mvwinch(w, p.y, p.x + 1);
      break;
    case left:
      next_ch = mvwinch(w, p.y, p.x - 1 );
      break;
  }
  return next_ch;
}

//verifica se o sprite pode se mover na direção dada
int can_go_to_direction(WINDOW *w, SPRITE sp, enum direction direction){
  int can_go = 0;
  //verifica se eh fim do mapa
  switch (direction) {
    case up:
      if (sp.position.y > 0) {
        can_go = 1;
      }
      break;
    case right:
      if (sp.position.x < MAX_X - 1) {
        can_go = 1;
      }
      break;
    case down:
      if (sp.position.y < MAX_Y - 1) {
        can_go = 1;
      }
      break;
    case left:
      if (sp.position.x > 0) {
        can_go = 1;
      }
  }
  //verificacoes especificas pra cada tipo de sprite
  int next_ch = next_char(w, sp.position, direction);
  if (sp.representation == CH_MR_DO){
    return (next_ch != CH_ROCK && next_ch != -1 && can_go);
  }
  else{
    return (next_ch != CH_WALL && next_ch != CH_ROCK && next_ch != -1 && can_go);
  }
}

//tenta mover o sprite e retorna um booleano indicando se foi possivel se mover
int move_sprite(WINDOW *w, SPRITE *sprite, enum direction direction){
  sprite->position.last_x = sprite->position.x;
  sprite->position.last_y = sprite->position.y;
  int can_go = can_go_to_direction(w, *sprite, direction);
  if (can_go) {
    switch (direction) {
      case up:
        sprite->position.y--;
        break;
      case right:
        sprite->position.x++;
        break;
      case down:
        sprite->position.y++;
        break;
      case left:
        sprite->position.x--;
        break;
    }
  }
  sprite->direction = direction;
  return can_go;
}

int can_fall(WINDOW *w, struct position* p, enum direction direction){
  int next_ch = next_char(w, *p, down);
  return (next_ch == ' ');
}

//algoritmo de movimentação dos fantasmas
void move_ghost(WINDOW *w, SPRITE *gh){
  enum direction gh_direction = gh->direction;
  int d;
  int can_go = 0;
  if (!can_go_to_direction(w, *gh, gh->direction)){//nao pode continuar na mesma direcao
    d = rand() % 2;//0 ou 1
    switch (gh_direction) {//tenta ir numa nova direcao
      case right:
      case left:
        can_go = move_sprite(w, gh, d ? up : down) || move_sprite(w, gh, d ? down : up);
        break;
      case up:
      case down:
        can_go = move_sprite(w, gh, d ? right : left) || move_sprite(w, gh, d ? left : right);
        break;
    }
    if(!can_go){//nao conseguiu ir para uma nova direcao, volta pela direcao que venho
      switch (gh_direction) {
        case up:
          move_sprite(w, gh, down);
          break;
        case down:
          move_sprite(w, gh, up);
          break;
        case right:
          move_sprite(w, gh, left);
          break;
        case left:
          move_sprite(w, gh, right);
          break;
      }
    }
  }
  else{//continua na direcao que esta
    move_sprite(w, gh, gh->direction);
  }
}

void move_ghosts(WINDOW *w, SPRITE *ghosts){
  SPRITE *current = ghosts;
  while(current != NULL){
    move_ghost(w, current);
    current = current->next;
  }
}

//tiro morre quando nao pode mais se mover
void move_shot(WINDOW *w, SPRITE *s){
  if (!move_sprite(w, s, s->direction)) {
    s->alive = 0;
  }
}

enum direction get_keyboard_direction(chtype ch){
  switch(ch){
    case KEY_RIGHT:
      return right;
    case KEY_LEFT:
      return left;
    case KEY_UP:
      return up;
    case KEY_DOWN:
      return down;
  }
  return right;
}

int valid_key(chtype ch){
  int valid = 0;
  switch(ch){
    case KEY_RIGHT:
    case KEY_LEFT:
    case KEY_UP:
    case KEY_DOWN:
      valid = 1;
  }
  return valid;
}
/*
void move_rocks(WINDOW *w, sprite *rocks){
  for (int i = 0; i < MAX_ROCKS; i++) {
    if(rocks[i].alive){
      move_rock(w, &rocks[i]);
    }
  }
}
void move_rock(WINDOW *w, sprite *rk){
  if (can_fall(w, &rk->position, rk->direction)){
    if(rk->falling == 10){
      move_sprite(w, rk, rk->direction);
    }
    else{
      rk->falling++;
    }
  }
  else{
    if(rk->falling == 10){
      rk->alive = 0;
      print_char(w, rk);
    }
    else{
      print_char(w, rk);
    }
  }
}
*/
