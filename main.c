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

int timer_ready = 0;
struct game_state game_state = {.score = 0};

int main(int argc, const char *argv[]){
  config();
  show_menu();

  return 0;
}

void play_level_one(void){
  chtype MAP[MAX_Y][MAX_X];
  make_map(load_level(1), MAP);
  unsigned int created_ghosts = 0;
  unsigned int ghost_timer = 0;

  //cria a janela do jogo dentro da borda
  WINDOW *border_window = newwin(MAX_Y + 2, MAX_X + 2, 0, 0);
  WINDOW *game_window = newwin(MAX_Y, MAX_X, 1, 1);
  WINDOW *info_window = newwin(5, 11, 0, MAX_X + 5);
  box(border_window, 0, 0);

  draw_map(game_window, MAP);

  //configuracao do timer
  config_timer();

  struct position nest_position = find_char(MAP, '&');
  struct ghost ghosts[MAX_GHOSTS];
  create_ghosts(game_window, ghosts, nest_position);
  struct shot shot = {.sprite = DEFAULT_SHOT};
  sprite nest = DEFAULT_NEST;
  nest.position = nest_position;
  struct mr_do md = {.sprite = DEFAULT_MR_DO};
  md.sprite.position = find_char(MAP, ACS_PI);
  md.sprite.representation = ACS_PI | COLOR_PAIR(2);

  int ch;
  while((ch = getch()) != KEY_F(1)){
    if(md.sprite.alive){
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
    mvwprintw(info_window, 1, 0, "SCORE: %d", game_state.score);
    if (!md.sprite.alive) {
      mvwprintw(info_window, 2, 0, "GAME OVER");
    }
    print_char(game_window, &nest);
    print_char(game_window, &md.sprite);
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

void draw_map(WINDOW *w, chtype MAP[MAX_Y][MAX_X]){
  for (int i = 0; i < MAX_Y; i++) {
    for (int j = 0; j < MAX_X; j++) {
      if (MAP[i][j] == 'f') {
        mvwaddch(w, i, j, 'f' | COLOR_PAIR(4));
      }
      else{
        mvwaddch(w, i, j, MAP[i][j]);
      }
    }
  }
}

struct position find_char(chtype MAP[MAX_Y][MAX_X], int ch){
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
    ghosts[i].sprite.representation = ACS_CKBOARD | COLOR_PAIR(3);
    ghosts[i].sprite.position = position;
  }
}

const sprite DEFAULT_GHOST = {
  .alive = 0,
  .direction = UP_DIRECTION
};

const sprite DEFAULT_NEST = {
  .representation = '&' | COLOR_PAIR(1),
  .alive = 1
};

const sprite DEFAULT_SHOT = {
  .representation = '*',
  .alive = 0
};

const sprite DEFAULT_MR_DO = {
  .alive = 1
};
