#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "diff2d.h"
#include <time.h>

//----------------------------------------------------------------// medidores globais de tempo
clock_t start, end, dcostart,dcoend;
double cpu_time_used, dcotime;
//----------------------------------------------------------------//

void performance(){
    dcotime = ((double) (dcoend - dcostart)) / CLOCKS_PER_SEC;
    

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
}
/*

    
    start = clock();
    // Chame a função que você deseja medir aqui
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tempo de execucao: %f segundos\n", cpu_time_used);


/*--------------------------------------------------------------------------*/


float dco (float v,         /* value at one point */
           float w,         /* value at the other point */
           float lambda)    /* contrast parameter */

/* diffusivity */

{
    float result = 0.0, temp_result = 0.0;

    temp_result = (float)fabs(v-w);
    temp_result = pow(temp_result,0.2);
    temp_result = temp_result/lambda;
    if(temp_result != 0.0){
        temp_result = -(temp_result/5.0);
    }
    result = exp(temp_result);
    //result = exp( - (pow(fabs(v-w),0.2)/lambda) / 5.0);

    return (result);
}


/*--------------------------------------------------------------------------*/


void diff2d

     (//float    ht,        /* time step size, >0, e.g. 0.5 */
      //float    lambda,    /* contrast parameter */
      long     nx,        /* image dimension in x direction */
      long     ny,        /* image dimension in y direction */
      float    **f,
      float    lut[])       /* input: original image ;  output: smoothed */


/*--------------------------------------------------------------------------*/
/*                                                                          */
/*             NONLINEAR TWO DIMENSIONAL DIFFUSION FILTERING                */
/*                                                                          */
/*                       (Joachim Weickert, 7/1994)                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/


/* Explicit scheme with 9-point stencil and exponential stabilization.      */
/* Conservative, conditionally consistent to the discrete integration       */
/* model, unconditionally stable, preserves maximum-minimum principle.      */


{

long    i, j;                                     /* loop variables */
float   qC, qN, qNE, qE, qSE, qS, qSW, qW, qNW;   /* weights */
float   **g;                                      /* work copy of f */

start = clock();//-------------------inicia o tempo

/* ---- allocate storage for g ---- */

g = (float **) malloc ((nx+2) * sizeof(float *));
if (g == NULL)
   {
     printf("not enough storage available\n");
     exit(1);
   }
for (i=0; i<=nx+1; i++)
    {
      g[i] = (float *) malloc ((ny+2) * sizeof(float));
      if (g[i] == NULL)
         {
           printf("not enough storage available\n");
           exit(1);
         }
    }


/* ---- copy f into g ---- */

for (i=1; i<=nx; i++)
 for (j=1; j<=ny; j++)
     g[i][j] = f[i-1][j-1];


/* ---- create dummy boundaries ---- */

for (i=1; i<=nx; i++)
    {
     g[i][0]    = g[i][1];
     g[i][ny+1] = g[i][ny];
    }

for (j=0; j<=ny+1; j++)
    {
     g[0][j]    = g[1][j];
     g[nx+1][j] = g[nx][j];
    }


/* ---- diffusive averaging ---- */
dcostart =clock();


for (i=1; i<=nx; i++)
 for (j=1; j<=ny; j++)

     {

       /* calculate weights */



       qN  = lut[(int)fabs(g[i][j]- g[i  ][j+1])];
       qNE = lut[(int)fabs(g[i][j]- g[i+1][j+1])];
       qE  = lut[(int)fabs(g[i][j]- g[i+1][j  ])];
       qSE = lut[(int)fabs(g[i][j]- g[i+1][j-1])];
       qS  = lut[(int)fabs(g[i][j]- g[i  ][j-1])];
       qSW = lut[(int)fabs(g[i][j]- g[i-1][j-1])];
       qW  = lut[(int)fabs(g[i][j]- g[i-1][j  ])];
       qNW = lut[(int)fabs(g[i][j]- g[i-1][j+1])];
       qC  = 1.0 - qN - qNE - qE - qSE - qS - qSW - qW - qNW;


       /* weighted averaging */

       f[i-1][j-1] = qNW * g[i-1][j+1] + qN * g[i][j+1] + qNE * g[i+1][j+1] +
                     qW  * g[i-1][j  ] + qC * g[i][j  ] + qE  * g[i+1][j  ] +
                     qSW * g[i-1][j-1] + qS * g[i][j-1] + qSE * g[i+1][j-1];

     }  /* for */


/* ---- disallocate storage for g ---- */
dcoend=clock();

for (i=0; i<=nx+1; i++)
    free(g[i]);
free(g);

end = clock();
performance();


return;

} /* diff */


/*--------------------------------------------------------------------------*/

