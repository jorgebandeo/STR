#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int valor_total = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t signal_motor = PTHREAD_COND_INITIALIZER;
pthread_cond_t signal_freio = PTHREAD_COND_INITIALIZER;

void thread_motor(void) {
    // logica do motor
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&signal_motor, &mutex);
}

void thread_frenagem(void) {
 // logica do freio
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&signal_motor, &mutex); 
}

void thread_ESV(void) {
 // logica do equipamentos de suporte a vida
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&signal_motor, &mutex);

}

void thread_LVT(void) {
 // logica do LVT 
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&signal_motor, &mutex);

}

void thread_display(void) {
 // logica do freio  
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&signal_motor, &mutex);

}

void thread_input(void) {
 // logica do freio  
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&signal_motor, &mutex);

}

int main(int argc, char *argv[]) {
    pthread_t t1, t2, t3, t4, t5, t6;

    // Criação das threads
    pthread_create(&t1, NULL, thread_motor, NULL);
    pthread_create(&t2, NULL, thread_frenagem, NULL);
    pthread_create(&t3, NULL, thread_ESV, NULL);
    pthread_create(&t4, NULL, thread_LVT, NULL);
    pthread_create(&t5, NULL, thread_display, NULL);
    pthread_create(&t6, NULL, thread_input, NULL);
    

    while(1) {
        char *line = getchar();
        if(strcmp(&line, "a") == 0){
            pthread_cond_signal(&signal_motor);
        }

        if(strcmp(&line, "f") == 0){
            pthread_cond_signal(&signal_freio);
        }
    }

    // Adicione lógica para encerrar as threads se necessário

    return 0;
}
