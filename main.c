#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <ncurses.h>
#include <unistd.h>
#include "main.h"
#include "movement.h"

int ready_to_draw = 0;

int main(int argc, const char *argv[]){
  init();
  WINDOW *game_window;
  char MAP[MAX_X][MAX_Y];
  int ch, i;
  struct mr_do md;
  struct ghost gh;
  struct itimerval timer;

  //configuracao do timer
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = INTERVAL;//intervalo
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = INTERVAL;//tempo ate o primeiro sinal
  setitimer(ITIMER_REAL, &timer, 0);
  signal(SIGALRM, timer_handler);

  game_window = newwin(MAX_Y, MAX_X, 1, 1);
  gh.position.x = gh.position.y = 0;
  gh.representation = 'g';
  md.representation = 64;
  md.position.x = md.last_x = MAX_X / 2;
  md.position.y = md.last_y = MAX_Y / 2;
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
      for (i = 0; i < MAX_GHOSTS; i++) {
        mvwaddch(game_window, gh.position.y, gh.position.x, gh.representation);
        move_right(&gh.position);
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

//Carrega o arquivo de cenário inicial
//@TODO tratar erros?
FILE* load_level(int level){
  FILE *level_file;

  if(level == 1){
    level_file = fopen("level1.txt", "r");
  }else if(level == 2){
    level_file = fopen("level2.txt", "r");
  }else{
    level_file = fopen("continua.txt","r");
  }

  return level_file;
}

//Gera o mapa de acordo com o arquivo de texto
//@TODO substituir os caracteres pelos corretos e tratamento de erros
void make_map(FILE *level, char *p){

  long lSize;
  char *buffer;
  int i, j, cont;

  //Aloca espaço na memória e lê o arquivo carregado
  fseek(level,0L,SEEK_END);
  lSize = ftell(level);
  rewind(level);
  buffer = calloc( 1, lSize+1 );
  fread(buffer,lSize,1,level);

  //Atualiza a matriz, ignorando caracteres fora do padrão
  for(i = 0; i < MAX_Y; i++){
    for(j = 0; j < MAX_X; j++){
      while(buffer[cont] != '@' && buffer[cont] != '.'){
        cont ++;
      }
      switch(buffer[cont]){

      case '@': *p = 'E';
                p++;
                break;
      case '.': *p = ' ';
                p++;
                break;
      default: break;
      }
      cont++;
    }
  }

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
