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
void config_timer(struct itimerval* timer);//@TODO descobrir porque ocorre um warning quando botamos isso no header

int main(int argc, const char *argv[]){
  init();
  FILE *level_file;
  level_file = load_level(1);
  char MAP[MAX_Y][MAX_X];
  make_map(level_file, MAP);

  //cria a janela do jogo dentro da borda
  WINDOW *game_window;
  game_window = newwin(MAX_Y, MAX_X, 1, 1);
  //desenha o mapa
  for (int i = 0; i < MAX_Y; i++) {
    for (int j = 0; j < MAX_X; j++) {
        mvwaddch(game_window, i, j, MAP[i][j]);
    }
  }
  //configuracao do timer
  struct itimerval timer;
  config_timer(&timer);

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
        move_right(&md.position);
        break;
      case KEY_LEFT:
        move_left(&md.position);
        break;
      case KEY_UP:
        move_up(&md.position);
        break;
      case KEY_DOWN:
        move_down(&md.position);
        break;
    }
    if (ready_to_draw) {
      for (int i = 0; i < MAX_GHOSTS; i++) {
        //@TODO mover para uma funcao
        mvwaddch(game_window, gh.position.last_y, gh.position.last_x, ' ');//apaga a ultima posicao
        mvwaddch(game_window, gh.position.y, gh.position.x, gh.representation);
        gh.position.last_x = gh.position.x;
        gh.position.last_y = gh.position.y;
        move_right(&gh.position);
      }
      ready_to_draw = 0;
    }
    //move mr do
    //@TODO retirar isso quando tivermos o mapa em forma de matriz
    mvwaddch(game_window, md.position.last_y, md.position.last_x, ' ');//apaga a ultima posicao
    mvwaddch(game_window, md.position.y, md.position.x, md.representation);
    md.position.last_x = md.position.x;
    md.position.last_y = md.position.y;

    wrefresh(game_window);
  }
  endwin();
  return 0;
}

//configuracoes iniciais
void init(void){
  initscr();			/* Start curses mode 		*/
  cbreak();				/* Line buffering disabled	*/
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
  noecho();			/* Don't echo() while we do getch */
  curs_set(0);
  draw_borders();
}

void draw_borders(void){
  int i;
  //bordas horizontais
  for (i = 0; i < MAX_X + 1; i++) {
    mvaddch(0, i, '-');
    mvaddch(MAX_Y + 1, i, '-');
  }

  //bordas verticais
  for (i = 0; i < MAX_Y + 1; i++) {
    mvaddch(i, 0, '|');
    mvaddch(i, MAX_X + 1, '|');
  }
}

void timer_handler(){
  ready_to_draw = 1;
}

void config_timer(struct itimerval* timer){
  timer->it_interval.tv_sec = 0;
  timer->it_interval.tv_usec = INTERVAL;//intervalo
  timer->it_value.tv_sec = 0;
  timer->it_value.tv_usec = INTERVAL;//tempo ate o primeiro sinal
  setitimer(ITIMER_REAL, timer, 0);
  signal(SIGALRM, timer_handler);
}
