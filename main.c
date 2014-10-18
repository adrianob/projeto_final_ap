#include "main.h"
#include "movement.h"
#include "sprites.h"
#include "file_operations.h"

int timer_ready = 0;
struct game_state game_state = {.score = 0, .level = 1};

//insere um novo sprite no inicio da lista encadeada
void push(sprite **head_ref, sprite s) {
  sprite *node = (sprite *) malloc(sizeof(struct sprite));
  *node = s;
  node->next = *head_ref;
  *head_ref = node;
}

//imprime uma lista de sprites na janela
void print_list(WINDOW *w, sprite *head){
  sprite *current = head;
  while(current != NULL){
    if (current->alive) {
      mvwaddch(w, current->position.y, current->position.x, current->representation);
    }
    current = current->next;
  }
}

//retorna a posicao do caractere no mapa
struct position find_char(struct sprite_list *sl, chtype ch){
  struct position position;
  sprite *list[] = {sl->ghosts, sl->mr_do, sl->nest};
  for (int i = 0; i < 3; i++) {
    sprite *current = list[i];
    while(current != NULL){
      if (current->representation == ch) {
        position = current->position;
      }
      current = current->next;
    }
  }
  return position;
}

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

void kill_sprite(WINDOW *w, sprite *sp, struct position position, chtype rep){
  sprite *current = sp;
  while(current != NULL){
    if (current->position.x == position.x && current->position.y == position.y && current->representation == rep) {
      current->alive = 0;
    }
    current = current->next;
  }
}

void check_sprite_collision(struct sprite_list *sl, sprite *sp){
  sprite *list[] = {sl->walls, sl->fruits, sl->ghosts, sl->mr_do, sl->shot};
  for (int i = 0; i < 5; i++) {
    sprite *current = list[i];
    while(current != NULL){
      if ((current->alive && sp->alive) &&
          (current->representation != sp->representation) &&(
          ((current->position.x == sp->position.x) && (current->position.y == sp->position.y))//mesma posicao
          ||
          ((sp->position.last_x == current->position.x) && (sp->position.last_y == current->position.y))//inverteram de posicao
          )) {//colidiu @TODO passar o teste pra uma funcao separada

        //mrdo colide com parede
        if (current->representation == CH_WALL) {
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
        else if (current->representation == CH_GHOST && sp->representation == CH_SHOT) {
          current->alive = 0;
          sp->alive = 0;
          game_state.score += 10;
        }
      }
      current = current->next;
    }
  }

}

int list_size(sprite *sp){
  int count = 0;
  sprite *current = sp;
  while(current != NULL){
    count++;
    current = current->next;
  }
  return count;
}

void check_ghosts_collision(struct sprite_list *sl, sprite *sp){
  sprite *current = sp;
  while(current != NULL){
    check_sprite_collision(sl, sp);
    current = current->next;
  }
}

int main(int argc, const char *argv[]){
  config();
  show_menu();

  return 0;
}

void play(void){
  unsigned int ghost_timer = 0;
  //cria a janela do jogo dentro da borda
  WINDOW *border_window = newwin(MAX_Y + 2, MAX_X + 2, 0, 0);
  WINDOW *game_window = newwin(MAX_Y, MAX_X, 1, 1);
  WINDOW *info_window = newwin(10, 13, 0, MAX_X + 5);
  box(border_window, 0, 0);

  config_timer();

  chtype MAP[MAX_Y][MAX_X];
  make_map(load_level(game_state.level), MAP);
  struct sprite_list sprite_list= {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
  make_lists(MAP, &sprite_list);
  int ch, mrdo_direction;
  while((ch = getch()) != KEY_F(1)){
    if(sprite_list.mr_do->alive){
      switch(ch){
        case KEY_RIGHT:
          mrdo_direction = RIGHT_DIRECTION;
          break;
        case KEY_LEFT:
          mrdo_direction = LEFT_DIRECTION;
          break;
        case KEY_UP:
          mrdo_direction = UP_DIRECTION;
          break;
        case KEY_DOWN:
          mrdo_direction = DOWN_DIRECTION;
          break;
        case ' ':
          //@TODO colocar numa funcao separada
          if (list_size(sprite_list.shot) < 1) {//cria o tiro
            sprite shot = DEFAULT_SHOT;
            push(&sprite_list.shot, shot);
          }
          if (!sprite_list.shot->alive) {
            sprite_list.shot->alive = 1;
            sprite_list.shot->position = find_char(&sprite_list, CH_MR_DO);
            sprite_list.shot->direction = sprite_list.mr_do->direction;
          }
          break;
      }
    }

    if (timer_ready) {
      ghost_timer++;

      if (ghost_timer == (GHOST_INTERVAL / INTERVAL)) {//hora de criar novos fantasmas
        if (list_size(sprite_list.ghosts) < MAX_GHOSTS) {
          sprite ghost = DEFAULT_GHOST;
          ghost.position = find_char(&sprite_list, CH_NEST);
          push(&sprite_list.ghosts, ghost);
        }
        ghost_timer = 0;
      }

      move_ghosts(game_window, sprite_list.ghosts);
      move_sprite(game_window, sprite_list.mr_do, mrdo_direction);
      //checar colisoes so depois de mover todos sprites
      if(list_size(sprite_list.shot) > 0 && sprite_list.shot->alive){
        move_shot(game_window, sprite_list.shot);
        check_sprite_collision(&sprite_list, sprite_list.shot);
      }
      check_sprite_collision(&sprite_list, sprite_list.mr_do);
      check_ghosts_collision(&sprite_list, sprite_list.ghosts);
      mrdo_direction = 0;
      timer_ready = 0;
    }

    //cuidado com a ordem! imprime por cima se tiver dois sprites no mesmo lugar
    print_list(game_window, sprite_list.spaces);
    print_list(game_window, sprite_list.walls);
    print_list(game_window, sprite_list.fruits);
    print_list(game_window, sprite_list.ghosts);
    print_list(game_window, sprite_list.shot);
    print_list(game_window, sprite_list.nest);
    print_list(game_window, sprite_list.mr_do);
    check_state(info_window, sprite_list);

    refresh_windows(info_window, game_window, border_window);
  }

  endwin();
}

void refresh_windows(WINDOW *info_window, WINDOW *game_window, WINDOW *border_window){
  wrefresh(info_window);
  wrefresh(border_window);
  wrefresh(game_window);
}

void show_menu(void){
  char *choices[] = {
                    "Novo Jogo",
                    "Continuar",
                    "High Scores",
                    "Sair"
  };

  ITEM **menu_items;
  MENU *game_menu;
  int n_choices = ARRAY_SIZE(choices);
  menu_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));

  for(int i = 0; i < n_choices; ++i){
    menu_items[i] = new_item(choices[i], "");
  }

  set_item_userptr(menu_items[0], play);//nivel 1
  set_item_userptr(menu_items[3], exit);//sair

  game_menu = new_menu((ITEM **)menu_items);
  post_menu(game_menu);
  refresh();

  int c;
  while((c = getch()) != KEY_F(1)){
    switch(c){
      case KEY_DOWN:
        menu_driver(game_menu, REQ_DOWN_ITEM);
        break;
      case KEY_UP:
        menu_driver(game_menu, REQ_UP_ITEM);
        break;
      case 10: //enter
        {
        ITEM *cur;
				void (*p)(char *);

        cur = current_item(game_menu);
        p = item_userptr(cur);
        p((char *)item_name(cur));
        pos_menu_cursor(game_menu);
        }
        break;
    }
  }
}

void config(void){
  srand(time(NULL));
  initscr();			/* Start curses mode 		*/
  cbreak();				/* Line buffering disabled	*/
  setlocale(LC_ALL, "");
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_GREEN, COLOR_BLACK);
  noecho();			/* Don't echo() while we do getch */
  curs_set(0);
}

void config_timer(void){
  struct itimerval timer;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = INTERVAL;//intervalo
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = INTERVAL;//tempo ate o primeiro sinal
  setitimer(ITIMER_REAL, &timer, 0);
  signal(SIGALRM, timer_handler);
}

void timer_handler(int i){
  timer_ready = 1;
}

void draw_map(WINDOW *w, chtype (*MAP)[MAX_X]){
  for (int i = 0; i < MAX_Y; i++) {
    for (int j = 0; j < MAX_X; j++) {
      if(MAP[i][j] == CH_WALL || MAP[i][j] == ' '){
        mvwaddch(w, i, j, MAP[i][j]);
      }
    }
  }
}

int count_alive(sprite *sp){
  int count = 0;
  sprite *current = sp;
  while(current != NULL){
    if (current->alive) {
      count++;
    }
    current = current->next;
  }
  return count;
}

void check_state(WINDOW *w, struct sprite_list sl){

  int alive_ghosts = count_alive(sl.ghosts);
  int alive_fruits = count_alive(sl.fruits);
  int created_ghosts = list_size(sl.ghosts);

  if ((alive_ghosts == 0 && created_ghosts == MAX_GHOSTS) || (!alive_fruits)) {
    mvwprintw(w, 2, 0, "YOU WIN! ");
    game_state.level = 2;
    play();
  }
  else if(!sl.mr_do->alive) {
    mvwprintw(w, 2, 0, "GAME OVER!");
  }else{
    mvwprintw(w, 1, 0, "SCORE: %d", game_state.score);
    mvwprintw(w, 2, 0, "Fruits: %d ", alive_fruits);
    mvwprintw(w, 5, 0, "--GHOSTS--");
    mvwprintw(w, 6, 0, "Remaining  %d ", MAX_GHOSTS - created_ghosts);
    mvwprintw(w, 7, 0, "Alive      %d ", alive_ghosts);
    mvwprintw(w, 8, 0, "Killed     %d ", (created_ghosts - alive_ghosts));
  }
}
