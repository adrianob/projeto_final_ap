#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <ncurses.h>
#include <menu.h>
#include <locale.h>
#include "main.h"
#include "movement.h"
#include "file_operations.h"
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

int ready_to_draw = 0;

int main(int argc, const char *argv[]){
  //configuracoes iniciais
  config();
  show_menu();

  return 0;
}

void play_level_one(void){
  char MAP[MAX_Y][MAX_X];
  make_map(load_level(1), MAP);

  //cria a janela do jogo dentro da borda
  WINDOW *border_window;
  border_window = newwin(MAX_Y + 2, MAX_X + 2, 0, 0);
  WINDOW *game_window;
  game_window = newwin(MAX_Y, MAX_X, 1, 1);
  box(border_window, 0, 0);
  draw_map(game_window, MAP);

  //configuracao do timer
  config_timer();

  struct shot s;
  s.sprite.representation = '*';
  struct ghost gh;
  gh.sprite.representation = ACS_CKBOARD;
  gh.sprite.position.x = gh.sprite.position.y = 0;
  gh.sprite.direction = 3;
  struct mr_do md;
  md.sprite.representation = ACS_PI;
  md.sprite.position.x = md.sprite.position.last_x = MAX_X / 2;
  md.sprite.position.y = md.sprite.position.last_y = MAX_Y / 2;

  int ch;
  while((ch = getch()) != KEY_F(1)){
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
        s.sprite.position.x = md.sprite.position.x;
        s.sprite.position.y = md.sprite.position.y;
        s.sprite.direction = md.sprite.direction;
        break;
    }
    if (ready_to_draw) {
      for (int i = 0; i < MAX_GHOSTS; i++) {
        move_ghost(game_window, &gh);
      }
      //@TODO transformar isso numa flag que mantem o tiro andando ate acertar alguma coisa
      shoot(game_window, &s);
      ready_to_draw = 0;
    }

    wrefresh(border_window);
    wrefresh(game_window);
  }
  endwin();
}

void show_menu(void){
  char *choices[] = {
                    "Fase 1",
                    "Fase 2",
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
  initscr();			/* Start curses mode 		*/
  cbreak();				/* Line buffering disabled	*/
  setlocale(LC_ALL, "");
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
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

void timer_handler(void){
  ready_to_draw = 1;
}

void draw_map(WINDOW *w, char MAP[MAX_Y][MAX_X]){
  for (int i = 0; i < MAX_Y; i++) {
    for (int j = 0; j < MAX_X; j++) {
      mvwaddch(w, i, j, MAP[i][j]);
    }
  }
}
