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
int qMesa[8] = {7, 7, 7, 7, 6, 6, 6, 6};

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

bool blocoValido(tCarta *mesaOri, tCarta *mesaDest){
    tCarta *atual = mesaOri;
    tCarta *aux = mesaOri;

    // sequencia do bloco valida
    while(atual->prox!=NULL){
        if(((atual->naipe<=4 && atual->prox->naipe>=5) || (atual->naipe>=5 && atual->prox->naipe<=4)) && (atual->prox->numero==((atual->numero)-1))){
            atual = atual->prox;
        }else{
            return false;
        }
    }
    
    // compara com a mesa de destino
    if(mesaDest==NULL || ((mesaDest->numero == ((aux->numero)+1)) && ((aux->naipe<=4 && mesaDest->naipe>=5) || (aux->naipe>=5 && mesaDest->naipe<=4)))){
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
        if((mesaEsc<0 || mesaEsc>7) || ult[mesaEsc]==NULL){
            system("cls");
            bold(1);
            printf("Mesa inexistente ou vazia, escolha outra mesa!\n");
            bold(0);
            system("pause");
            return 0;
        }

        temp[pos] = ult[mesaEsc];
        dispo[pos] = 0;

        //atualiza o ultimo da mesa
        if(primMesa[mesaEsc]->prox == NULL){
            primMesa[mesaEsc] = NULL;
            ult[mesaEsc] = NULL;
        }else{
            atual = primMesa[mesaEsc];
            while(atual->prox!=ult[mesaEsc])
                atual = atual->prox;
            
            ult[mesaEsc] = atual;
            ult[mesaEsc]->prox = NULL;
        }
        qMesa[mesaEsc]--;
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
        case 1: // mesa vazia
            ult[mesaEsc] = temp[tempEsc];
            primMesa[mesaEsc] = temp[tempEsc];
            temp[tempEsc] = NULL;
            ult[mesaEsc]->prox = NULL;
            primMesa[mesaEsc]->prox = NULL;
            dispo[tempEsc] = 1;
            qMesa[mesaEsc]++;
            break;
        case 2: // pelo numero
            ult[mesaEsc]->prox = temp[tempEsc];
            ult[mesaEsc] = ult[mesaEsc]->prox;
            ult[mesaEsc]->prox = NULL;
            temp[tempEsc] = NULL;
            dispo[tempEsc] = 1;
            qMesa[mesaEsc]++;
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
        qMesa[mesaEsc]--;
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

void moveTempNaipe() {
    int tempEsc, op, indice;
    tCarta *atual;

    printf("Escolha a posicao do temporario: ");
    scanf("%d", &tempEsc);

    if((temp[tempEsc]==NULL) || (tempEsc < 0 || tempEsc > 3)){
        system("cls");
        printf("Posicao vazia ou invalida, tente novamente.\n");
        system("pause");
        return 0;
    }

    indice = temp[tempEsc]->naipe - 3;

    if(primNaipe[indice]==NULL){
        atual = primNaipe[indice];
    }else{
        atual = primNaipe[indice];
        while(atual->prox!=NULL){
            atual = atual->prox;
        }     
    }

    if(naipeValido(temp[tempEsc], atual)){
        if(atual==NULL){
            primNaipe[indice] = temp[tempEsc];
            primNaipe[indice]->prox = NULL;
            temp[tempEsc] = NULL;
            dispo[tempEsc] = 1;
        }else{
            atual->prox = temp[tempEsc];
            atual = atual->prox;
            atual->prox = NULL;
            temp[tempEsc] = NULL;
            dispo[tempEsc] = 1;
        }
    }else{
        system("cls");
        printf("Movimento invalido, tente novamente.\n");
        system("pause");
    }
    
}

void moveNaipeTemp() {
    int naipeEsc, i=0, pos=4;
    tCarta *atual, *aux;

    printf("Escolha um naipe: ");
    scanf("%d", &naipeEsc);
    if(naipeEsc<0 || naipeEsc>3 || primNaipe[naipeEsc]==NULL){
        system("cls");
        printf("Posicao invalida ou vazia, tente novamente.\n");
        system("pause");
        return 0;
    }

    atual = primNaipe[naipeEsc];
    aux = atual;
    while(atual->prox!=NULL){
        aux = atual;
        atual = atual->prox;
    }

    if(atual != NULL){
        while (i<4){
            if(dispo[i]==1){
                pos = i;
                i++;
            }else{
                i++;
            }
        }
        if(pos!=4){
            temp[pos] = atual;
            dispo[pos] = 0;
            if(primNaipe[naipeEsc]->prox==NULL)
                primNaipe[naipeEsc] = NULL;
            else if(atual->prox==NULL)
                aux->prox = NULL;
        }else{
            system("cls");
            printf("Temporario cheio, faca outra jogada!\n");
            system("pause");
        }
    }else{
        system("cls");
        printf("Naipe vazio, faca outra jogada!\n");
        system("pause");
    }
}

void moveNaipeMesa() {
    int naipeEsc, mesaEsc;
    tCarta *atual, *aux;

    printf("Escolha um naipe: ");
    scanf("%d", &naipeEsc);
    if(naipeEsc<0 || naipeEsc>3 || primNaipe[naipeEsc]==NULL){
        system("cls");
        printf("Posicao invalida ou vazia, tente novamente.\n");
        system("pause");
        return 0;
    }

    printf("Escolha uma mesa: ");
    scanf("%d", &mesaEsc);
    if(mesaEsc<0 || mesaEsc>7 || ult[mesaEsc]==NULL){
        system("cls");
        printf("Posicao invalida ou vazia, tente novamente.\n");
        system("pause");
        return 0;
    }

    atual = primNaipe[naipeEsc];
    aux = atual;
    while(atual->prox!=NULL){
        aux = atual;
        atual = atual->prox;
    }

    int op = movValido(atual, ult[mesaEsc]);
    switch(op){
        case 1:
            ult[mesaEsc] = atual;
            primMesa[mesaEsc] = atual;
            if(primNaipe[naipeEsc]->prox==NULL)
                primNaipe[naipeEsc] = NULL;
            else if(atual->prox==NULL)
                aux->prox = NULL;
            ult[mesaEsc]->prox = NULL;
            primMesa[mesaEsc]->prox = NULL;
            qMesa[mesaEsc]++;
            break;
        case 2:
            ult[mesaEsc]->prox = atual;
            ult[mesaEsc] = ult[mesaEsc]->prox;
            ult[mesaEsc]->prox = NULL;
            if(primNaipe[naipeEsc]->prox==NULL)
                primNaipe[naipeEsc] = NULL;
            else if(atual->prox==NULL)
                aux->prox = NULL;
            qMesa[mesaEsc]++;
            break;
        case 3:
            printf("Movimento Invalido.\n");
            system("pause");
            break;
    }

}

void moveMesaMesa() {
    int qCartas, mesaOrigem, mesaDest;
    tCarta *atual, *ant;
    bool flag = false;

    printf("Quantas cartas deseja mover? ");
    scanf("%d", &qCartas);
    printf("Mesa de origem: ");
    scanf("%d", &mesaOrigem);
    if(mesaOrigem<0 || mesaOrigem>7){
        system("cls");
        printf("Mesa Invalida, escolha de 0 a 7!\n");
        system("pause");
        return 0;
    }else if(primMesa[mesaOrigem]==NULL){
        system("cls");
        printf("Mesa vazia, jogue novamente!\n");
        system("pause");
        return 0;
    }else if (qCartas>qMesa[mesaOrigem]){
        system("cls");
        printf("Bloco de cartas maior que a quantidade de cartas na mesa, jogue novamente!\n");
        system("pause");
        return 0;
    }
    printf("Mesa de destino: ");
    scanf("%d", &mesaDest);
    if(mesaDest<0 || mesaDest>7){
        system("cls");
        printf("Mesa Invalida, escolha de 0 a 7!\n");
        system("pause");
        return 0;
    }

    atual = primMesa[mesaOrigem];
    ant = atual;
    for(int i=0;i<(qMesa[mesaOrigem]-qCartas);i++){
        ant = atual;
        atual = atual->prox;
        flag = true;
    }
    
    if(blocoValido(atual, ult[mesaDest])){
        if(ult[mesaDest]==NULL){
            primMesa[mesaDest] = atual;
            if(primMesa[mesaOrigem]->prox==NULL)
                primMesa[mesaOrigem] = ult[mesaOrigem] = NULL;
            else if(flag){
                ant->prox = NULL;
                ult[mesaOrigem] = ant;       
            }else
                primMesa[mesaOrigem] = ult[mesaOrigem] = NULL;
        }else{
            ult[mesaDest]->prox = atual;
            if(primMesa[mesaOrigem]->prox==NULL)
                primMesa[mesaOrigem] = ult[mesaOrigem] = NULL;
            else if(flag){
                ant->prox = NULL;
                ult[mesaOrigem] = ant;
            }else
                primMesa[mesaOrigem] = ult[mesaOrigem] = NULL;
        }

        //atualiza o ultimo da mesa destino
        atual = primMesa[mesaDest];
        while(atual->prox!=NULL){
            atual = atual->prox;
        }
        ult[mesaDest] = atual;
        ult[mesaDest]->prox = NULL;

        qMesa[mesaOrigem]-=qCartas;
        qMesa[mesaDest]+=qCartas;

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

    while(op!=8){
        imprime();
        printf("(1)Mesa-Temporario\n");
        printf("(2)Temporario-Mesa\n");
        printf("(3)Mesa-Naipe\n");
        printf("(4)Temporario-Naipe\n");
        printf("(5)Naipe-Temporario\n");
        printf("(6)Naipe-Mesa\n");
        printf("(7)Mesa-Mesa\n");
        printf("(8)Sair\n");
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
            moveTempNaipe();
            system("cls");
            break;
        case 5:
            moveNaipeTemp();
            system("cls");
            break;
        case 6:
            moveNaipeMesa();
            system("cls");
            break;
        case 7:
            moveMesaMesa();
            system("cls");
            break;
        case 8:
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