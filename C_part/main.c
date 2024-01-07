// Jacopo Zacchinga SM3201293
#include "pgm.h"
#include <omp.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Error: Invalid arguments.\n");
    fprintf(stderr, "Usage: %s <file_name> <max_iterations> <n_rows>\n",
            argv[0]);
    return -1;
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

#ifdef GCC_EXTENSIONS
  printf(
      "\033[1;33mUsing version with GCC extensions\n\033[0m"); // Yellow color
#else
  printf("\033[1;32mUsing default version\n\033[0m"); // Green color
#endif

  printf("\nTime taken: %f ms\n\n", ms);

  return 0;
}
