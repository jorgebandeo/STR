#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int valor_total = 0;
pthread_mutex_t em = PTHREAD_MUTEX_INITIALIZER;

void thread_Motor(void) {
 // logica do motor
}

void thread_frenagem(void) {
 // logica do freio  
}

void thread_ESV(void) {
 // logica do equipamentos de suporte a vida

void thread_LVT(void) {
 // logica do LVT 
}

void thread_Dsplay(void) {
 // logica do freio  
}

void thread_Input(void) {
 // logica do freio  
}

void* wrapper_incrementa(void* arg) {
    while (1) {
        sleep(1);
        thread_incrementa();
    }
    return NULL;
}

void* wrapper_decrementa(void* arg) {
    while (1) {
        sleep(2);
        thread_decrementa();
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t t1, t2, t3, t4;

    // Criação das threads
    pthread_create(&t1, NULL, wrapper_incrementa, NULL);
    pthread_create(&t2, NULL, wrapper_incrementa, NULL);
    pthread_create(&t3, NULL, wrapper_decrementa, NULL);
    pthread_create(&t4, NULL, wrapper_decrementa, NULL);

    printf("Digite enter para terminar o programa:\n");
    getchar(); // Espera por um caractere do usuário

    // Adicione lógica para encerrar as threads se necessário

    return 0;
}
