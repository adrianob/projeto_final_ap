#ifndef MR_DO_FILE
#define MR_DO_FILE
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

FILE* load_level(int level); //Recebe o level (1 - nível um, 2 - nível dois, qualquer outra coisa - nível 3) e retorna um FILE
void make_map(FILE * level, char p[MAX_Y][MAX_X]);  //Recebe um FILE e o ponteiro da matriz e inicializa o mapa
void write_map(char MAP[MAX_Y][MAX_X]);             //Salva o mapa atual em um .txt para continuar o jogo
#endif
