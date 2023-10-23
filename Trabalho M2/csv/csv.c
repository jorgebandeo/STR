#include "csv.h"
#include <stdio.h>
#include <string.h>

void write_to_csv(char function_name[], double data)
{
  FILE *file;

  char full_path[100] = "csv/results/";
  strcat(full_path, function_name);
  strcat(full_path, ".csv");

  file = fopen(full_path, "a+");
  fprintf(file, "%f\n", data);
  fclose(file);
}
