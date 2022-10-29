#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_RESET "\033[37m"

typedef struct no{
    short int numero;
    char naipe;
    struct no *prox;
}tCarta;

tCarta *primMesa[8], *ult[8] = {NULL};
tCarta *temp, *primTemp = NULL;
tCarta *primMonte = NULL;
tCarta *primNaipe[4] = {NULL};
int random[52];

bool findVal(int *array, int size, int value) {
    for (int i = 0; i < size; ++i) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
}

void generateRandom(int array[], int length) {
    int temp;

    for (int i = 0; i < length; ++i) {
        temp = rand() % 52;
        if (findVal(array, i, temp)) {
            i--;
            continue;
        } else {
            array[i] = temp;
        }
    }
}

void gerarBaralho(){
    int pos = 0;
    tCarta *ant, *novo;

    for(int i=3;i<=6;i++){
        for(int j=1;j<=13;j++){
            novo = (tCarta *)malloc(sizeof(tCarta));
            novo->naipe = i;
            novo->numero = j;
            if(pos==0){
                novo->prox = NULL;
                primMonte = novo;
                ant = primMonte;
                pos++;
            }else{                
                ant->prox = novo;
                ant = novo;          
                pos++;
            }
        }
    }
    
}

void embaralhaBaralho(){
    tCarta *aux, *atual;
    int cont=51, pNum, pNaipe;

    while(cont>=0){
        atual = primMonte;
        aux = primMonte;
        pNum = atual->numero;
        pNaipe = atual->naipe;
        
        for (int i = 0; i < random[cont]; i++){
            atual = atual->prox;
        }
        aux->numero = atual->numero;
        aux->naipe = atual->naipe;
        atual->numero = pNum;
        atual->naipe = pNaipe;
    
        cont--;
    }
}

void distribuiMesa(){
    tCarta *atual, *aux, *auxUlt;

    atual = primMonte;
    for (int i = 0; i < 8; i++){
        if(i<4){
            for (int j = 0; j < 7; j++){
                if(j==0){
                    primMesa[i] = atual;
                    aux = primMesa[i];
                }else{
                    atual = atual->prox; 
                    aux->prox = atual;
                    aux = atual;
                }
            }
            atual=atual->prox;
            ult[i] = aux;
        }else if(i>=4){
            for (int j = 0; j < 6; j++){
                if(j==0){
                    primMesa[i] = atual;
                    aux = primMesa[i];
                }else{
                    atual = atual->prox; 
                    aux->prox = atual;
                    aux = atual;
                }
            }
            atual=atual->prox;
            ult[i] = aux;
        }
    }
    for (int k = 0; k < 8; k++)
    {
        ult[k]->prox = NULL;
    }
    
    
}

int imprime(){
    tCarta *atual = primMonte;
    tCarta *atual2;
    int i=1;

    for(int i=0;i<8;i++){
        atual2 = primMesa[i];
        printf("[MESA %d] : ", i);
        while(atual2!=NULL){
            if(atual2->naipe<=4)
                printf(ANSI_COLOR_RED "[%2d|%c]" ANSI_COLOR_RESET, atual2->numero, atual2->naipe);
            else
                printf("[%2d|%c]", atual2->numero, atual2->naipe);

            atual2 = atual2->prox;
        }
        printf("\n");
    }
}

int main(){
    srand(time(NULL));

    generateRandom(random, 52);
    gerarBaralho();
    embaralhaBaralho();
    distribuiMesa();
    imprime();


}