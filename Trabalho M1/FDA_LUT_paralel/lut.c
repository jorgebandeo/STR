#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void crialut(float ht, float lambda, float lut[]){
    

    

    for (int i = 0; i < 256; i++) {
        
        float result = 0.0, temp_result = 0.0;

        temp_result = (float)i;
        temp_result = pow(temp_result,0.2);
        temp_result = temp_result/lambda;
        if(temp_result != 0.0){
            temp_result = -(temp_result/5.0);
        }
        result = exp(temp_result);

        result = (1.0 - exp(-8.0 * ht * result)) / 8.0; /// executa todo o calculo feito de difusão


        lut[i] = result; //armazena o pre resultado no vetor

        
    }
   /*   printf("Valores do vetor:\n");
        for (int i = 0; i < 256; i++) {
            printf("%.6f ", lut[i]);
        }*/
        

}