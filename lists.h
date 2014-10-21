#ifndef MR_DO_LISTS
#define MR_DO_LISTS

#include "main.h"

void push(sprite **head_ref, sprite s);
void print_list(WINDOW *w, sprite *head);
void print_lists(WINDOW *w, struct sprite_list sl);
int list_size(sprite *sp);
struct position find_char(struct sprite_list *sl, chtype ch);
int count_alive(sprite *sp);

#endif
