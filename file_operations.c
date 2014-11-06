/*funções de leitura e escrita em arquivos*/
#include "file_operations.h"
#include "lists.h"
#include <string.h>

//Carrega o arquivo texto do cenário inicial
FILE* load_level(int level){
  FILE *level_file = NULL;

  if(level == 1){
    level_file = fopen("fase1.txt", "rb");
  }else if(level == 2){
    level_file = fopen("fase2.txt", "rb");
  }

  return level_file;
}

//Gera a matriz do mapa a partir do arquivo de texto
void make_map(FILE *level, chtype (*p)[MAX_X]){
  long l_size;
  char *buffer;
  int i, j, cont;

  //Aloca espaço na memória e lê o arquivo carregado
  fseek(level, 0L, SEEK_END);
  l_size = ftell(level);
  rewind(level);
  buffer = calloc( 1, l_size + 1 );
  fread(buffer, l_size, 1, level);
  cont = 0;

  //Atualiza a matriz, ignorando caracteres fora do padrão
  for(i = 0; i < MAX_Y; i++){
    for(j = 0; j < MAX_X; j++){
      //ignora novas linhas
      while(buffer[cont] == '\n'){
        cont ++;
      }
      p[i][j] = buffer[cont];
      cont++;
    }
  }
  fclose(level);
}

//le o arquivo binario com a lista de sprites e recria as lista para dar continuidade ao jogo
void load_state(struct sprite_list *sprite_list){
  FILE *game_state_file;
  game_state_file = fopen("estado.bin", "rb");
  SPRITE current;
  //le o score e o level no inicio do arquivo
  fread(&(game_state.score), sizeof(int), 1, game_state_file);
  fread(&(game_state.level), sizeof(int), 1, game_state_file);
  //le as listas de sprites de cada tipo em sprite_list
  for (int i = 0; i < sizeof(*sprite_list)/sizeof(SPRITE*); i++) {
    do{
      fread(&current, sizeof(SPRITE), 1, game_state_file);
      switch(current.representation){
        case CH_SPACE:
          push(&(sprite_list->spaces), current);
          break;
        case CH_WALL:
          push(&(sprite_list->walls), current);
          break;
        case CH_FRUIT:
          push(&(sprite_list->fruits), current);
          break;
        case CH_GHOST:
          push(&(sprite_list->ghosts), current);
          break;
        case CH_SHOT:
          push(&(sprite_list->shot), current);
          break;
        case CH_NEST:
          push(&(sprite_list->nest), current);
          break;
        case CH_MR_DO:
          push(&(sprite_list->mr_do), current);
          break;
      }
    } while(current.next != NULL);
  }
  fclose(game_state_file);
}

//Salva o estado atual do jogo em um arquivo binário 
void save_map(WINDOW *game_window, struct sprite_list sl){
  //salva lista de sprites
  FILE *game_state_file;
  game_state_file = fopen("estado.bin", "wb");
  //salva o score o level no começo do arquivo
  fwrite(&(game_state.score), sizeof(int), 1, game_state_file);
  fwrite(&(game_state.level), sizeof(int), 1, game_state_file);
  SPRITE *list[] = {sl.spaces, sl.walls, sl.fruits, sl.ghosts, sl.nest, sl.mr_do, sl.shot};
  //salva todas as listas no arquivo binário
  for (int i = 0; i < sizeof(list)/sizeof(SPRITE*); i++) {
    SPRITE *current = list[i];
    while(current != NULL){
      fwrite(current, sizeof(SPRITE), 1, game_state_file);
      current = current->next;
    }
  }
  fclose(game_state_file);

  //salva o mapa atual em um arquivo texto, sem utilidade por enquanto
  chtype buffer[MAX_Y][MAX_X];

  for(int i = 0; i < MAX_Y; i++){
    for(int j = 0; j < MAX_X; j++){
      buffer[i][j] = mvwinch(game_window, i, j);
      switch(buffer[i][j]){
        case CH_WALL:
          buffer[i][j] = 'p';
          break;
        case CH_SHOT:
          buffer[i][j] = 't';
          break;
        case CH_GHOST:
          buffer[i][j] = 'i';
          break;
        case CH_FRUIT:
          buffer[i][j] = 'f';
          break;
        case CH_MR_DO:
          buffer[i][j] = 'd';
          break;
        case CH_NEST:
          buffer[i][j] = 'n';
          break;
        case CH_SPACE:
          buffer[i][j] = 'v';
          break;
      }
    }
  }

  FILE *cont_map;
  cont_map = fopen("continuar.txt", "wb");

  for (int i=0; i < MAX_Y; i++){
    for(int j = 0; j < MAX_X; j++){
        fprintf(cont_map, "%c", buffer[i][j]);
      }
    fprintf(cont_map, "%s", "\n");
  }
  fclose(cont_map);
}
