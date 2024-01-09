// Jacopo Zacchinga SM3201293
#include "pgm.h"
#include <omp.h>
#include <string.h>

// Function to check file extension
int has_pgm_extension(const char *file_name) {
  int len = strlen(file_name);
  return (len > 4 && strcmp(&file_name[len - 4], ".pgm") == 0);
}

// Function to validate input parameters
int validate_inputs(int argc, const char *file_name, int max_iter, int n_rows) {
  if (!has_pgm_extension(file_name)) {
    fprintf(stderr, "Error: File name must end with .pgm extension\n");
    return INVALID_FILE_EXTENSION;
  }
  if (max_iter <= 0) {
    fprintf(stderr, "Error: max_iterations must be a positive integer\n");
    return INVALID_MAX_ITER;
  }
  if (n_rows <= 0) {
    fprintf(stderr, "Error: n_rows must be a positive integer\n");
    return INVALID_N_ROWS;
  }
  return SUCCESS;
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <file_name> <max_iterations> <n_rows>\n",
            argv[0]);
    return INVALID_ARGUMENTS;
  }

  const char *file_name = argv[1];
  const int max_iter = atoi(argv[2]);
  const int n_rows = atoi(argv[3]);

  int validation_status = validate_inputs(argc, file_name, max_iter, n_rows);
  if (validation_status != SUCCESS) {
    return validation_status;
  }

  double start = omp_get_wtime();
  int error = create_image(file_name, max_iter, n_rows);
  double end = omp_get_wtime();

  if (error != SUCCESS) {
    fprintf(stderr, "Error occurred: %d\n", error);
    return error;
  }

  double ms = (end - start) * 1000;
#ifdef DEBUG
#ifdef GCC_EXTENSIONS
  printf(
      "\033[1;33mUsing version with GCC extensions\n\033[0m"); // Yellow color
#else
  printf("\033[1;32mUsing default version\n\033[0m"); // Green color
#endif
  printf("Time taken: %f ms\n", ms);
#endif

  return SUCCESS;
}
