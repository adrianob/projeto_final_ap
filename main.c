/*Programa principal, contém apenas a função main para facilitar os testes unitários*/
#include "main.h"
#include "game.h"
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
