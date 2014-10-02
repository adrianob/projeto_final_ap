#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <ncurses.h>
#include <unistd.h>
#include "main.h"
#include "movement.h"
#include "file_operations.h"

int ready_to_draw = 0;

int main(int argc, const char *argv[]){
  //configuracoes iniciais
  config();

  FILE *level_file;
  level_file = load_level(1);
  char MAP[MAX_Y][MAX_X];
  make_map(level_file, MAP);

  //cria a janela do jogo dentro da borda
  WINDOW *game_window;
  game_window = newwin(MAX_Y, MAX_X, 1, 1);
  //desenha as bordas
  draw_borders();
  //desenha o mapa
  draw_map(game_window, MAP);
  //configuracao do timer
  config_timer();

  struct mr_do md;
  struct ghost gh;
  gh.position.x = gh.position.y = 0;
  gh.representation = 'g';
  md.representation = 64;
  md.position.x = md.position.last_x = MAX_X / 2;
  md.position.y = md.position.last_y = MAX_Y / 2;

  int ch;
  while((ch = getch()) != KEY_F(1)){
    switch(ch){
      case KEY_RIGHT:
        move_right(game_window, &md.position, md.representation);
        break;
      case KEY_LEFT:
        move_left(game_window, &md.position, md.representation);
        break;
      case KEY_UP:
        move_up(game_window, &md.position, md.representation);
        break;
      case KEY_DOWN:
        move_down(game_window, &md.position, md.representation);
        break;
    }
    if (ready_to_draw) {
      for (int i = 0; i < MAX_GHOSTS; i++) {
        move_down(game_window, &gh.position, gh.representation);
      }
      ready_to_draw = 0;
    }

    wrefresh(game_window);
  }
  endwin();
  return 0;
}

void config(void){
  initscr();			/* Start curses mode 		*/
  cbreak();				/* Line buffering disabled	*/
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
  noecho();			/* Don't echo() while we do getch */
  curs_set(0);
}

void draw_borders(void){
  mvaddch(0, 0, '+');
  mvaddch(0, MAX_X + 1, '+');
  mvaddch(MAX_Y + 1, 0, '+');
  mvaddch(MAX_Y + 1, MAX_X + 1, '+');
  mvhline(0, 1, '-', MAX_X);
  mvhline(MAX_Y + 1, 1, '-', MAX_X);
  mvvline(1, 0, '|', MAX_Y);
  mvvline(1, MAX_X + 1, '|', MAX_Y);
}

void timer_handler(){
  ready_to_draw = 1;
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

void draw_map(WINDOW *w, char MAP[MAX_Y][MAX_X]){
  for (int i = 0; i < MAX_Y; i++) {
    for (int j = 0; j < MAX_X; j++) {
      mvwaddch(w, i, j, MAP[i][j]);
    }
  }
}
