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
void make_map(FILE *level, int p[MAX_Y][MAX_X]){

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
      while(buffer[cont] == '\n'){
        cont ++;
      }

      switch(buffer[cont]){

        case 'p':
                  p[i][j] = '#';
                  break;
        case 'v':
                  p[i][j] = ' ';
                  break;
        case 'd':
                  p[i][j] = ACS_PI;
                  break;
        case 'n':
                  p[i][j] = '&';
                  break;
        case 'f':
                  p[i][j] = 'f';
                  break;
        default: break;
      }
      cont++;
    }
  }
}

//Salva o mapa atual em um .txt para continuar o jogo
void write_map(int MAP[MAX_Y][MAX_X]){

  char buffer[MAX_Y][MAX_X];

  for(int i = 0; i < MAX_Y; i++){
    for(int j = 0; j < MAX_X; j++){
      switch(MAP[i][j]){

        case '#': 
                  buffer[i][j] = 'p';
                  break;
        case ' ': 
                  buffer[i][j] = 'v';
                  break;
        default:  break;
      }
    }
  }


  FILE *cont_map;
  cont_map = fopen("continue_level.txt", "wb");

  for (int i=0; i < MAX_Y; i++){
    for(int j = 0; j < MAX_X; j++){
        fprintf(cont_map, "%c", buffer[i][j]);
      }
    fprintf(cont_map, "%s", "\n");
  }
}
