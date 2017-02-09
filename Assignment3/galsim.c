#define _BSD_SOURCE

#include "file_operations/file_operations.h"
#include "graphics/graphics.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Sleep function

const int windowWidth = 800;

int main(int argc, char *argv[]) {
  
  /* Check for correct number of input arguments */
  if (argc != 6) {
    printf("Error: Expected exactly 5 input arguments.");
    return -1;
  }
    
  /* Input variables */
  short N = (short)atoi(argv[1]);
  char *filename = argv[2];
  int nsteps = atoi(argv[3]);
  double delta_t = atof(argv[4]);
  const int graphics = atoi(argv[5]);
  
  /* Setup graphics */
  if (graphics) {
    InitializeGraphics(argv[5], windowWidth, windowWidth);
    SetCAxes(0,1);
  }
  float L = 1;
  float W = 1;
  float radius = 0.01*L;
  float circleColor = 0;
  
  /* Constants */
  const double G = 100/(double)N;
  const double epsilon = 0.001;
  
  /* Read file */
  char fileDest[50];
  strcpy(fileDest, "input_data/");
  strcat(fileDest, filename);
  double *data = (double*)malloc(N*5*sizeof(double));
  int flag = read_doubles_from_file(N*5, data, fileDest);
  printf("Reading input file: flag = %i\n", flag);
  
  /* Display input data */
  printf("\nInput data:\n");
  double x, y, m, vx, vy;
  for (short i = 0; i < N; i++) {
    x = data[5*i];
    y = data[5*i + 1];
    m = data[5*i + 2];
    vx = data[5*i + 3];
    vy = data[5*i + 4];
    printf("%lf\t%lf\t%lf\t%lf\t%lf\n", x, y, m, vx, vy);
  }
  
  /* Create an array for the previous data */
  double *dataPrev = (double*)malloc(N*5*sizeof(double));
  
  /* Loop over time */
  double *dataTemp;
  for (short k = 0; k < nsteps; k++) {
    
    /* Update previous data */
    for (short i = 0; i < N*5; i++) {
      dataPrev[i] = data[i];
    }
    
    /* Doesn't work without this line */
    printf(" ");
    
    /* Update particles */
    for (short i = 0; i < N; i++) {
      dataTemp = update(data[5*i], data[5*i + 1], data[5*i + 2],
                        data[5*i + 3], data[5*i + 4], dataPrev,
                        i, N, G, epsilon, delta_t);
      data[5*i] = dataTemp[0];
      data[5*i + 1] = dataTemp[1];
      data[5*i + 2] = dataTemp[2];
      data[5*i + 3] = dataTemp[3];
      data[5*i + 4] = dataTemp[4];
    }
    
    /* Do graphics. */
    if (graphics) {
      ClearScreen();
      for (short i = 0; i < N; i++) {
        DrawCircle(data[5*i], data[5*i + 1], L, W, radius, circleColor);
      }
      Refresh();
      usleep(10);
    }
  }
    
  /* Close graphics */
  if (graphics) {
    FlushDisplay();
    CloseDisplay();
  }
  
  /* Display output data */
  printf("\n\nOutput data:\n");
  for (short i = 0; i < N; i++) {
    x = data[5*i];
    y = data[5*i + 1];
    m = data[5*i + 2];
    vx = data[5*i + 3];
    vy = data[5*i + 4];
    printf("%lf\t%lf\t%lf\t%lf\t%lf\n", x, y, m, vx, vy);
  }
  printf("\n\n");
  
  /* Write result file */
  flag = write_doubles_to_file(N*5, data, "result.gal");
  printf("\nWriting output file: flag = %i\n", flag);
  
  /* Free memory */
  free(data);
  free(dataPrev);
  
  return 0;
}

