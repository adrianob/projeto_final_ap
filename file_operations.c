#include "file_operations.h"

//Carrega o arquivo de cenário inicial
//@TODO tratar erros?
FILE* load_level(int level){
  FILE *level_file;

  if(level == 1){
    level_file = fopen("fase1.txt", "rb");
  }else if(level == 2){
    level_file = fopen("fase2.txt", "rb");
  }else{
    level_file = fopen("continuar.txt","rb");
  }

  return level_file;
}

//Gera o mapa de acordo com o arquivo de texto
//@TODO substituir os caracteres pelos corretos e tratamento de erros
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

//Salva o mapa atual em um .txt para continuar o jogo
void save_map(WINDOW *game_window){

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
