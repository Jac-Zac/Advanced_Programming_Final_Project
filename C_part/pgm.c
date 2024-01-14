// Jacopo Zacchinga SM3201293
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mandelbrot.h"
#include "pgm.h"

// Function to set image information
int set_image_info(netpbm_ptr img_ptr, const char *file_name,
                   const int n_rows) {
  img_ptr->fd = fopen(file_name, "w+");
  if (img_ptr->fd == NULL) {
    perror("Error: Unable to open file");
    return ERROR_FILE_OPEN;
  }

  img_ptr->height = n_rows;
  img_ptr->width = n_rows * WIDTH_SCALE;

  int header_size =
      fprintf(img_ptr->fd, "P5\n%d %d\n255\n", img_ptr->width, img_ptr->height);
  if (header_size < 0) {
    fclose(img_ptr->fd);
    return ERROR_FILE_WRITE;
  }

  img_ptr->offset = header_size;

  // Compute the size
  img_ptr->size = img_ptr->width * img_ptr->height + img_ptr->offset;

  if (ftruncate(fileno(img_ptr->fd), img_ptr->size) == -1) {
    perror("Failed to extend the file size");
    fclose(img_ptr->fd);
    return ERROR_FILE_TRUNCATE;
  }

  // The mmap call maps the file described by the file descriptor into memory.
  // PROT_READ | PROT_WRITE allows both reading and writing to the mapped
  // memory. MAP_SHARED allows the mapping to be shared with other processes.
  img_ptr->data = mmap(NULL, img_ptr->size, PROT_READ | PROT_WRITE, MAP_SHARED,
                       fileno(img_ptr->fd), 0);

  if (img_ptr->data == MAP_FAILED) {
    perror("Memory mapping failed");
    fclose(img_ptr->fd);
    return ERROR_MMAP;
  }

  return SUCCESS;
}

int create_image(const char *file_name, const int max_iter, const int n_rows) {
  netpbm image;
  int err = set_image_info(&image, file_name, n_rows);
  if (err != SUCCESS) {
    return err;
  }

  calculate_mandelbrot(&image, max_iter);
  return close_image(&image);
}

void calculate_mandelbrot(netpbm *img_ptr, const int max_iter) {
  // This is a constant which we can pre compute
  const float log_max_iter = log((float)max_iter);
  // Values we use for scaling purposes
  float y_normalization = 2.0f / ((float)img_ptr->height - 1.0f);
  float x_normalization = 3.0f / ((float)img_ptr->width - 1.0f);

#ifndef GCC_EXTENSIONS
#pragma omp parallel for schedule(dynamic)
  // Exploit the symmetry of the Mandelbrot set to only compute half of it
  for (int y = 0; y <= img_ptr->height / 2; y++) {
    // Compute the normalized coordinates
    float imag = (y * y_normalization) - 1.0;
    for (int x = 0; x < img_ptr->width; x++) {
      float real = (x * x_normalization) - 2.0;

      // Compute the symmetric part together
      char *pixel = pixel_at(img_ptr, x, y);
      char *pixel_symmetric = pixel_at(img_ptr, x, img_ptr->height - y - 1);

      int mandelbrot_val = calculate_mandelbrot_pixel(real, imag, max_iter);

      // It is cleaner if I do this
      if (mandelbrot_val == max_iter) {
        *pixel = MAX_COLOR;
      } else {
        *pixel = MAX_COLOR * log((float)mandelbrot_val) / log_max_iter;
      }

      *pixel_symmetric = *pixel;
    }
  }

#else
// Number of float we can fit using SIMD instruction to vectorize the code
#define NUM_PIXELS 4

// Parallel computation using OpenMP with dynamic scheduler since different
// iteration might take different times
#pragma omp parallel for schedule(dynamic)
  // Exploit the symmetry of the Mandelbrot set to only compute half of it
  for (int y = 0; y <= img_ptr->height / 2; y += NUM_PIXELS) {
    // Initialize NUM_PIXELS with different y values in the outer loop to avoid
    // unnecessary re computation
    v4sf imag = {y * y_normalization - 1.0f, (y + 1) * y_normalization - 1.0f,
                 (y + 2) * y_normalization - 1.0f,
                 (y + 3) * y_normalization - 1.0f};

    for (int x = 0; x < img_ptr->width; x++) {
      float reals = x * x_normalization - 2.0;
      // Initialize NUM_PIXELS with the same x value
      v4sf real = (v4sf){reals, reals, reals, reals};

      // Compute the symmetric part together
      char *pixel[NUM_PIXELS] = {
          pixel_at(img_ptr, x, y), pixel_at(img_ptr, x, y + 1),
          pixel_at(img_ptr, x, y + 2), pixel_at(img_ptr, x, y + 3)};

      char *pixel_symmetric[NUM_PIXELS] = {
          pixel_at(img_ptr, x, img_ptr->height - y - 1),
          pixel_at(img_ptr, x, img_ptr->height - y - 2),
          pixel_at(img_ptr, x, img_ptr->height - y - 3),
          pixel_at(img_ptr, x, img_ptr->height - y - 4)};

      // Compute the Mandelbrot set value for the current point
      v4si mandelbrot_val = calculate_mandelbrot_pixel(real, imag, max_iter);

      // Set the values for the different pixels
      for (int i = 0; i < NUM_PIXELS; i++) {
        // Adding also this check is better solution actually
        if (mandelbrot_val[i] == max_iter) {
          *pixel[i] = MAX_COLOR;
        } else {
          *pixel[i] =
              MAX_COLOR * (log((float)mandelbrot_val[i]) / log_max_iter);
        }
        *pixel_symmetric[i] = *pixel[i];
      }
    }
  }
#endif
}

char *pixel_at(netpbm_ptr img_ptr, int x, int y) {
  if (img_ptr == NULL) {
    return NULL;
  }
  if (x < 0 || x >= img_ptr->width) {
    return NULL;
  }
  if (y < 0 || y >= img_ptr->height) {
    return NULL;
  }
  return &img_ptr->data[y * img_ptr->width + x + img_ptr->offset];
}

int close_image(netpbm_ptr img_ptr) {
  if (img_ptr == NULL) {
    return ERROR_FILE_CLOSE;
  }
  munmap(img_ptr->data, img_ptr->size);

  return fclose(img_ptr->fd);
}
