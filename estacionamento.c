#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define N_VAGAS 5
#define N_CLIENTES 20

pthread_t painel;
pthread_t clientes[N_CLIENTES];
sem_t vagas[N_VAGAS];

sem_t sArrecadar[N_VAGAS];
sem_t sTotal;

int arrecadar[N_VAGAS];
int total = 0;
int taxa = 2;

void * vaga1(int * cliente){
    sem_wait(&vagas[0]);
    int tempo = rand() % 10;
    printf("Vaga 1, Cliente: %d \n", cliente);

    sem_wait(&sArrecadar[0]);
    arrecadar[0] = arrecadar[0] + taxa * tempo;

    sem_wait(&sTotal);
    total = total  + taxa * tempo;
    sem_post(&sTotal);
    
    sleep(tempo);
    sem_post(&sArrecadar[0]);
    sem_post(&vagas[0]);
}

void * vaga2(int * cliente){
    sem_wait(&vagas[1]);
    int tempo = rand() % 10;
    printf("Vaga 2, Cliente: %d \n", cliente);

    sem_wait(&sArrecadar[1]);
    arrecadar[1] = arrecadar[1] + taxa * tempo;

    sem_wait(&sTotal);
    total = total  + taxa * tempo;
    sem_post(&sTotal);

    sleep(tempo);
    sem_post(&sArrecadar[1]);
    sem_post(&vagas[1]);
}

void * vaga3(int * cliente){
    sem_wait(&vagas[2]);
    int tempo = rand() % 10;
    printf("Vaga 3, Cliente: %d \n", cliente);

    sem_wait(&sArrecadar[2]);
    arrecadar[2] = arrecadar[2] + taxa * tempo;

    sem_wait(&sTotal);
    total = total  + taxa * tempo;
    sem_post(&sTotal);

    sleep(tempo);
    sem_post(&sArrecadar[2]);
    sem_post(&vagas[2]);
}

void * vaga4(int * cliente){
    sem_wait(&vagas[3]);
    int tempo = rand() % 10;
    printf("Vaga 4, Cliente: %d \n", cliente);

    sem_wait(&sArrecadar[3]);
    arrecadar[3] = arrecadar[3] + taxa * tempo;

    sem_wait(&sTotal);
    total = total  + taxa * tempo;
    sem_post(&sTotal);

    sleep(tempo);
    sem_post(&sArrecadar[3]);
    sem_post(&vagas[3]);
}

void * vaga5(int * cliente){
    sem_wait(&vagas[4]);
    int tempo = rand() % 10;
    printf("Vaga 5, Cliente: %d \n", cliente);

    sem_wait(&sArrecadar[4]);
    arrecadar[4] = arrecadar[4] + taxa * tempo;
    
    sem_wait(&sTotal);
    total = total  + taxa * tempo;
    sem_post(&sTotal);
    
    sleep(tempo);
    sem_post(&sArrecadar[4]);
    sem_post(&vagas[4]);
}

void * controlador(int * c){ // procura vaga livre e atribui ao cliente
    int cliente = c;
    int value; // armazena o valor do semaforo

    printf("Cliente %d, chando vaga\n", cliente);
    int i;
    for (i = 0; i < N_VAGAS; i++){
        sem_getvalue(&vagas[i], &value);

        if(value == 1){ // encontra um semaforo livre

            switch (i){ // atribui o clienta a vaga de acordo com o semaforo
                case 0:
                    pthread_create(&clientes[cliente], NULL, vaga1, (void *) (cliente));
                    break;
                case 1:
                    pthread_create(&clientes[cliente], NULL, vaga2, (void *) (cliente));
                    break;
                case 2:
                    pthread_create(&clientes[cliente], NULL, vaga3, (void *) (cliente));
                    break;
                case 3:
                    pthread_create(&clientes[cliente], NULL, vaga4, (void *) (cliente));
                    break;
                case 4:
                    pthread_create(&clientes[cliente], NULL, vaga5, (void *) (cliente));
                    break;
                
                default:
                    break;
            }

            pthread_join(clientes[cliente],NULL);
            break; // para o for depois de achar uma vaga livre
        }

    }

    pthread_exit(NULL);
}

void printPainel(){
    while(1){
        // system("clear");

        sem_wait(&sArrecadar[0]);
        printf("Arrecadador vaga 1: %d \n", arrecadar[0]);
        sem_post(&sArrecadar[0]);

        sem_wait(&sArrecadar[1]);
        printf("Arrecadador vaga 2: %d \n", arrecadar[1]);
        sem_post(&sArrecadar[1]);

        sem_wait(&sArrecadar[2]);
        printf("Arrecadador vaga 3: %d \n", arrecadar[2]);
        sem_post(&sArrecadar[2]);

        sem_wait(&sArrecadar[3]);
        printf("Arrecadador vaga 4: %d \n", arrecadar[3]);
        sem_post(&sArrecadar[3]);

        sem_wait(&sArrecadar[4]);
        printf("Arrecadador vaga 5: %d \n", arrecadar[4]);
        sem_post(&sArrecadar[4]);

        sem_wait(&sTotal);
        printf("Total: %d \n", total);
        sem_post(&sTotal);

        sleep(1);
    }
}

int main(){
    srand( (unsigned)time(NULL) ); // muda a seed para gerar aleatoriedade 
    int i;
    for ( i = 0; i < N_VAGAS; i++){ // inicializa os arrecadadores e semaforos das vagas
        arrecadar[i] = 0;
        sem_init(&vagas[i], 0, 1);
        sem_init(&sArrecadar[i], 0, 1);
    }

    sem_init(&sTotal,0,1); // inicia o semaforo do totalizador

    pthread_create(&painel, NULL, printPainel, NULL);

    for (i = 0; i < N_CLIENTES; i++){ // manda as threads de clientes para o controlador
        pthread_create(&clientes[i], NULL, controlador, (void *) (i));
    }

    for (i = 0; i < N_CLIENTES; i++){
        pthread_join(clientes[i],NULL);
    }

    pthread_join(painel,NULL);

    return 0;
}