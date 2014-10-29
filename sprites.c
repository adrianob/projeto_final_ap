#include "sprites.h"

//cria as lista de sprites a partir do mapa
void make_lists(chtype (*MAP)[MAX_X], struct sprite_list *sl){
  //percorre a matriz do mapa
  for (int i = 0; i < MAX_Y; i++) {
    for (int j = 0; j < MAX_X; j++) {
      //sempre vai ter um espaco em branco abaixo de todo o mapa
      SPRITE space = DEFAULT_SPACE;
      space.position.x = j;
      space.position.y = i;
      push(&(sl->spaces), space);

      SPRITE s;
      switch (MAP[i][j]) {
        case 'p':
          s = DEFAULT_WALL;
          s.position = (struct position){.x = j, .y = i};
          push(&(sl->walls), s);
          break;
        case 't':
          s = DEFAULT_SHOT;
          s.position = (struct position){.x = j, .y = i};
          push(&(sl->shot), s);
          break;
        case 'i':
          s = DEFAULT_GHOST;
          s.position = (struct position){.x = j, .y = i};
          push(&(sl->ghosts), s);
          break;
        case 'f':
          s = DEFAULT_FRUIT;
          s.position = (struct position){.x = j, .y = i};
          push(&(sl->fruits), s);
          break;
        case 'd':
          s = DEFAULT_MR_DO;
          s.position = (struct position){.x = j, .y = i};
          push(&(sl->mr_do), s);
          break;
        case 'n':
          s = DEFAULT_NEST;
          s.position = (struct position){.x = j, .y = i};
          push(&(sl->nest), s);
          break;
      }
    }
  }
}

int collided(SPRITE *current, SPRITE *sp){
  return (current->alive && sp->alive) &&
          (current->representation != sp->representation) &&
            (((current->position.x == sp->position.x) && (current->position.y == sp->position.y)) || //mesma posicao
            ((sp->position.last_x == current->position.x) && (sp->position.last_y == current->position.y))//inverteram de posicao
          );
}

void check_sprite_collision(struct sprite_list *sl){
  SPRITE *list;
  SPRITE *current = sl->walls;
  while(current != NULL){
    //mrdo colide com parede
    if (collided(current, sl->mr_do)) {
      current->alive = 0;
    }

    current = current->next;
  }

  current = sl->fruits;
  while(current != NULL){
    //mrdo colide com fruta
    if (collided(current, sl->mr_do)) {
      current->alive = 0;
      game_state.score += 50;
    }

    //tiro colide com fruta
    if (list_size(sl->shot) > 0){
      if (collided(current, sl->shot)) {
        current->alive = 0;
        sl->shot->alive = 0;
      }
    }

    //fantasma colide com fruta
    list = sl->ghosts;
    while(list != NULL){
      if (collided(current, list)) {
        current->alive = FALSE;
      }
      list = list->next;
    }

    current = current->next;
  }

  current = sl->ghosts;
  while(current != NULL){
    //fantasma colide com mrdo
    if (collided(current, sl->mr_do)) {
      sl->mr_do->alive = 0;
    }

    //fantasma colide com tiro
    if (list_size(sl->shot) > 0){
      if (collided(current, sl->shot)) {
        current->alive = 0;
        sl->shot->alive = 0;
        game_state.score += 10;
      }
    }

    current = current->next;
  }

}

void create_shot(struct sprite_list *sl){
  if (list_size(sl->shot) < 1) {//cria o tiro
    SPRITE shot = DEFAULT_SHOT;
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
    SPRITE ghost = DEFAULT_GHOST;
    ghost.position = find_char(sl, CH_NEST);
    push(&sl->ghosts, ghost);
  }
}

void create_rocks(WINDOW *w, SPRITE *rocks){
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

const SPRITE DEFAULT_GHOST = {
  .representation = CH_GHOST,
  .alive = 1,
  .direction = LEFT_DIRECTION
};

const SPRITE DEFAULT_FRUIT = {
  .alive = 1,
  .representation = CH_FRUIT
};

const SPRITE DEFAULT_ROCK = {
  .alive = 0,
  .direction = DOWN_DIRECTION,
  .representation = CH_ROCK
};

const SPRITE DEFAULT_NEST = {
  .representation = CH_NEST,
  .alive = 1
};

const SPRITE DEFAULT_SHOT = {
  .representation = CH_SHOT,
  .direction = RIGHT_DIRECTION,
  .alive = 0
};

const SPRITE DEFAULT_MR_DO = {
  .representation = CH_MR_DO,
  .alive = 1
};

const SPRITE DEFAULT_WALL = {
  .representation = CH_WALL,
  .alive = 1
};

const SPRITE DEFAULT_SPACE = {
  .representation = CH_SPACE,
  .alive = 1
};
