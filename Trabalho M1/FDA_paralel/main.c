#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pgmfiles.h"
#include "diff2d.h"
#include <time.h>

// gcc -o fda pgmtolist.c pgmfiles.c diff2d.c main.c -lm -fopenmp

void main (int argc, char **argv) {
  char   row[80];
  float  **matrix;
  int i, j;
  FILE   *inimage, *outimage;
  long   imax;
  float  lambda;
  int result;
  eightBitPGMImage *PGMImage;

  clock_t start, end, totalstart, totalend;
  double cpu_time_used, inputlag=0, read_image_name=0,ASFM=0,RIM=0,process_image=0,CTRI=0,write_image=0,disallocate_storage=0;
  
  totalstart = clock();
  /* ---- read image name  ------------------------------------------------------------ */
  PGMImage = (eightBitPGMImage *) malloc(sizeof(eightBitPGMImage));
  read_image_name = read_image_name + ((double)(clock() - totalstart))/CLOCKS_PER_SEC; 
  
  /* ---- inputlag -------------------------------------------------------------------- */
  start = clock();
  if (!argv[1])
  {
    
	  printf("name of input PGM image file (with extender): ");
    scanf("%s", PGMImage->fileName);
  }
  else
  {
    strcpy(PGMImage->fileName, argv[1]);
  }
  inputlag = inputlag + ((double)(clock() - start))/CLOCKS_PER_SEC;

  /* ---- read image name  ------------------------------------------------------------ */
  start = clock();
  result = read8bitPGM(PGMImage);
  if(result < 0) 
    {
      printPGMFileError(result);
      exit(result);
    }
  read_image_name = read_image_name + ((double)(clock() - start))/CLOCKS_PER_SEC; 
  /* ---- allocate storage for matrix(ASFM) ------------------------------------------------- */
  start = clock();
  matrix = (float **) malloc (PGMImage->x * sizeof(float *));
  if (matrix == NULL)
    { 
      printf("not enough storage available\n");
      exit(1);
    } 
  for (i=0; i<PGMImage->x; i++)
    {
      matrix[i] = (float *) malloc (PGMImage->y * sizeof(float));
      if (matrix[i] == NULL)
        { 
	  printf("not enough storage available\n");
	  exit(1);
        }
    }
  ASFM = ASFM + ((double)(clock() - start))/CLOCKS_PER_SEC; 
  /* ---- read image data into matrix(RIM) ------------------------------------------------ */
  start = clock();
 for (i=0; i<PGMImage->x; i++)
    for (j=0; j<PGMImage->y; j++)
      matrix[i][j] = (float) *(PGMImage->imageData + (i*PGMImage->y) + j); 
  RIM = RIM + ((double)(clock() - start))/CLOCKS_PER_SEC; 
  /* ---- inputlag -------------------------------------------------------------------- */
  start = clock();
  printf("contrast paramter lambda (>0) : ");
  //~ gets(row);  sscanf(row, "%f", &lambda);
  scanf("%f", &lambda);
  printf("number of iterations: ");
  //~ gets(row);  sscanf(row, "%ld", &imax);
  scanf("%ld", &imax);
  inputlag = inputlag + ((double)(clock() - start))/CLOCKS_PER_SEC; 

/* ---- process image ---------------------------------------------------------------- */
  start = clock();
  for (i=1; i<=imax; i++)
    {
      printf("iteration number: %3ld \n", i);
      diff2d (0.5, lambda, PGMImage->x, PGMImage->y, matrix); 
    }
  process_image = process_image + ((double)(clock() - start))/CLOCKS_PER_SEC; 
  printf("%f",process_image);
  /* ---- copy the Result Image to PGM Image/File structure(CTRI) -------------------------- */
  start = clock();
  for (i=0; i<PGMImage->x; i++)
    for (j=0; j<PGMImage->y; j++)
      *(PGMImage->imageData + i*PGMImage->y + j) = (char) matrix[i][j];
  CTRI =  CTRI + ((double)(clock() - start))/CLOCKS_PER_SEC; 
   printf("%f",CTRI);
  /* ---- inputlag -------------------------------------------------------------------- */
  start = clock();
  //------------------------------
  if (argc < 2)
  {
    printf("name of output PGM image file (with extender): ");
    scanf("%s", PGMImage->fileName);
  }
  else
  { 
    strcpy(PGMImage->fileName, argv[2]);
  }
  inputlag = inputlag + ((double)(clock() - start))/CLOCKS_PER_SEC; 
  /* ---- write image ---------------------------------------------------------------- */
  start = clock();
  write8bitPGM(PGMImage);
  write_image =write_image +  ((double)(clock() - start))/CLOCKS_PER_SEC; 
  /* ---- disallocate storage -------------------------------------------------------- */
  start = clock();
  for (i=0; i<PGMImage->x; i++)
    free(matrix[i]);
  free(matrix);

  free(PGMImage->imageData);
  free(PGMImage);
  totalend = clock();
  disallocate_storage =  disallocate_storage + ((double)(totalend - start))/CLOCKS_PER_SEC; 
  cpu_time_used = ((double) (totalend - totalstart)) / CLOCKS_PER_SEC;
  printf("read image name: %f\n", read_image_name);
  printf("inputlag: %f\n", inputlag);
  printf("llocate storage for matrix(ASFM): %f\n",ASFM);
  printf("read image data into matrix(RIM): %f\n",RIM);
  printf("process image: %f\n", process_image);
  printf("copy the Result Image to PGM Image/File structure(CTRI) : %f\n", CTRI);
  printf("write image: %f\n",write_image);
  printf("disallocate storage: %f\n", disallocate_storage);
  printf("Tempo TOTAL de execucao: %f\n", cpu_time_used - inputlag);
}

 
