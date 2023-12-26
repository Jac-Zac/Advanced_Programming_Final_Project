// Copyright (c) 2023 Jacopo Zacchinga. All Rights Reserved.

#include <omp.h>
#include <stdio.h>

#define WARNING 1

// Tmp testing
#include "pgm.h"

#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Usage: %s <file_name> <max_iterations> <n_rows>\n", argv[0]);
    return WARNING;
  }

  // Read arguments from command line
  // convert to integer where necessary
  const char *file_name = argv[1];
  const int max_iter = atoi(argv[2]);
  const int n_rows = atoi(argv[3]);

  double start = omp_get_wtime();
  int error = create_image(file_name, max_iter, n_rows);
  double end = omp_get_wtime();

  if (error != 0) {
    printf("Erorr: %d", error);
  }

  double ms = (end - start) * 1000;

#ifdef SIMD
  printf("\033[1;31mUsing Neon version\n\033[0m"); // Green color
#else
  printf("\033[1;32mUsing default version\n\033[0m"); // Green color
#endif

  printf("\nTime taken: %f ms\n\n", ms);

  return 0;
}
