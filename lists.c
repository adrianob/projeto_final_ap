#include "lists.h"

//insere um novo sprite no inicio da lista encadeada
void push(sprite **head_ref, sprite s) {
  sprite *node = (sprite *) malloc(sizeof(struct sprite));
  *node = s;
  node->next = *head_ref;
  *head_ref = node;
}

//imprime uma lista de sprites na janela
void print_list(WINDOW *w, sprite *head){
  sprite *current = head;
  while(current != NULL){
    if (current->alive) {
      mvwaddch(w, current->position.y, current->position.x, current->representation);
    }
    current = current->next;
  }
}

void print_lists(WINDOW *w, struct sprite_list sl){
  //cuidado com a ordem! imprime por cima se tiver dois sprites no mesmo lugar
  sprite *list[] = {sl.spaces, sl.walls, sl.fruits, sl.ghosts, sl. shot, sl.nest, sl.mr_do};
  for (int i = 0; i < sizeof(list)/sizeof(sprite*); i++) {
    print_list(w, list[i]);
  }
}

//retorna a posicao do caractere no mapa
struct position find_char(struct sprite_list *sl, chtype ch){
  struct position position;
  sprite *list[] = {sl->mr_do, sl->nest};
  for (int i = 0; i < (int)sizeof(list)/sizeof(sprite*); i++) {
    sprite *current = list[i];
    while(current != NULL){
      if (current->representation == ch) {
        position = current->position;
      }
      current = current->next;
    }
  }
  return position;
}

int list_size(sprite *sp){
  int count = 0;
  sprite *current = sp;
  while(current != NULL){
    count++;
    current = current->next;
  }
  return count;
}

int count_alive(sprite *sp){
  int count = 0;
  sprite *current = sp;
  while(current != NULL){
    if (current->alive) {
      count++;
    }
    current = current->next;
  }
  return count;
}
