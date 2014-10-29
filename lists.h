#ifndef MR_DO_LISTS
#define MR_DO_LISTS

#include "main.h"

void push(SPRITE **head_ref, SPRITE s);
void print_list(WINDOW *w, SPRITE *head);
void print_lists(WINDOW *w, struct sprite_list sl);
int list_size(SPRITE *sp);
struct position find_char(struct sprite_list *sl, chtype ch);
int count_alive(SPRITE *sp);

#endif
