#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#ifdef _WIN32 || _WIN64
#include <Windows.h>
#else
#include <unistd.h>
#endif

pthread_mutex_t mutex_motor = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_temperature = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_overheating = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t signal_motor = PTHREAD_COND_INITIALIZER;
pthread_cond_t signal_freio = PTHREAD_COND_INITIALIZER;

int ACCELERATE = 0;
int TEMPERATURE = 50;
int OVERHEATING = 1;

clock_t ACCELERATE_CLOCK_END = 0, ENGINE_TEMPERATURE_CLOCK_END = 0;

void accelerate()
{
    ACCELERATE_CLOCK_END = clock();
    return;
}

void monitor_engine_temperature()
{
    while (1)
    {
        pthread_mutex_lock(&mutex_temperature);
        if (TEMPERATURE > 99)
        {
            ENGINE_TEMPERATURE_CLOCK_END = clock();
            pthread_mutex_lock(&mutex_overheating);
            OVERHEATING = 1;
            pthread_mutex_unlock(&mutex_overheating);
        }
        else
        {
            pthread_mutex_lock(&mutex_overheating);
            OVERHEATING = 0;
            pthread_mutex_unlock(&mutex_overheating);
        }
        pthread_mutex_unlock(&mutex_temperature);
    }
}

void thread_motor(void)
{
    pthread_t temperature_sensor;
    pthread_create(&temperature_sensor, NULL, monitor_engine_temperature, NULL);

    while (1)
    {
        if (ACCELERATE == 1)
        {
            accelerate();
        }
    }
}

void thread_frenagem(void)
{
    // logica do freio
    pthread_mutex_lock(&mutex_motor);
    pthread_cond_wait(&signal_motor, &mutex_motor);
}

void thread_ESV(void)
{
    // logica do equipamentos de suporte a vida
    pthread_mutex_lock(&mutex_motor);
    pthread_cond_wait(&signal_motor, &mutex_motor);
}

void thread_LVT(void)
{
    // logica do LVT
    pthread_mutex_lock(&mutex_motor);
    pthread_cond_wait(&signal_motor, &mutex_motor);
}

void thread_display(void)
{
    // logica do freio
    pthread_mutex_lock(&mutex_motor);
    pthread_cond_wait(&signal_motor, &mutex_motor);
}

void thread_input(void)
{
    // logica do freio
    pthread_mutex_lock(&mutex_motor);
    pthread_cond_wait(&signal_motor, &mutex_motor);
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2, t3, t4, t5, t6;
    pthread_t thread_temperature_variation;
    pthread_create(&t1, NULL, thread_motor, NULL);

    clock_t clock_accelereate_start, clock_temperature_start;

    // Criação das threads

    // pthread_create(&t2, NULL, thread_frenagem, NULL);
    // pthread_create(&t3, NULL, thread_ESV, NULL);
    // pthread_create(&t4, NULL, thread_LVT, NULL);
    // pthread_create(&t5, NULL, thread_display, NULL);
    // pthread_create(&t6, NULL, thread_input, NULL);

    while (1)
    {
        char *line = getchar();

        if (strcmp(&line, "a") == 0)
        {
            printf("Acelerando!!\n");
            pthread_mutex_lock(&mutex_motor);
            ACCELERATE = 1;
            clock_accelereate_start = clock();
            ACCELERATE = 0;
            pthread_mutex_unlock(&mutex_motor);
            printf("Pronto\n");
            printf("Tempo de resposta: %f \n", (double)(ACCELERATE_CLOCK_END - clock_accelereate_start) / CLOCKS_PER_SEC);
        }
        if (strcmp(&line, "t") == 0)
        {
            pthread_mutex_lock(&mutex_temperature);
            TEMPERATURE = 100;
            clock_temperature_start = clock();
            pthread_mutex_unlock(&mutex_temperature);
            printf("Tempo de resposta: %f \n", (double)(ENGINE_TEMPERATURE_CLOCK_END - clock_temperature_start) / CLOCKS_PER_SEC);
        }
        if (strcmp(&line, "f") == 0)
        {
            pthread_cond_signal(&signal_freio);
        }
    }

    return 0;
}
