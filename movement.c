/*controla movimentação dos sprites na tela*/
#include "main.h"
#include "movement.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

//retorna o caractere adjacente a posição dada na direção dada
int next_char(WINDOW *w, struct position p, int direction){
  int next_ch;
  switch (direction) {
    case UP_DIRECTION:
      next_ch = mvwinch(w, p.y - 1, p.x );
      break;
    case DOWN_DIRECTION:
      next_ch = mvwinch(w, p.y + 1, p.x );
      break;
    case RIGHT_DIRECTION:
      next_ch = mvwinch(w, p.y, p.x + 1);
      break;
    case LEFT_DIRECTION:
      next_ch = mvwinch(w, p.y, p.x - 1 );
      break;
  }
  return next_ch;
}

//verifica se o sprite pode se mover na direção dada
int can_go_to_direction(WINDOW *w, SPRITE sp, int direction){
  int can_go = 0;
  //verifica se eh fim do mapa
  switch (direction) {
    case UP_DIRECTION:
      if (sp.position.y > 0) {
        can_go = 1;
      }
      break;
    case RIGHT_DIRECTION:
      if (sp.position.x < MAX_X - 1) {
        can_go = 1;
      }
      break;
    case DOWN_DIRECTION:
      if (sp.position.y < MAX_Y - 1) {
        can_go = 1;
      }
      break;
    case LEFT_DIRECTION:
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
int move_sprite(WINDOW *w, SPRITE *sprite, int direction){
  sprite->position.last_x = sprite->position.x;
  sprite->position.last_y = sprite->position.y;
  int can_go = can_go_to_direction(w, *sprite, direction);
  if (can_go) {
    switch (direction) {
      case UP_DIRECTION:
        sprite->position.y--;
        break;
      case RIGHT_DIRECTION:
        sprite->position.x++;
        break;
      case DOWN_DIRECTION:
        sprite->position.y++;
        break;
      case LEFT_DIRECTION:
        sprite->position.x--;
        break;
    }
  }
  if (direction) {
    sprite->direction = direction;
  }
  return can_go;
}

int can_fall(WINDOW *w, struct position* p, int direction){
  int next_ch = next_char(w, *p, DOWN_DIRECTION);
  return (next_ch == ' ');
}

//algoritmo de movimentação dos fantasmas
void move_ghost(WINDOW *w, SPRITE *gh){
  int gh_direction = gh->direction;
  int d;
  int can_go = 0;
  if (!can_go_to_direction(w, *gh, gh->direction)){//nao pode continuar na mesma direcao
    d = rand() % 2;//0 ou 1
    switch (gh->direction) {//tenta ir numa nova direcao
      case RIGHT_DIRECTION:
      case LEFT_DIRECTION:
        can_go = move_sprite(w, gh, d ? UP_DIRECTION : DOWN_DIRECTION) || move_sprite(w, gh, d ? DOWN_DIRECTION : UP_DIRECTION);
        break;
      case UP_DIRECTION:
      case DOWN_DIRECTION:
        can_go = move_sprite(w, gh, d ? RIGHT_DIRECTION : LEFT_DIRECTION) || move_sprite(w, gh, d ? LEFT_DIRECTION : RIGHT_DIRECTION);
        break;
    }
    if(!can_go){//nao conseguiu ir para uma nova direcao, volta pela direcao que venho
      switch (gh_direction) {
        case UP_DIRECTION:
          move_sprite(w, gh, DOWN_DIRECTION);
          break;
        case DOWN_DIRECTION:
          move_sprite(w, gh, UP_DIRECTION);
          break;
        case RIGHT_DIRECTION:
          move_sprite(w, gh, LEFT_DIRECTION);
          break;
        case LEFT_DIRECTION:
          move_sprite(w, gh, RIGHT_DIRECTION);
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

int get_keyboard_direction(chtype ch){
  switch(ch){
    case KEY_RIGHT:
      return RIGHT_DIRECTION;
    case KEY_LEFT:
      return LEFT_DIRECTION;
    case KEY_UP:
      return UP_DIRECTION;
    case KEY_DOWN:
      return DOWN_DIRECTION;
  }
  return 0;
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
