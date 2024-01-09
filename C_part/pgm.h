// Jacopo Zacchinga SM3201293
#pragma once
#include <stdio.h>
#include <stdlib.h>

#define WIDTH_SCALE 1.5
#define MAX_COLOR 255

struct _netpbm_image {
  int width;
  int height;
  int offset;
  int size;
  char *data;
  FILE *fd;
};

typedef struct _netpbm_image netpbm;
typedef struct _netpbm_image *netpbm_ptr;

typedef enum {
  SUCCESS = 0,
  INVALID_ARGUMENTS = -1,
  INVALID_FILE_EXTENSION = -2,
  INVALID_MAX_ITER = -3,
  INVALID_N_ROWS = -4,
  ERROR_NULL_POINTER = -5,
  ERROR_FILE_OPEN = -6,
  ERROR_FILE_WRITE = -7,
  ERROR_FILE_TRUNCATE = -8,
  ERROR_MMAP = -9,
  ERROR_FILE_CLOSE = -10
} ErrorCode;

// Set info about the image and map the file
int set_image_info(netpbm_ptr img_ptr, const char *file_name, const int n_rows);

// Set up image and call the Mandelbrot function but let the potential to call
// other functions instead of that
int create_image(const char *file_name, const int max_iter, const int n_rows);

// Create image computing the color for each point
void calculate_mandelbrot(netpbm *image, const int max_iter);

// Get the pixel at a specific position
char *pixel_at(netpbm_ptr img_ptr, int x, int y);

int close_image(netpbm_ptr img_ptr);
