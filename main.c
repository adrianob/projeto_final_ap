/*Programa principal, contém apenas a função main para facilitar os testes unitários*/
#include "mr_do_menus.h"

int timer_ready = 0;

int main(int argc, const char *argv[]){
  config();
  show_menu();

  return 0;
}
