#include "sprites.h"

void create_ghosts(WINDOW *w, sprite ghosts[MAX_GHOSTS], struct position position){
  for (int i = 0; i < MAX_GHOSTS; i++) {
    ghosts[i] = DEFAULT_GHOST;
    ghosts[i].representation = CH_GHOST;
    ghosts[i].position = position;
  }
}

void create_fruits(WINDOW *w, sprite *fruits){
  for (int i = 0; i < MAX_FRUITS; i++) {
    fruits[i] = DEFAULT_FRUIT;
  }
}

void create_rocks(WINDOW *w, sprite rocks[MAX_ROCKS]){
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

void find_fruits(chtype MAP[MAX_Y][MAX_X], sprite *fruits){
  int f = 0;
  for (int i = 0; i < MAX_Y; i++) {
    for (int j = 0; j < MAX_X; j++) {
      if (MAP[i][j] == CH_FRUIT) {
        fruits[f].position.y = i;
        fruits[f].position.x = j;
        f++;
      }
    }
  }
}

void print_fruits(WINDOW *w, sprite *fruits){
  for(int i = 0; i < MAX_FRUITS; i++){
    mvwaddch(w, fruits[i].position.y, fruits[i].position.x, fruits[i].representation);
  }
}

const sprite DEFAULT_GHOST = {
  .alive = 0,
  .direction = UP_DIRECTION
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
  .alive = 0
};

const sprite DEFAULT_MR_DO = {
  .alive = 1
};
