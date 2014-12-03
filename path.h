#ifndef MR_DO_PATH
#define MR_DO_PATH
typedef struct node{
    struct position pos;    //coordenadas
    int f;                  //peso (G+H)
    int g;                  //quadrados de distância do ponto inicial
    int h;                  //estimativa do numero de quadrados que faltam
    struct node *parent;    //quadrado adjacente com menor custo G
    int status;             //0 = default; 1 = lista aberta; -1 = lista fechada
    int pass;               //é passável? 0 = não; 1 = sim
} NODE;

void createMap(NODE (*map_node)[60], char (*map_char)[60], struct position destiny);
void findDestiny(struct position *destiny, char (*map_char)[60]);

void findAdjacents(struct position pos, NODE **adjacents, NODE (*map_node)[60]);
NODE *findLowerF(NODE (*map_node)[60]);
int findPath(NODE (*map_node)[60], struct position destiny);
void writeWeight(NODE currentNode, NODE **adjacents);
int isBetterWay(NODE *currentNode, NODE *adjacentParent);
int isDestinyInClosedList(NODE (*map_node)[60], struct position destiny);
int isOpenListEmpty(NODE (*map_node)[60]);
NODE  *nextStep(NODE (*map_node)[60], struct position destiny);
#endif
