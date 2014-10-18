#include "sprites.h"

//cria as lista de sprites a partir do mapa
void make_lists(chtype (*MAP)[MAX_X], struct sprite_list *sl){
  //percorre a matriz do mapa
  for (int i = 0; i < MAX_Y; i++) {
    for (int j = 0; j < MAX_X; j++) {
      //sempre vai ter um espaco em branco abaixo de todo o mapa
      sprite space = DEFAULT_SPACE;
      space.position.x = j;
      space.position.y = i;
      push(&(sl->spaces), space);

      sprite s;
      switch (MAP[i][j]) {
        case 'p':
          s = DEFAULT_WALL;
          s.position.x = j;
          s.position.y = i;
          push(&(sl->walls), s);
          break;
        case 't':
          s = DEFAULT_SHOT;
          s.position.x = j;
          s.position.y = i;
          push(&(sl->shot), s);
          break;
        case 'i':
          s = DEFAULT_GHOST;
          s.position.x = j;
          s.position.y = i;
          push(&(sl->ghosts), s);
          break;
        case 'f':
          s = DEFAULT_FRUIT;
          s.position.x = j;
          s.position.y = i;
          push(&(sl->fruits), s);
          break;
        case 'd':
          s = DEFAULT_MR_DO;
          s.position.x = j;
          s.position.y = i;
          push(&(sl->mr_do), s);
          break;
        case 'n':
          s = DEFAULT_NEST;
          s.position.x = j;
          s.position.y = i;
          push(&(sl->nest), s);
          break;
      }
    }
  }
}

void check_sprite_collision(struct sprite_list *sl, sprite *sp){
  sprite *list[] = {sl->walls, sl->fruits, sl->ghosts, sl->mr_do, sl->shot};
  for (int i = 0; i < (int)sizeof(list)/sizeof(sprite*); i++) {
    sprite *current = list[i];
    while(current != NULL){
      if ((current->alive && sp->alive) &&
          (current->representation != sp->representation) &&(
          ((current->position.x == sp->position.x) && (current->position.y == sp->position.y)) || //mesma posicao
          ((sp->position.last_x == current->position.x) && (sp->position.last_y == current->position.y))//inverteram de posicao
          )) {//colidiu @TODO passar o teste pra uma funcao separada

        //mrdo colide com parede
        if (current->representation == CH_WALL && sp->representation == CH_MR_DO) {
          current->alive = 0;
        }
        //mrdo colide com fruta
        else if (current->representation == CH_FRUIT && sp->representation == CH_MR_DO) {
          current->alive = 0;
          game_state.score += 50;
        }
        //fantasma colide com mrdo
        else if (current->representation == CH_MR_DO && sp->representation == CH_GHOST) {
          current->alive = 0;
        }
        //mrdo colide com fantasma
        else if (current->representation == CH_GHOST && sp->representation == CH_MR_DO) {
          sp->alive = 0;
        }
        //tiro colide com o fantasma
        else if (current->representation == CH_GHOST && sp->representation == CH_SHOT) {
          current->alive = 0;
          sp->alive = 0;
          game_state.score += 10;
        }
        //tiro colide com fruta
        else if (current->representation == CH_FRUIT && sp->representation == CH_SHOT) {
          current->alive = 0;
          sp->alive = 0;
        }
        //fantasma colide com fruta
        else if (current->representation == CH_FRUIT && sp->representation == CH_GHOST) {
          current->alive = 0;
        }
      }
      current = current->next;
    }
  }
}

void check_ghosts_collision(struct sprite_list *sl, sprite *sp){
  sprite *current = sp;
  while(current != NULL){
    check_sprite_collision(sl, sp);
    current = current->next;
  }
}

void create_shot(struct sprite_list *sl){
  if (list_size(sl->shot) < 1) {//cria o tiro
    sprite shot = DEFAULT_SHOT;
    push(&sl->shot, shot);
  }
  if (!sl->shot->alive) {
    sl->shot->alive = 1;
    sl->shot->position = find_char(sl, CH_MR_DO);
    sl->shot->direction = sl->mr_do->direction;
  }
}

void create_ghost(struct sprite_list *sl){
  if (list_size(sl->ghosts) < MAX_GHOSTS) {
    sprite ghost = DEFAULT_GHOST;
    ghost.position = find_char(sl, CH_NEST);
    push(&sl->ghosts, ghost);
  }
}

void create_rocks(WINDOW *w, sprite *rocks){
  for (int i = 0; i < MAX_ROCKS; i++) {

    rocks[i] = DEFAULT_ROCK;

    do{
      int x = rand() % MAX_X;
      int y = rand() % MAX_Y;

      if (mvwinch(w,y,x) == CH_WALL) {
        rocks[i].position.x = x;
        rocks[i].position.y = y;
        rocks[i].alive = 1;
      }
    }while(!rocks[i].alive);
  }
}

const sprite DEFAULT_GHOST = {
  .representation = CH_GHOST,
  .alive = 1,
  .direction = LEFT_DIRECTION
};

const sprite DEFAULT_FRUIT = {
  .alive = 1,
  .representation = CH_FRUIT
};

const sprite DEFAULT_ROCK = {
  .alive = 0,
  .direction = DOWN_DIRECTION,
  .representation = CH_ROCK
};

const sprite DEFAULT_NEST = {
  .representation = CH_NEST,
  .alive = 1
};

const sprite DEFAULT_SHOT = {
  .representation = CH_SHOT,
  .position = {0, 0, 0, 0},
  .direction = RIGHT_DIRECTION,
  .alive = 0
};

const sprite DEFAULT_MR_DO = {
  .representation = CH_MR_DO,
  .alive = 1
};

const sprite DEFAULT_WALL = {
  .representation = CH_WALL,
  .alive = 1
};

const sprite DEFAULT_SPACE = {
  .representation = CH_SPACE,
  .alive = 1
};
