#include "main.h"
#include "scores.h"
#include "mr_do_menus.h"
#include "file_operations.h"

int compare(struct score *elem1, struct score *elem2)
{
   if ( elem1->score < elem2->score)
      return -1;

   else if (elem1->score > elem2->score)
      return 1;

   else
      return 0;
}

void high_scores(){
  struct score highscores[TOP_SCORES];
  load_score(highscores);
  WINDOW *border_window = newwin(MAX_Y + 2, MAX_X + 2, 0, 0);
  WINDOW *score_window = newwin(MAX_Y, MAX_X, 1, 1);
  print_score(score_window, highscores);

  wrefresh(border_window);
  wrefresh(score_window);

  char opt = getchar();
  while ( opt != 'S' && opt != 's'){
    opt = getchar();
  }

  endwin();
  clear();
  show_menu();
}

void check_score(int score){
  clear();
  struct score highscores[TOP_SCORES];
  load_score(highscores);
  int scored = 0;
  for(int i = 0; i < TOP_SCORES; i++){
    if(score > highscores[i].score){
      scored = 1;
    }
  }
  if(scored){
    highscores[TOP_SCORES].score = score;
    printw( "Informe seu nome:\n");
    refresh();
    nodelay(stdscr, FALSE);
    echo();
    getnstr(highscores[TOP_SCORES].name, MAX_NAME);
    noecho();
    nodelay(stdscr, TRUE);
  }
  qsort((void *) &highscores,              // Endereço do primeiro elemento do array
   6,                                       // Número de elementos do array
   sizeof(struct score),                    // Tamanho de cada elemento do array
   (compfn)compare );                       // Ponteiro para função de comparação
  save_score(highscores);
  high_scores();
}

void print_score(WINDOW *w, struct score *sc){
  for(int i = 0; i<TOP_SCORES; i++){
    mvwprintw(w, i+1, 0, "Nome: %s", sc[i].name);
    mvwprintw(w, i+1, 30, "SCORE: %d", sc[i].score);
  }

  mvwprintw(w, 7, 0, "Pressione s para voltar ao menu principal...");
}
