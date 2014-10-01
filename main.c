#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <ncurses.h>
#include <unistd.h>
#define MAX_X 80 //tamanho de colunas do mapa principal
#define MAX_Y 30 //tamanho de linhas do mapa principal
#define MAX_GHOSTS 1
#define INTERVAL 500000

int ready_to_draw = 0;
void init(void);
void draw_borders(void);
void ghost_timer_handler();

struct moving_element {
  int x, y;
};

struct mr_do {
  struct moving_element position;
	int last_x, last_y, state;
  char representation;
};

struct ghost {
  struct moving_element position;
  int state;
  char representation;
};

struct moving_element move_right(struct moving_element position);
struct moving_element move_left(struct moving_element position);
struct moving_element move_up(struct moving_element position);
struct moving_element move_down(struct moving_element position);

int main(int argc, const char *argv[]){
  init();
  char MAP[MAX_X][MAX_Y];
  int ch, i;
  WINDOW *game_window;
  struct mr_do md;
  struct ghost gh;
  struct itimerval ghost_timer;

  //configuracao do timer
  ghost_timer.it_interval.tv_sec = 0;
  ghost_timer.it_interval.tv_usec = INTERVAL;//intervalo
  ghost_timer.it_value.tv_sec = 0;
  ghost_timer.it_value.tv_usec = INTERVAL;//tempo ate o primeiro sinal
  setitimer(ITIMER_REAL, &ghost_timer, 0);
  signal(SIGALRM, ghost_timer_handler);

  game_window = newwin(MAX_Y, MAX_X, 1, 1);
  gh.position.x = gh.position.y = 0;
  gh.representation = 'g';
  md.representation = 64;
  md.position.x = md.last_x = MAX_X / 2;
  md.position.y = md.last_y = MAX_Y / 2;
  while((ch = getch()) != KEY_F(1)){
    switch(ch){
      case KEY_RIGHT:
        md.position = move_right(md.position);
        break;
      case KEY_LEFT:
        md.position = move_left(md.position);
        break;
      case KEY_UP:
        md.position = move_up(md.position);
        break;
      case KEY_DOWN:
        md.position = move_down(md.position);
        break;
    }
    if (ready_to_draw) {
      for (i = 0; i < MAX_GHOSTS; i++) {
        mvwaddch(game_window, gh.position.y, gh.position.x, gh.representation);
        gh.position = move_right(gh.position);
      }
      ready_to_draw = 0;
    }
    //move mr do
    //@TODO retirar isso quando tivermos o mapa em forma de matriz
    mvwaddch(game_window, md.last_y, md.last_x, ' ');//apaga a ultima posicao
    mvwaddch(game_window, md.position.y, md.position.x, md.representation);
    wrefresh(game_window);
    md.last_x = md.position.x;
    md.last_y = md.position.y;
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

//@TODO talvez seja melhor passar por referencia
struct moving_element move_right(struct moving_element position){
  if (position.x < MAX_X - 1) {
    position.x++;
  }
  return position;
}

struct moving_element move_left(struct moving_element position){
  if (position.x > 0) {
    position.x--;
  }
  return position;
}

struct moving_element move_up(struct moving_element position){
  if (position.y > 0) {
    position.y--;
  }
  return position;
}

struct moving_element move_down(struct moving_element position){
  if (position.y < MAX_Y - 1) {
    position.y++;
  }
  return position;
}
