#ifndef MR_DO_FILE
#define MR_DO_FILE
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

struct game_state game_state;

FILE* load_level(int level); //Recebe o level (1 - nível um, 2 - nível dois, qualquer outra coisa - nível 3) e retorna um FILE
void make_map(FILE * level, chtype (*p)[MAX_X]);  //Recebe um FILE e o ponteiro da matriz e inicializa o mapa
void save_map(WINDOW *game_window, struct sprite_list sl);
void load_state(struct sprite_list *sprite_list);
void save_score(struct score *hi_score);
void load_score(struct score *hi_score);


#endif
