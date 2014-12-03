/*controla movimentação dos sprites na tela*/
#include "main.h"
#include "path.h"
#include "movement.h"

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
      if (sp.position.y > 0)
        can_go = 1;
      break;
    case right:
      if (sp.position.x < MAX_X - 1)
        can_go = 1;
      break;
    case down:
      if (sp.position.y < MAX_Y - 1)
        can_go = 1;
      break;
    case left:
      if (sp.position.x > 0)
        can_go = 1;
  }
  //verificacoes especificas pra cada tipo de sprite
  int next_ch = next_char(w, sp.position, direction);
  if (sp.representation == CH_MR_DO){
    return (next_ch != -1 && can_go);
  }
  else{
    return (next_ch != CH_WALL && next_ch != -1 && can_go);
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

//algoritmo de movimentação dos fantasmas
void move_ghost(WINDOW *w, SPRITE *gh, NODE map_node[MAX_Y][MAX_X], struct position destiny){
    //Nó inicial
    NODE *start;
    start = &map_node[gh->position.y][gh->position.x];
    start->g = 0;
    start->f = start->h;
    start->parent = NULL;
    start->status = 1;

    //INICIALIZAÇÃO//

    int res = findPath(map_node, destiny);      //Função que encontra o caminho.

    struct position next;                       //struct position contendo o próximo passo
    next = (nextStep(map_node, destiny))->pos;
    if(res){
      if (next.x < gh->position.x) {
        move_sprite(w, gh, left);
      }
      if (next.y > gh->position.y) {
        move_sprite(w, gh, down);
      }
      if (next.y < gh->position.y) {
        move_sprite(w, gh, up);
      }
      if (next.x > gh->position.x) {
        move_sprite(w, gh, right);
      }
    }
    else{
      move_sprite(w, gh, right);
    }
}

void move_ghosts(WINDOW *w, SPRITE *ghosts){
  SPRITE *current = ghosts;
  char map_char[MAX_Y][MAX_X];
  chtype ch;
  struct position destiny;
  int mr_do_alive = 0;

  for (int i = 0; i < MAX_Y; i++) {
    for (int j = 0; j < MAX_X; j++) {
      ch = mvwinch(w, i, j);
      if (ch == CH_WALL) {
        map_char[i][j] = '1';
      }
      else if(ch == CH_MR_DO){
        mr_do_alive = 1;
        destiny.y = i;
        destiny.x = j;
        map_char[i][j] = '2';
      }
      else{
        map_char[i][j] = '2';
      }
    }
  }
  if (!mr_do_alive) {
    return;
  }

  NODE map_node[MAX_Y][MAX_X];
  createMap(map_node, map_char, destiny);

  while(current != NULL){
    if (current->alive) {
      move_ghost(w, current, map_node, destiny);
    }
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
