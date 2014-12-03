#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "path.h"

//Partindo do destino faz o caminho contrário e descobre o próximo passo a ser dado
NODE *nextStep(NODE (*map_node)[60], struct position destiny){
    NODE *dest = &map_node[destiny.y][destiny.x];
    while(dest->parent->parent != NULL){
        dest = dest->parent;
    }
    return dest;
}

//Criação do mapa e cálculo do custo H
void createMap(NODE (*map_node)[60], char (*map_char)[60], struct position destiny){
    for(int i=0; i<MAX_Y; i++){
        for(int j=0; j<MAX_X; j++){
            map_node[i][j].pos.y = i;
            map_node[i][j].pos.x = j;
            map_node[i][j].h = fabs(destiny.x - map_node[i][j].pos.x) + fabs(destiny.y - map_node[i][j].pos.y); //H é uma estimativa (superestimada) do quanto falta para chegar ao destino
            map_node[i][j].parent = NULL;
            map_node[i][j].status = 0;
            if(map_char[i][j] == '1'){      //substituir '1' pelo char correspondente à parede
                map_node[i][j].pass = 0;
            }else{
                map_node[i][j].pass = 1;
            }
        }
    }
}

//Retorna os quadrados adjacentes. No caso de adjacente fora da área válida retorna um ponteiro NULL
void findAdjacents(struct position pos, NODE **adjacents, NODE (*map_node)[60]){
    if(pos.y > 0){
        adjacents[0] = &map_node[pos.y -1][pos.x];
    }else{
        adjacents[0] = NULL;
    }
    if(pos.x > 0){
        adjacents[1] = &map_node[pos.y][pos.x -1];
    }else{
        adjacents[1] = NULL;
    }
    if(pos.y < 22){
        adjacents[2] = &map_node[pos.y +1][pos.x];
    }else{
        adjacents[2] = NULL;
    }
    if(pos.x < 59){
        adjacents[3] = &map_node[pos.y][pos.x +1];
    }else{
        adjacents[3] = NULL;

    }
}

//Varre a matriz em busca do menor F
NODE *findLowerF(NODE (*map_node)[60]){
    NODE *lower = map_node[0];
    for(int i=0; i<MAX_Y; i++){
        for(int j=0; j<MAX_X; j++){
            if( (lower->status != 1) || (map_node[i][j].f < lower->f && map_node[i][j].status == 1)){
                lower = &map_node[i][j];
            }
        }
    }
    return lower;
}

//Cálculo dos pesos G (passos dados desde o início) e F (soma de G e H)
void writeWeight(NODE currentNode, NODE **adjacent){
    (*adjacent)->g = currentNode.g + 10;
    (*adjacent)->f = (*adjacent)->g + (*adjacent)->h;
}

//Encontra qual possível parent é o melhor caminho através do custo G dos quadrados adjacentes
int isBetterWay(NODE *currentNode, NODE *adjacentParent){
    return (currentNode->g < adjacentParent->g);
}

//Função principal
int findPath(NODE (*map_node)[60], struct position destiny){

    NODE *currentNode = findLowerF(map_node);                           //Encontra menor F (nó corrente)
    currentNode->status = -1;                                           //Move para a lista fechada
    NODE *adjacents[4];                                                 //Array de ponteiros para os adjacentes
    findAdjacents(currentNode->pos, adjacents, map_node);               //Encontra Adjacentes

    for (int i=0; i < 4; i++){                                          //Para cada adjacente...
        if(adjacents[i] != NULL){                                       //ignora se for nulo, não passável ou se estiver na lista fechada
            if(adjacents[i]->pass && adjacents[i]->status != -1){
                if(adjacents[i]->status != 1){                          //Se não estiver na lista aberta move o nó para a lista aberta, faz o quadrado corrente o pai dele e calcula os pesos F e G
                    adjacents[i]->status = 1;
                    adjacents[i]->parent = currentNode;
                    writeWeight(*currentNode, adjacents+i);
                }else if(adjacents[i]->status == 1){                        //Se já estiver na lista aberta verifica se o caminho atual não é melhor
                    if(isBetterWay(currentNode, adjacents[i]->parent)){   //se for, atualiza o parent e os custos F e G
                        adjacents[i]->parent = currentNode;
                        writeWeight(*currentNode, adjacents+i);
                    }
                }
            }
        }
    }
                                                                        //Argumento de parada:
    if (isDestinyInClosedList(map_node, destiny)){                      //Procura destino na lista fechada, se estiver, o caminho foi encontrado
        return 1;
    }else if (isOpenListEmpty(map_node)){
        return 0;                                                       //Se não, verifica se existe algum elemento na lista aberta, se não tiver, não existe um caminho
    }else{
        return findPath(map_node, destiny);                             //Se a lista aberta contém elementos, é possível que exista um caminho. Chamada recursiva para a função
    }

}

//Procura destino na lista fechada
int isDestinyInClosedList(NODE (*map_node)[60], struct position destiny){
    if(map_node[destiny.y][destiny.x].status == -1){
        return 1;
    }else{
        return 0;
    }
}

//Procura qualquer elemento na lista aberta
int isOpenListEmpty(NODE (*map_node)[60]){
    for(int i=0; i<MAX_Y; i++){
        for(int j=0; j<MAX_X; j++){
            if(map_node[i][j].status==1){
                return 0;
            }
        }
    }
    return 1;
}
