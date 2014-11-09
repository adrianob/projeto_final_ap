/*funçõs que trabalham com listas encadeadas*/
#include "lists.h"

//insere um novo sprite no inicio da lista encadeada
void push(SPRITE **head_ref, SPRITE s) {
  SPRITE *node = (SPRITE *) malloc(sizeof(SPRITE));
  *node = s;
  node->next = *head_ref;
  *head_ref = node;
}

//imprime uma lista de sprites na janela
void print_list(WINDOW *w, SPRITE *head){
  SPRITE *current = head;
  while(current != NULL){
    if (current->alive) {
      mvwaddch(w, current->position.y, current->position.x, current->representation);
    }
    current = current->next;
  }
}

//imprime todos os sprites contidos na lista na tela
void print_map(WINDOW *w, struct sprite_list sl){
  //cuidado com a ordem! imprime por cima se tiver dois sprites no mesmo lugar
  SPRITE *list[] = {sl.spaces, sl.walls, sl.fruits, sl.ghosts, sl.shot, sl.nest, sl.mr_do};
  for (int i = 0; i < sizeof(list)/sizeof(SPRITE*); i++) {
    print_list(w, list[i]);
  }
}

//retorna a posicao do caractere no mapa
struct position find_char(struct sprite_list *sl, chtype ch){
  struct position position;
  SPRITE *list[] = {sl->mr_do, sl->nest};
  for (int i = 0; i < (int)sizeof(list)/sizeof(SPRITE*); i++) {
    SPRITE *current = list[i];
    while(current != NULL){
      if (current->representation == ch) {
        position = current->position;
      }
      current = current->next;
    }
  }
  return position;
}

int list_size(SPRITE *sp){
  int count = 0;
  SPRITE *current = sp;
  while(current != NULL){
    count++;
    current = current->next;
  }
  return count;
}

//retorna quantidade de sprites vivos numa lista
int count_alive(SPRITE *sp){
  int count = 0;
  SPRITE *current = sp;
  while(current != NULL){
    if (current->alive) {
      count++;
    }
    current = current->next;
  }
  return count;
}
