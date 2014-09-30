#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#define MAX_X 80 //tamanho de colunas do mapa principal
#define MAX_Y 30 //tamanho de linhas do mapa principal

struct mr_do {
	int start_x, start_y, last_x, last_y, state;//como a localizacao do mr.do vai ser sempre a mesma do cursor so precisamos do valor inicial
  char representation;
};

void init(void);
void draw_borders(void);

int main(int argc, const char *argv[]){
  init();
  int ch, x, y;
  struct mr_do md;
  WINDOW *game_window;
  game_window = newwin(MAX_Y, MAX_X, 1, 1);
  md.representation = 64;
  md.start_x = x = md.last_x = MAX_X / 2;
  md.start_y = y = md.last_y = MAX_Y / 2;
  mvwaddch(game_window, md.start_y, md.start_x, md.representation);
  wrefresh(game_window);
  while((ch = getch()) != 27){//esc
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
    mvwaddch(game_window, md.last_y, md.last_x, ' ');
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
