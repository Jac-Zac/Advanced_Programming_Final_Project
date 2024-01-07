// Jacopo Zacchinga SM3201293
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mandelbrot.h"
#include "pgm.h"

#define WIDTH_SCALE 1.5
#define MAX_COLOR 255

int set_image_info(netpbm_ptr img_ptr, const char *file_name,
                   const int n_rows) {
  img_ptr->fd = fopen(file_name, "w+");

  if (img_ptr->fd == NULL) {
    fprintf(stderr, "Error: Unable to open file %s.\n", file_name);
    return -2;
  }

  img_ptr->height = n_rows;
  img_ptr->width = n_rows * WIDTH_SCALE;

  // Write to the newly created file
  int header_size =
      fprintf(img_ptr->fd, "P5\n%d %d\n255\n", img_ptr->width, img_ptr->height);

  img_ptr->offset = header_size;

  // Compute the size
  img_ptr->size = img_ptr->width * img_ptr->height + img_ptr->offset;

  // Extend the file size to the desired size
  if (ftruncate(fileno(img_ptr->fd), img_ptr->size) == -1) {
    perror("Failed to extend the file size");
    fclose(img_ptr->fd);
    return -1;
  }

  img_ptr->data = mmap((void *)0, img_ptr->size, PROT_READ | PROT_WRITE,
                       MAP_SHARED, fileno(img_ptr->fd), 0);

  if (img_ptr->data == MAP_FAILED) {
    fclose(img_ptr->fd);
    return -3;
  }

  return 0;
}

int create_image(const char *file_name, const int max_iter, const int n_rows) {
  netpbm image;

  int err = set_image_info(&image, file_name, n_rows);

  if (err != 0) {
    return err;
  }

  // This is a constant which we can pre compute
  const float log_max_iter = log((float)max_iter);
  // Values we use for scaling purposes
  float y_normalization = 2.0f / ((float)image.height - 1.0f);
  float x_normalization = 3.0f / ((float)image.width - 1.0f);

#ifdef GCC_EXTENSIONS
// Number of float we can fit using SIMD instruction to vectorize the code
#define NUM_PIXELS 4

// Parallel computation using OpenMP
#pragma omp parallel for schedule(dynamic)
  for (int y = 0; y <= image.height / 2; y += NUM_PIXELS) {
    // Initialize NUM_PIXELS with different y values
    v4sf imag = {y * y_normalization - 1.0f, (y + 1) * y_normalization - 1.0f,
                 (y + 2) * y_normalization - 1.0f,
                 (y + 3) * y_normalization - 1.0f};

    for (int x = 0; x < image.width; x++) {
      float reals = x * x_normalization - 2.0;
      // Initialize NUM_PIXELS with the same x value
      v4sf real = (v4sf){reals, reals, reals, reals};

      // Compute the symmetric part together
      char *pixel[NUM_PIXELS] = {
          pixel_at(&image, x, y), pixel_at(&image, x, y + 1),
          pixel_at(&image, x, y + 2), pixel_at(&image, x, y + 3)};

      // Exploit the symmetry of the Mandelbrot set to only compute half of it
      char *pixel_symmetric[NUM_PIXELS] = {
          pixel_at(&image, x, image.height - y - 1),
          pixel_at(&image, x, image.height - y - 2),
          pixel_at(&image, x, image.height - y - 3),
          pixel_at(&image, x, image.height - y - 4)};

      // Compute the Mandelbrot set value for the current point
      v4si mandelbrot_val = mandelbrot_point_calc(real, imag, max_iter);

      // Set the values for the different pixels
      for (int i = 0; i < NUM_PIXELS; i++) {
        *pixel[i] = MAX_COLOR * (log((float)mandelbrot_val[i]) / log_max_iter);
        *pixel_symmetric[i] = *pixel[i];
      }
    }
  }
#else

#pragma omp parallel for schedule(dynamic)
  for (int y = 0; y <= image.height / 2; y++) {
    // Compute the normalized coordinates
    float imag = (y * y_normalization) - 1.0;
    for (int x = 0; x < image.width; x++) {
      float real = (x * x_normalization) - 2.0;

      // Compute the symmetric part together
      char *pixel = pixel_at(&image, x, y);
      // Wasting a bit of time hear
      char *pixel_symmetric = pixel_at(&image, x, image.height - y - 1);

      int mandelbrot_val = mandelbrot_point_calc(real, imag, max_iter);

      *pixel = MAX_COLOR * log((float)mandelbrot_val) / log_max_iter;
      *pixel_symmetric = *pixel;
    }
  }
#endif
  return close_image(&image);
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
    return -1;
  }
  munmap(img_ptr->data, img_ptr->size);

  return fclose(img_ptr->fd);
}
