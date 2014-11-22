/*funções utilizadas diretamente no jogo*/
#include "main.h"
#include "game.h"
#include "file_operations.h"
#include "mr_do_menus.h"
#include "lists.h"
#include "sprites.h"
#include "movement.h"

//função pricipal que cria a tela do jogo, recebe comandos do usuário e controla toda a lógica do jogo
void play(void){
  unsigned int ghost_timer = 0;
  //cria a janela do jogo dentro da borda
  WINDOW *border_window = newwin(MAX_Y + 2, MAX_X + 2, 0, 0);
  WINDOW *game_window = newwin(MAX_Y, MAX_X, 1, 1);
  WINDOW *info_window = newwin(10, 13, 0, MAX_X + 5);
  box(border_window, 0, 0);

  config_timer();

  chtype MAP[MAX_Y][MAX_X];
  struct sprite_list sprite_list= {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
  if (game_state.saved_game) {//continua
    load_state(&sprite_list);
  }
  else{
    make_map(load_level(game_state.level), MAP);//le o arquivo da fase e carrega na matriz
    make_lists(MAP, &sprite_list);//cria uma lista de sprites a partir da matriz da fase
  }

  int ch, should_move = 0;
  enum direction mrdo_direction;

  while((ch = getch()) != 27){//loop principal, espera tecla esc para sair
    if(sprite_list.mr_do->alive){
      if (ch == ' ') {
        create_shot(&sprite_list);
      }
      else {
        if (valid_key(ch)) {
          mrdo_direction = get_keyboard_direction(ch);
          should_move = 1;
        }
      }
    }

    if (timer_ready) {
      ghost_timer++;

      if (ghost_timer == (GHOST_INTERVAL / INTERVAL)) {//hora de criar novos fantasmas
        create_ghost(&sprite_list);
        ghost_timer = 0;
      }

      move_ghosts(game_window, sprite_list.ghosts);
      if (should_move) {
        move_sprite(game_window, sprite_list.mr_do, mrdo_direction);
        should_move = 0;
      }

      if(sprite_list.shot->alive){
        move_shot(game_window, sprite_list.shot);
      }
      //checar colisoes so depois de mover todos sprites
      check_sprite_collision(&sprite_list);

      timer_ready = FALSE;
    }

    print_map(game_window, sprite_list);
    show_info(info_window, sprite_list);
    check_state(info_window, sprite_list);
    refresh_windows(info_window, game_window, border_window);
  }//fim loop principal

  save_state(sprite_list);
  show_menu();
}

//continua o jogo a partir de um jogo salvo
void continue_play(void){
  game_state.saved_game = 1;
  play();
}

void play_level_2(void){
  game_state.level = 2;
  game_state.saved_game = 0;
  play();
}

void new_game(void){
  game_state.saved_game = 0;
  game_state.score = 0;
  game_state.level = 1;
  play();
}

void refresh_windows(WINDOW *info_window, WINDOW *game_window, WINDOW *border_window){
  wrefresh(info_window);
  wrefresh(border_window);
  wrefresh(game_window);
}

void exit_game(void){
  endwin();
  exit(EXIT_SUCCESS);
}

//mostra informações sobre o jogo na tela
void show_info(WINDOW *w, struct sprite_list sl){
  int alive_ghosts = count_alive(sl.ghosts);
  int alive_fruits = count_alive(sl.fruits);
  int created_ghosts = list_size(sl.ghosts);

  if(!sl.mr_do->alive) {
    mvwprintw(w, 2, 0, "GAME OVER!");
  }else{
    mvwprintw(w, 1, 0, "SCORE: %d", game_state.score);
    mvwprintw(w, 2, 0, "Fruits: %d ", alive_fruits);
    mvwprintw(w, 5, 0, "--GHOSTS--");
    mvwprintw(w, 6, 0, "Remaining  %d ", MAX_GHOSTS - created_ghosts);
    mvwprintw(w, 7, 0, "Alive      %d ", alive_ghosts);
    mvwprintw(w, 8, 0, "Killed     %d ", (created_ghosts - alive_ghosts));
    mvwprintw(w, 9, 0, "LEVEL      %d ", game_state.level);
  }
}

//verifica se passou de fase
void check_state(WINDOW *w, struct sprite_list sl){

  int alive_ghosts = count_alive(sl.ghosts);
  int alive_fruits = count_alive(sl.fruits);
  int created_ghosts = list_size(sl.ghosts);

  if ((alive_ghosts == 0 && created_ghosts == MAX_GHOSTS) || (alive_fruits == 0)) {//ganhou a fase
    mvwprintw(w, 2, 0, "YOU WIN! ");
    if (game_state.level == 1) {
      play_level_2();
    }
    else{
      check_score(game_state.score);
    }
  }
}

//configurações iniciais
void config(void){
  srand(time(NULL));
  setlocale(LC_ALL, "");
  initscr();			//inicia modo ncurses
  cbreak(); //não espera nova linha para capturar
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);		//pega teclas especiais
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_GREEN, COLOR_BLACK);
  noecho();			//nao imprime na tela teclas no usuario
  curs_set(0); //nao imprime cursor
}

//inicialização do timer
void config_timer(void){
  struct itimerval timer;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = INTERVAL;//intervalo
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = INTERVAL;//tempo ate o primeiro sinal
  setitimer(ITIMER_REAL, &timer, 0);
  signal(SIGALRM, timer_handler);
}

//função de retorno do timer
void timer_handler(int i){
  timer_ready = 1;
}

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
