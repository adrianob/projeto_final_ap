#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <ncurses.h>
#include <unistd.h>
#define MAX_X 80 //tamanho de colunas do mapa principal
#define MAX_Y 30 //tamanho de linhas do mapa principal
#define MAX_GHOSTS 1

int ready_to_draw = 0;
void init(void);
void draw_borders(void);
void ghost_timer_handler();

struct mr_do {
	int start_x, start_y, last_x, last_y, state;
  char representation;
};

struct ghost {
  int x, y, state;
  char representation;
};

int main(int argc, const char *argv[]){
  init();
  char MAP[MAX_X][MAX_Y];
  int ch, x, y, i;
  WINDOW *game_window;
  struct mr_do md;
  struct ghost gh;
  struct itimerval ghost_timer;

  ghost_timer.it_interval.tv_sec = 0;
  ghost_timer.it_interval.tv_usec = 500000;//intervalo
  ghost_timer.it_value.tv_sec = 0;
  ghost_timer.it_value.tv_usec = 500000;//tempo ate o primeiro sinal
  setitimer(ITIMER_REAL, &ghost_timer, 0);
  signal(SIGALRM, ghost_timer_handler);

  game_window = newwin(MAX_Y, MAX_X, 1, 1);
  gh.x = gh.y = 0;
  gh.representation = 'g';
  md.representation = 64;
  md.start_x = x = md.last_x = MAX_X / 2;
  md.start_y = y = md.last_y = MAX_Y / 2;
  while((ch = getch()) != KEY_F(1)){
    switch(ch){
      case KEY_RIGHT:
        if (x < MAX_X - 1) {
          x++;
        }
        break;
      case KEY_LEFT:
        if (x > 0) {
          x--;
        }
        break;
      case KEY_UP:
        if (y > 0) {
          y--;
        }
        break;
      case KEY_DOWN:
        if (y < MAX_Y - 1) {
          y++;
        }
        break;
    }
    if (ready_to_draw) {
      for (i = 0; i < MAX_GHOSTS; i++) {
        mvwaddch(game_window, gh.y, gh.x, gh.representation);
        gh.x++;
        gh.y++;
      }
      ready_to_draw = 0;
    }
    //move mr do
    //@TODO retirar isso quando tivermos o mapa em forma de matriz
    mvwaddch(game_window, md.last_y, md.last_x, ' ');//apaga a ultima posicao
    mvwaddch(game_window, y, x, md.representation);
    wrefresh(game_window);
    md.last_x = x;
    md.last_y = y;
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

void ghost_timer_handler(){
  ready_to_draw = 1;
}
