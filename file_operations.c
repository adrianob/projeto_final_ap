/*funções de leitura e escrita em arquivos*/
#include "file_operations.h"
#include "lists.h"

//Carrega o arquivo texto do cenário inicial
FILE* load_level(int level){
  FILE *level_file = NULL;

  if(level == 1){
    if((level_file = fopen("fase1.txt", "rb")) == NULL)
      printf("erro ao abrir arquivo de fase\n");
  }else if(level == 2){
    if((level_file = fopen("fase2.txt", "rb")) == NULL)
      printf("erro ao abrir arquivo de fase\n");
  }

  return level_file;
}

//Gera a matriz do mapa a partir do arquivo de texto
void make_map(FILE *level, chtype (*p)[MAX_X]){
  char buffer;

  rewind(level);

  //Atualiza a matriz, ignorando caracteres fora do padrão
  for(int i = 0; i < MAX_Y; i++){
    for(int j = 0; j < MAX_X; j++){
      if(fread(&buffer, sizeof(char), 1, level) != 1){
        printw("erro na leitura do arquivo\n");
      }
      p[i][j] = (chtype)buffer;
    }
    //pula \n
    if(fseek(level, sizeof(char), SEEK_CUR) != 0){
      printw("erro ao ler arquivo\n");
    }
  }
  fclose(level);

}

//le o arquivo binario com a lista de sprites e recria as lista para dar continuidade ao jogo
void load_state(struct sprite_list *sprite_list){
  FILE *game_state_file;
  if((game_state_file = fopen("estado.bin", "rb")) == NULL){
    printw("erro ao abrir arquivo de scores\n");
  }
  else{
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
}

//Salva o estado atual do jogo em um arquivo binário
void save_state(struct sprite_list sl){
  //salva lista de sprites
  FILE *game_state_file;
  game_state_file = fopen("estado.bin", "wb+");
  if (game_state_file) {
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
  }
  else{
    printw("erro ao escrever no arquivo\n");
  }
}

//salva o mapa atual em um arquivo texto
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
  cont_map = fopen("continuar.txt", "wb+");
  if (cont_map) {
    for (int i=0; i < MAX_Y; i++){
      for(int j = 0; j < MAX_X; j++){
          fprintf(cont_map, "%c", (char)buffer[i][j]);
        }
      fprintf(cont_map, "%s", "\n");
    }
    fclose(cont_map);
  }
  else{
    printw("erro ao abrir arquivo\n");
  }
}

void save_score(struct score *hi_score){
  FILE *hi_score_file;
  hi_score_file = fopen("highscores.bin","wb+");
  if (hi_score_file){
    for(int i = TOP_SCORES; i >= 0 ; i--){
      fwrite(&hi_score[i], sizeof(struct score), 1, hi_score_file);
    }
    fclose(hi_score_file);
  }
  else{
    printw("erro ao abrir arquivo\n");
  }
}

void load_score(struct score *hi_score){
  FILE *hi_score_file;
  hi_score_file = fopen("highscores.bin","rb");
  if (!hi_score_file){
    for(int i = 0; i < TOP_SCORES; i++){
      hi_score[i].name[0] = '\0';
      hi_score[i].score = 0;
    }
  }else{
    for(int i = 0; i < TOP_SCORES; i++){
      if(!fread(&hi_score[i], sizeof(struct score), 1, hi_score_file)){
        hi_score[i].name[0] = '\0';
        hi_score[i].score = 0;
      }
    }
    fclose(hi_score_file);
  }
}
