#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
pthread_mutex_t mutex_abs = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_arbeg = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cinto = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_lvt= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_motor = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_temperature = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_display = PTHREAD_COND_INITIALIZER;
//Injecao do motor
int ACCELERATE = 0; 
//ABS
int FREIO = 0; 
//Temperatura do motor
int TEMPERATURE = 50;
int QUENTE = 0;
//Equipamentos de suporte a vida
int BATIDA = 0; //arbeg
int CINTO = 0; //cinto
//LVT
int VIDROS = 0;
int LUZ = 0;
int TRNCA = 0;

struct timespec ACCELERATE_CLOCK_END, ENGINE_TEMPERATURE_CLOCK_END, 
clock_accelerate_start, clock_temperature_start,
ABS_CLOCK_END ,  ABS_CLOCK_START,//ABS
ARBEG_CLOCK_END, ARBEG_CLOCK_START,//arbeg
CINTO_CLOCK_END, CINTO_CLOCK_START,//cinto
LVT_CLOCK_START, // tranca, luz, vidro
TRANCA_CLOCK_END, //tranca do carro 
LUZ_CLOCK_END,//Luz dianteira
VIDRO_CLOCK_END//vidro, luz, tranca
;
void *thread_arbeg(void *arg) {
    while (1) {
        usleep(11);
        if (BATIDA == 1) {
            
            clock_gettime(CLOCK_MONOTONIC, &ARBEG_CLOCK_START);
            usleep(1);
            usleep(15);
            clock_gettime(CLOCK_MONOTONIC, &ARBEG_CLOCK_END);
        }
        
        pthread_cond_signal(&cond_display);
        
    }
    return NULL;
}
void *thread_cinto(void *arg) {
    while (1) {
        usleep(11);
        if (CINTO == 1) {
            
            clock_gettime(CLOCK_MONOTONIC, &CINTO_CLOCK_START);
            usleep(1);
            usleep(15);
            clock_gettime(CLOCK_MONOTONIC, &CINTO_CLOCK_END);
        }
        
        pthread_cond_signal(&cond_display);
        
    }
    return NULL;
}
void *thread_ltv(void *arg) {
    while (1) {
        usleep(11);
        clock_gettime(CLOCK_MONOTONIC, &LVT_CLOCK_START);
        
        if (LUZ == 1) {
            usleep(1);
            usleep(15);
            clock_gettime(CLOCK_MONOTONIC, &LUZ_CLOCK_END);
            
        }
        
        if (VIDROS){
            
            usleep(1);
            usleep(15);
            clock_gettime(CLOCK_MONOTONIC, &VIDRO_CLOCK_END);
        }
        
        if (TRNCA == 1) {
            
            usleep(1);
            usleep(15);
            clock_gettime(CLOCK_MONOTONIC, &TRANCA_CLOCK_END);
        }
  
        pthread_cond_signal(&cond_display); 
        
    }
    return NULL;
}
void *thread_abs(void *arg) {
    while (1) {
        usleep(11);
        if (FREIO == 1) {
            
            clock_gettime(CLOCK_MONOTONIC, &ABS_CLOCK_START);
            usleep(1);
            usleep(15);
            clock_gettime(CLOCK_MONOTONIC, &ABS_CLOCK_END);
        }
        
        pthread_cond_signal(&cond_display);
        
    }
    return NULL;
}
void *thread_motor(void *arg) {
    while (1) {
        usleep(11);
        if (ACCELERATE == 1) {
            
            clock_gettime(CLOCK_MONOTONIC, &clock_accelerate_start);
            usleep(1);
            usleep(15);
            clock_gettime(CLOCK_MONOTONIC, &ACCELERATE_CLOCK_END);
        }
        
        pthread_cond_signal(&cond_display);
       
    }
    return NULL;
}
void *thread_temperature(void *arg) {
    while (1) {
        usleep(11);
        
        if (TEMPERATURE > 70) {
            clock_gettime(CLOCK_MONOTONIC, &clock_temperature_start);
            QUENTE = 1;
            usleep(1000);
            usleep(15);
            clock_gettime(CLOCK_MONOTONIC, &ENGINE_TEMPERATURE_CLOCK_END);
        } else {
            QUENTE = 0;
        }
        
        
        
        // Sinaliza a thread de exibição para atualizar a tela
        pthread_cond_signal(&cond_display);

        // Aguarde um curto intervalo antes de verificar novamente
        // 10 microssegundos
    }
    return NULL;
}

void *display(void *arg) {
    long max_inj = 0, max_temp = 0, max_abs = 0 ,max_arbeg = 0, max_cinto = 0 , max_luz = 0, max_vidro = 0,max_tranca = 0  ;
    while (1) {
        usleep(100000);
        //pthread_cond_wait(&cond_display, &mutex_motor);

        // Calcula o tempo de resposta em microssegundos para ACCELERATE e TEMPERATURE
        //tranca
        long tranca_response_time_us =
        (TRANCA_CLOCK_END.tv_nsec - LVT_CLOCK_START.tv_nsec) ;
        if(tranca_response_time_us > max_tranca){
            max_tranca = tranca_response_time_us;
        }
        //vidro 
        long vidro_response_time_us =
        (VIDRO_CLOCK_END.tv_nsec - LVT_CLOCK_START.tv_nsec) ;
        if(vidro_response_time_us > max_vidro){
            max_vidro = vidro_response_time_us;
        }
        //LUZ frontal
        long luz_response_time_us =
        (LUZ_CLOCK_END.tv_nsec - LVT_CLOCK_START.tv_nsec) ;
        if(luz_response_time_us > max_luz){
            max_luz = luz_response_time_us;
        }
        //Cinto de seguranca
        long cinto_response_time_us =
        (CINTO_CLOCK_END.tv_nsec - CINTO_CLOCK_START.tv_nsec) ;
        if(cinto_response_time_us > max_cinto){
            max_cinto = cinto_response_time_us;
        }
        //batida - arbeag
        long arbeg_response_time_us =
        (ARBEG_CLOCK_END.tv_nsec - ARBEG_CLOCK_START.tv_nsec) ;
        if(arbeg_response_time_us > max_arbeg){
            max_arbeg = arbeg_response_time_us;
        }
        //ABS - Freio 
        long abs_response_time_us =
        (ABS_CLOCK_END.tv_nsec - ABS_CLOCK_START.tv_nsec) ;
        if(abs_response_time_us > max_abs){
            max_abs = abs_response_time_us;
        }

        long accelerate_response_time_us =
        (ACCELERATE_CLOCK_END.tv_nsec - clock_accelerate_start.tv_nsec) ;
        if(accelerate_response_time_us > max_inj){
            max_inj = accelerate_response_time_us;
        }
        
        long  temperature_response_time_us =
      
            (ENGINE_TEMPERATURE_CLOCK_END.tv_nsec - clock_temperature_start.tv_nsec) ;
        if(temperature_response_time_us > max_temp){
            max_temp = temperature_response_time_us;
        }
        // Limpa a tela e imprime os valores
        printf("\033[H\033[J");
        printf("Acelerador: %d - Tempo de resposta: %ld us\n", ACCELERATE, max_inj/1000);
        printf("Temperatura: %d - Tempo de resposta: %ld us\n", QUENTE, max_temp/1000);
        printf("Freio: %d - Tempo de resposta: %ld us\n", FREIO, max_abs/1000);
        printf("Bateu: %d - Tempo de resposta: %ld us\n", BATIDA, max_arbeg/1000);
        printf("Cinto colocado: %d - Tempo de resposta: %ld us\n", CINTO, max_cinto/1000);
        printf("Luz: %d - Tempo de resposta: %ld us\n", LUZ, max_luz/1000);
        printf("Tranca: %d - Tempo de resposta: %ld us\n", TRNCA, max_tranca/1000);
        printf("Vidro: %d - Tempo de resposta: %ld us\n", VIDROS, max_vidro/1000);
    }
    return NULL;
}
int main(int argc, char *argv[]) {
    pthread_attr_t main_attr;
    pthread_attr_init(&main_attr);
    struct sched_param main_param;
    main_param.sched_priority = 99; // Prioridade máxima
    pthread_attr_setschedparam(&main_attr, &main_param);
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &main_param);

    pthread_t t1, t2, tdisplay,tabs, tarbeg, tcinto,tltv ;

    pthread_attr_t motor_attr;
    pthread_attr_init(&motor_attr);
    struct sched_param motor_param;
    motor_param.sched_priority = 70; // Prioridade de funcao
    pthread_attr_setschedparam(&motor_attr, &motor_param);

    pthread_create(&t1, &motor_attr, thread_motor, NULL);

    pthread_attr_t default_top;
    pthread_attr_init(&default_top);
    struct sched_param top_param;
    top_param.sched_priority = 50; // Prioridade risco
    pthread_attr_setschedparam(&default_top, &top_param);

    pthread_create(&t2, &default_top, thread_temperature, NULL);
    pthread_create(&tabs, &default_top, thread_abs, NULL);
    pthread_create(&tarbeg, &default_top, thread_arbeg, NULL);


    pthread_attr_t default_attr;
    pthread_attr_init(&default_attr);
    struct sched_param default_param;
    default_param.sched_priority = 1; // Prioridade padrão
    pthread_attr_setschedparam(&default_attr, &default_param);

    pthread_create(&tdisplay, &default_attr, display, NULL);
    pthread_create(&tcinto, &default_attr, thread_cinto, NULL);
    pthread_create(&tltv, &default_attr, thread_ltv, NULL);

<<<<<<< HEAD
=======

>>>>>>> e9e5a44 (arrumado erro da tranca)

    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char line = ' ';
    while (line != 'q') {
        line = getchar();
        if (line == 'a') {
            pthread_mutex_lock(&mutex_motor);
            ACCELERATE = !ACCELERATE;
            pthread_mutex_unlock(&mutex_motor);
        }else if (line == 't') {
            pthread_mutex_lock(&mutex_temperature);
            if (TEMPERATURE == 100){
                TEMPERATURE = 50;
            }else{
                TEMPERATURE = 100;
            }
            // Sinaliza a thread de temperatura para verificar imediatamente
            pthread_cond_signal(&cond_display);
            pthread_mutex_unlock(&mutex_temperature);
        }else if(line == 'f'){
            pthread_mutex_lock(&mutex_abs);
            FREIO = !FREIO;
            pthread_mutex_unlock(&mutex_abs);
        }else if(line == 'b'){
            pthread_mutex_lock(&mutex_arbeg);
            BATIDA = !BATIDA;
            pthread_mutex_unlock(&mutex_arbeg);
        }else if(line == 'c'){
            pthread_mutex_lock(&mutex_cinto);
            CINTO = !CINTO;
            pthread_mutex_unlock(&mutex_cinto);
        }else if(line == 'j'){
            pthread_mutex_lock(&mutex_lvt);
            VIDROS = !VIDROS;
            pthread_mutex_unlock(&mutex_lvt);
        }else if(line == 'g'){
            pthread_mutex_lock(&mutex_lvt);
            TRNCA = !TRNCA;
            pthread_mutex_unlock(&mutex_lvt);
        }else if(line == 'l'){
            pthread_mutex_lock(&mutex_lvt);
            LUZ = !LUZ;
            pthread_mutex_unlock(&mutex_lvt);
        }
        
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    pthread_cancel(t1);
    pthread_cancel(t2);
    pthread_cancel(tdisplay);
    return 0;
}
