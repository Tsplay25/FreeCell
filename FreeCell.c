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
tCarta *temp[4] = {NULL};
tCarta *primMonte = NULL;
tCarta *primNaipe[4] = {NULL};
int dispo[4] = {1, 1, 1, 1};
int random[52];

void bold(int status) {
    static const char *seq[] = {"\x1b[0m", "\x1b[1m"};
    printf("%s", seq[!!status]);
}

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

int movValido(tCarta *tempo, tCarta *mesa){
    if(mesa==NULL){
        return 1;
    }else if(tempo->numero == mesa->numero-1 && ((tempo->naipe <=4 && mesa->naipe >=5) || (tempo->naipe >=5 && mesa->naipe <=4))){
        return 2;
    }else{
        return 3;
    }
}

bool naipeValido(tCarta *mesa, tCarta *naipe){
    if(naipe==NULL && mesa->numero == 1){
        return true;
    }else if(naipe!=NULL && naipe->numero == mesa->numero - 1){
        return true;
    }else{
        return false;
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

    for (int k = 0; k < 8; k++){
        ult[k]->prox = NULL;
    }
}

void imprime(){
    tCarta *atual, *atual2, *atual3;
    int i=1;

    //naipe
    for (int i = 0; i < 4; i++){
        atual3=primNaipe[i];
        bold(1);
        printf("[NAIPE %d] : ", i);
        bold(0);
        while(atual3!=NULL){
            if(atual3->naipe<=4)
                printf(ANSI_COLOR_RED "[%2d|%c]" ANSI_COLOR_RESET, atual3->numero, atual3->naipe);
            else
                printf("[%2d|%c]", atual3->numero, atual3->naipe);

            atual3 = atual3->prox;
        }
        printf("\n");

    }
    printf("\n");

    // temp
    bold(1);
    printf("[TEMP] :\n");
    bold(0);
    for(int i=0;i<4;i++){
        atual = temp[i];
        if(temp[i]==NULL){
            printf("%d - [    ]\n", i);
        }else{
            if(atual->naipe<=4){
                printf("%d - ", i);
                printf(ANSI_COLOR_RED "[%2d|%c]\n" ANSI_COLOR_RESET, atual->numero, atual->naipe);
            }
            else
                printf("%d - [%2d|%c]\n", i, atual->numero, atual->naipe);
        }
    }
    printf("\n");
    //mesa
    for(int i=0;i<8;i++){
        atual2 = primMesa[i];
        bold(1);
        printf("[MESA %d] : ", i);
        bold(0);
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

void moveMesaTemp(){
    tCarta *atual;
    int mesaEsc, i=0, pos=4;
    bool flag=false;

    while (i<4){
        if(dispo[i]==1){
            pos = i;
            i++;
        }else{
            i++;
        }
    }
    if(pos!=4){

        printf("\nEscolha a mesa: ");
        scanf("%d", &mesaEsc);
        if((mesaEsc<0 || mesaEsc>7) || ult[mesaEsc]==NULL)
            flag = true;
        
        while(flag){
            if((mesaEsc<0 || mesaEsc>7) || ult[mesaEsc]==NULL){
                system("cls");
                imprime();
                bold(1);
                printf("Mesa inexistente ou vazia, escolha outra mesa: ");
                bold(0);
                scanf("%d", &mesaEsc);
            }else
                flag=false;
        }

        temp[pos] = ult[mesaEsc];
        dispo[pos] = 0;

        //atualiza o ultimo da mesa
        if(primMesa[mesaEsc]->prox == NULL){
            primMesa[mesaEsc] = NULL;
            ult[mesaEsc] = NULL;
        }else{
            atual = primMesa[mesaEsc];
            while(atual->prox!=ult[mesaEsc]){
                atual = atual->prox;
            }
            ult[mesaEsc] = atual;
            ult[mesaEsc]->prox = NULL;
        }
    }else{
        system("cls");
        printf("Temporario cheio, faca outra jogada!\n");
        system("pause");
    }
}

void moveTempMesa(){
    int tempEsc, mesaEsc, op;

    printf("Escolha a posicao do temporario: ");
    scanf("%d", &tempEsc);

    if((temp[tempEsc]==NULL) || (tempEsc < 0 || tempEsc > 3)){
        system("cls");
        printf("Posicao vazia ou invalida, tente novamente.\n");
        system("pause");
        return 0;
    }
    printf("Escolha a mesa de destino: ");
    scanf("%d", &mesaEsc);
    if(mesaEsc<0 || mesaEsc>7){
        system("cls");
        printf("Posicao invalida, tente novamente.\n");
        system("pause");
        return 0;
    }

    op = movValido(temp[tempEsc], ult[mesaEsc]);
    switch(op){
        case 1:
            ult[mesaEsc] = temp[tempEsc];
            primMesa[mesaEsc] = temp[tempEsc];
            temp[tempEsc] = NULL;
            ult[mesaEsc]->prox = NULL;
            primMesa[mesaEsc]->prox = NULL;
            dispo[tempEsc] = 1;
            break;
        case 2:
            ult[mesaEsc]->prox = temp[tempEsc];
            ult[mesaEsc] = ult[mesaEsc]->prox;
            ult[mesaEsc]->prox = NULL;
            temp[tempEsc] = NULL;
            dispo[tempEsc] = 1;
            break;
        case 3:
            printf("Movimento Invalido.\n");
            system("pause");
            break;
    }
}

void moveMesaNaipe(){
    tCarta *atual;
    int mesaEsc, indice;

    printf("Escolha uma mesa: ");
    scanf("%d", &mesaEsc);
    if(mesaEsc<0 || mesaEsc>7 || ult[mesaEsc]==NULL){
        system("cls");
        printf("Posicao invalida ou vazia, tente novamente.\n");
        system("pause");
        return 0;
    }
    indice = ult[mesaEsc]->naipe - 3;

    if(primNaipe[indice]==NULL){
        atual = primNaipe[indice];
    }else{
        atual = primNaipe[indice];
        while(atual->prox!=NULL){
            atual = atual->prox;
        }     
    }

    if(naipeValido(ult[mesaEsc], atual)){
        if(atual==NULL){
            primNaipe[indice] = ult[mesaEsc];
            primNaipe[indice]->prox = NULL;
        }else{
            atual->prox = ult[mesaEsc];
            atual = atual->prox;
            atual->prox = NULL;
        }

        //atualiza o ultimo da mesa
        if(primMesa[mesaEsc]->prox == NULL){
            primMesa[mesaEsc] = NULL;
            ult[mesaEsc] = NULL;
        }else{
            atual = primMesa[mesaEsc];
            while(atual->prox!=ult[mesaEsc]){
                atual = atual->prox;
            }
            ult[mesaEsc] = atual;
            ult[mesaEsc]->prox = NULL;
        }
    }else{
        system("cls");
        printf("Movimento invalido, tente novamente.\n");
        system("pause");
    }
}

int main(){
    int op=0;
    srand(time(NULL));

    generateRandom(random, 52);
    gerarBaralho();
    embaralhaBaralho();
    distribuiMesa();

    while(op!=4){
        imprime();
        printf("(1)Mesa-Temporario\n");
        printf("(2)Temporario-Mesa\n");
        printf("(3)Mesa-Naipe\n");
        printf("(4)Sair\n");
        bold(1);
        printf("Escolha uma opcao de jogada: ");
        bold(0);
        scanf("%d", &op);
        switch (op){
        case 1:
            moveMesaTemp();
            system("cls");
            break;
        case 2:
            moveTempMesa();
            system("cls");
            break;
        case 3:
            moveMesaNaipe();
            system("cls");
            break;
        case 4:
            system("cls");
            printf("Jogo finalizado pelo usuario.\n");
            break;
        default:
            system("cls");
            printf("Opcao invalida!\n");
            system("pause");
            break;
        }
    }
}