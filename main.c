#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#define MAX_X 80
#define MAX_Y 30

void gotoxy(int x, int y) {
    printf("\x1b[%d;%df", y, x);
}

int main(int argc, const char *argv[]){
  int ch, i, j;

	initscr();			/* Start curses mode 		*/
	cbreak();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */
  curs_set(0);
  /*
  for (i = 0; i < MAX_X; i++) {
    mvaddch(0, i, '=');
  }
  */
  i = j = 20;
  mvaddch(20, 20, '&');
  refresh();			/* Print it on to the real screen */
  while(1){
    ch = getch();
    if(ch == KEY_RIGHT && i < MAX_X){
      i++;
    }
    else if (ch == KEY_LEFT && i > 1) {
      i--;
    }
    else if (ch == KEY_UP && j > 1) {
      j--;
    }
    else if (ch == KEY_DOWN && j < MAX_Y) {
      j++;
    }
    erase();
    mvaddch(j, i, '&');
    refresh();			/* Print it on to the real screen */
  }
    endwin();			/* End curses mode		  */
    return 0;
}
