#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <ncurses.h>
#include <menu.h>
#include <locale.h>
#include "main.h"
#include "movement.h"
#include "file_operations.h"
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

int timer_ready = 0;

int main(int argc, const char *argv[]){
  //configuracoes iniciais
  config();
  show_menu();

  return 0;
}

void play_level_one(void){
  int MAP[MAX_Y][MAX_X];
  make_map(load_level(1), MAP);
  unsigned int created_ghosts = 0;
  unsigned int ghost_timer = 0;

  //cria a janela do jogo dentro da borda
  WINDOW *border_window;
  border_window = newwin(MAX_Y + 2, MAX_X + 2, 0, 0);
  WINDOW *game_window;
  game_window = newwin(MAX_Y, MAX_X, 1, 1);
  box(border_window, 0, 0);
  draw_map(game_window, MAP);

  //configuracao do timer
  config_timer();

  struct position nest_position = find_char(game_window, MAP, '&');
  struct ghost ghosts[MAX_GHOSTS];
  create_ghosts(game_window, ghosts, nest_position);
  struct shot shot;
  shot.sprite = DEFAULT_SHOT;
  sprite nest = DEFAULT_NEST;
  nest.position = nest_position;
  //cria fantasmas
  struct mr_do md;
  md.sprite = DEFAULT_MR_DO;
  md.sprite.position = find_char(game_window, MAP, ACS_PI);
  md.sprite.representation = ACS_PI;

  int ch;
  while((ch = getch()) != KEY_F(1)){
    if(md.sprite.alive == 1){
      switch(ch){
        case KEY_RIGHT:
          move_sprite(game_window, &md.sprite, RIGHT_DIRECTION);
          break;
        case KEY_LEFT:
          move_sprite(game_window, &md.sprite, LEFT_DIRECTION);
          break;
        case KEY_UP:
          move_sprite(game_window, &md.sprite, UP_DIRECTION);
          break;
        case KEY_DOWN:
          move_sprite(game_window, &md.sprite, DOWN_DIRECTION);
          break;
        case ' ':
          if (!shot.sprite.alive){
            shot.sprite.alive = 1;
            shot.sprite.position = md.sprite.position;
            shot.sprite.direction = md.sprite.direction;
          }
          break;
      }
    }
    if (timer_ready) {
      ghost_timer++;
      move_ghosts(game_window, ghosts);

      //tempo de criar um novo fantasma
      if (ghost_timer == (GHOST_INTERVAL / INTERVAL)) {
        if (created_ghosts < MAX_GHOSTS) {
          ghosts[created_ghosts].sprite.alive = 1;
          created_ghosts++;
        }
        ghost_timer = 0;
      }

      if(shot.sprite.alive){
        shoot(game_window, &shot);
      }
      timer_ready = 0;
    }

    for (int i = 0; i < MAX_GHOSTS; i++) {
      check_collision(game_window, &ghosts[i].sprite, &md.sprite);
      check_shot_collision(game_window, &shot.sprite, &ghosts[i].sprite);
    }

    print_char(game_window, &nest);
    if (md.sprite.alive) {
      print_char(game_window, &md.sprite);
    }
    wrefresh(border_window);
    wrefresh(game_window);
  }
  endwin();
}

void show_menu(void){
  char *choices[] = {
                    "Fase 1",
                    "Continua",
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

  set_item_userptr(menu_items[0], play_level_one);//nivel 1
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
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);
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

void draw_map(WINDOW *w, int MAP[MAX_Y][MAX_X]){
  for (int i = 0; i < MAX_Y; i++) {
    for (int j = 0; j < MAX_X; j++) {
      if (MAP[i][j] == 'f') {
        wattron(w, COLOR_PAIR(4));
        mvwaddch(w, i, j, MAP[i][j]);
        wattroff(w, COLOR_PAIR(4));
      }
      else{
        mvwaddch(w, i, j, MAP[i][j]);
      }
    }
  }
}

struct position find_char(WINDOW *w, int MAP[MAX_Y][MAX_X], int ch){
  struct position position;
  for (int i = 0; i < MAX_Y; i++) {
    for (int j = 0; j < MAX_X; j++) {
      if (MAP[i][j] == ch) {
        position.x = j;
        position.y = i;
      }
    }
  }
  return position;
}

void create_ghosts(WINDOW *w, struct ghost ghosts[MAX_GHOSTS], struct position position){
  for (int i = 0; i < MAX_GHOSTS; i++) {
    ghosts[i].sprite = DEFAULT_GHOST;
    ghosts[i].sprite.representation = ACS_CKBOARD;
    ghosts[i].sprite.position = position;
  }
}

const sprite DEFAULT_GHOST = {
  .alive = 0,
  .direction = 1,
  .color = 2
};

const sprite DEFAULT_NEST = {
  .representation = '&',
  .color = 1,
  .alive = 1
};

const sprite DEFAULT_SHOT = {
  .representation = '*',
  .alive = 0,
  .color = 3
};

const sprite DEFAULT_MR_DO = {
  .alive = 1,
  .color = 3
};
