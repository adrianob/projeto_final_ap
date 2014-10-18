#include "main.h"
#include "lists.h"
#include "movement.h"
#include "sprites.h"
#include "file_operations.h"
#include "mr_do_menus.h"

int timer_ready = 0;
struct game_state game_state = {.score = 0, .level = 1};

int main(int argc, const char *argv[]){
  config();
  show_menu();

  return 0;
}

void play(void){
  unsigned int ghost_timer = 0;
  //cria a janela do jogo dentro da borda
  WINDOW *border_window = newwin(MAX_Y + 2, MAX_X + 2, 0, 0);
  WINDOW *game_window = newwin(MAX_Y, MAX_X, 1, 1);
  WINDOW *info_window = newwin(10, 13, 0, MAX_X + 5);
  box(border_window, 0, 0);

  config_timer();

  chtype MAP[MAX_Y][MAX_X];
  make_map(load_level(game_state.level), MAP);
  struct sprite_list sprite_list= {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
  make_lists(MAP, &sprite_list);
  int ch, mrdo_direction;
  while((ch = getch()) != KEY_F(1)){
    if(sprite_list.mr_do->alive){
      switch(ch){
        case KEY_RIGHT:
          mrdo_direction = RIGHT_DIRECTION;
          break;
        case KEY_LEFT:
          mrdo_direction = LEFT_DIRECTION;
          break;
        case KEY_UP:
          mrdo_direction = UP_DIRECTION;
          break;
        case KEY_DOWN:
          mrdo_direction = DOWN_DIRECTION;
          break;
        case ' ':
          //@TODO colocar numa funcao separada
          if (list_size(sprite_list.shot) < 1) {//cria o tiro
            sprite shot = DEFAULT_SHOT;
            push(&sprite_list.shot, shot);
          }
          if (!sprite_list.shot->alive) {
            sprite_list.shot->alive = 1;
            sprite_list.shot->position = find_char(&sprite_list, CH_MR_DO);
            sprite_list.shot->direction = sprite_list.mr_do->direction;
          }
          break;
      }
    }

    if (timer_ready) {
      ghost_timer++;

      if (ghost_timer == (GHOST_INTERVAL / INTERVAL)) {//hora de criar novos fantasmas
        if (list_size(sprite_list.ghosts) < MAX_GHOSTS) {
          sprite ghost = DEFAULT_GHOST;
          ghost.position = find_char(&sprite_list, CH_NEST);
          push(&sprite_list.ghosts, ghost);
        }
        ghost_timer = 0;
      }

      move_ghosts(game_window, sprite_list.ghosts);
      move_sprite(game_window, sprite_list.mr_do, mrdo_direction);
      //checar colisoes so depois de mover todos sprites
      if(list_size(sprite_list.shot) > 0 && sprite_list.shot->alive){
        move_shot(game_window, sprite_list.shot);
        check_sprite_collision(&sprite_list, sprite_list.shot);
      }
      check_sprite_collision(&sprite_list, sprite_list.mr_do);
      check_ghosts_collision(&sprite_list, sprite_list.ghosts);
      mrdo_direction = 0;
      timer_ready = 0;
    }

    print_lists(game_window, sprite_list);
    check_state(info_window, sprite_list);
    refresh_windows(info_window, game_window, border_window);
  }

  endwin();
}

void refresh_windows(WINDOW *info_window, WINDOW *game_window, WINDOW *border_window){
  wrefresh(info_window);
  wrefresh(border_window);
  wrefresh(game_window);
}

void check_state(WINDOW *w, struct sprite_list sl){

  int alive_ghosts = count_alive(sl.ghosts);
  int alive_fruits = count_alive(sl.fruits);
  int created_ghosts = list_size(sl.ghosts);

  if ((alive_ghosts == 0 && created_ghosts == MAX_GHOSTS) || (!alive_fruits)) {
    mvwprintw(w, 2, 0, "YOU WIN! ");
    game_state.level = 2;
    play();
  }
  else if(!sl.mr_do->alive) {
    mvwprintw(w, 2, 0, "GAME OVER!");
  }else{
    mvwprintw(w, 1, 0, "SCORE: %d", game_state.score);
    mvwprintw(w, 2, 0, "Fruits: %d ", alive_fruits);
    mvwprintw(w, 5, 0, "--GHOSTS--");
    mvwprintw(w, 6, 0, "Remaining  %d ", MAX_GHOSTS - created_ghosts);
    mvwprintw(w, 7, 0, "Alive      %d ", alive_ghosts);
    mvwprintw(w, 8, 0, "Killed     %d ", (created_ghosts - alive_ghosts));
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
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_GREEN, COLOR_BLACK);
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
