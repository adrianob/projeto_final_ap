/*funções específicas para trabalhar com tipo SPRITE
 * */
#include "sprites.h"

//cria sprites obrigatórios caso não estejam no arquivo da fase
void create_default_sprites(struct sprite_list *sprite_list){
  //se o mrdo nao esta no arquivo da fase
  if(list_size(sprite_list->mr_do) == 0 ){
    SPRITE mr_do = DEFAULT_MR_DO;
    //bota mrdo na base do mapa
    mr_do.position = (struct position) {.x = MAX_X / 2, .y = MAX_Y - 1 };
    push(&(sprite_list->mr_do), mr_do);
  }

  //se o ninho nao tiver no arquivo de fase
  if(list_size(sprite_list->nest) == 0 ){
    SPRITE nest = DEFAULT_NEST;
    //bota ninho no centro do mapa
    nest.position = (struct position) {.x = MAX_X / 2, .y = MAX_Y / 2 };
    push(&(sprite_list->nest), nest);
  }

  if(list_size(sprite_list->shot) == 0 ){
    SPRITE shot = DEFAULT_SHOT;
    push(&(sprite_list->shot), shot);
  }
}

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
  create_default_sprites(sl);

}

//retorna se dois sprites colidiram ou não
int collided(SPRITE *current, SPRITE *sp){
  return (current->alive && sp->alive) &&
          (current->representation != sp->representation) &&
            (((current->position.x == sp->position.x) && (current->position.y == sp->position.y)) || //mesma posicao
            ((sp->position.last_x == current->position.x) && (sp->position.last_y == current->position.y))//inverteram de posicao
          );
}

//verifica a colisão de toda a lista de sprites
void check_sprite_collision(struct sprite_list *sl){
  SPRITE *list;
  SPRITE *current = sl->walls;
  while(current != NULL){
    //mrdo colide com parede
    if (collided(current, sl->mr_do)) {
      current->alive = FALSE;
    }

    current = current->next;
  }

  current = sl->fruits;
  while(current != NULL){
    //mrdo colide com fruta
    if (collided(current, sl->mr_do)) {
      current->alive = FALSE;
      game_state.score += 50;
    }

    //tiro colide com fruta
    if (collided(current, sl->shot)) {
      current->alive = FALSE;
      sl->shot->alive = FALSE;
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
      sl->mr_do->alive = FALSE;
    }

    //fantasma colide com tiro
    if (collided(current, sl->shot)) {
      current->alive = FALSE;
      sl->shot->alive = FALSE;
      game_state.score += 10;
    }

    current = current->next;
  }

}

void create_shot(struct sprite_list *sl){
  if (!sl->shot->alive) {
    sl->shot->alive = TRUE;
    sl->shot->position = find_char(sl, CH_MR_DO);
    sl->shot->direction = sl->mr_do->direction;
  }
}

void create_ghost(struct sprite_list *sl){
  if (list_size(sl->ghosts) < MAX_GHOSTS) {
    SPRITE ghost = DEFAULT_GHOST;
    int random_number = rand() % 4;
    ghost.direction = random_number;
    ghost.position = find_char(sl, CH_NEST);
    push(&sl->ghosts, ghost);
  }
}

//tipos padrões de sprite para simplificar o código
const SPRITE DEFAULT_GHOST = {
  .representation = CH_GHOST,
  .alive = TRUE,
  .direction = left
};

const SPRITE DEFAULT_FRUIT = {
  .alive = TRUE,
  .representation = CH_FRUIT
};

const SPRITE DEFAULT_NEST = {
  .representation = CH_NEST,
  .alive = TRUE
};

const SPRITE DEFAULT_SHOT = {
  .representation = CH_SHOT,
  .direction = right,
  .alive = FALSE
};

const SPRITE DEFAULT_MR_DO = {
  .representation = CH_MR_DO,
  .alive = TRUE
};

const SPRITE DEFAULT_WALL = {
  .representation = CH_WALL,
  .alive = TRUE
};

const SPRITE DEFAULT_SPACE = {
  .representation = CH_SPACE,
  .alive = TRUE
};
