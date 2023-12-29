// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mandelbrot.h"
#include "pgm.h"
#include <math.h>

#define WIDTH_SCALE 1.5
#define MAX_COLOR 255

// Think about using error like this
// perror("Could not sync the file to disk");

// Set info about the image
int set_image_info(netpbm_ptr img_ptr, const char *file_name,
                   const int n_rows) {
  img_ptr->fd = fopen(file_name, "w+");

  if (img_ptr->fd == NULL) {
    return 2;
  }

  img_ptr->height = n_rows;
  img_ptr->width = n_rows * 1.5;

  // Write to the newly created file
  // Maybe do also this part with map
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

// Perhaps pass the function you care about as a parameter and if the function
// is the Mandelbrot set you can do it symmetric
int create_image(const char *file_name, const int max_iter, const int n_rows) {
  netpbm image;

  int err = set_image_info(&image, file_name, n_rows);

  // Maybe switch case hear
  if (err != 0) {
    return err;
  }

  // Maybe in the future pass this
  const float log_max_iter = log((float)max_iter);

#ifdef SIMD

#define NUM_PIXELS 4

  // Parallel computation using OpenMP
#pragma omp parallel for schedule(dynamic)
  for (int y = 0; y <= image.height / 2; y += 4) {
    // Ensure alignment for SIMD operations using GCC/Clang specific attribute
    float imag_values[NUM_PIXELS] __attribute__((aligned(16)));
    for (int i = 0; i < 4; ++i) {
      imag_values[i] = (2.0 * (y + i) / (image.height - 1.0)) - 1.0;
    }
    v4sf imag = *(v4sf *)imag_values;

    for (int x = 0; x < image.width; x++) {
      float reals = (3.0 * (float)x / ((float)image.width - 1.0)) - 2.0;
      v4sf real = (v4sf){reals, reals, reals, reals};

      // Compute the symmetric part together
      char *pixel[NUM_PIXELS] = {
          pixel_at(&image, x, y), pixel_at(&image, x, y + 1),
          pixel_at(&image, x, y + 2), pixel_at(&image, x, y + 3)};

      char *pixel_symmetric[NUM_PIXELS] = {
          pixel_at(&image, x, image.height - y - 1),
          pixel_at(&image, x, image.height - y - 2),
          pixel_at(&image, x, image.height - y - 3),
          pixel_at(&image, x, image.height - y - 4)};

      // Compute the Mandelbrot set value for the current point
      v4si mandelbrot_val = mandelbrot_point_calc(real, imag, max_iter);

      for (int i = 0; i < NUM_PIXELS; i++) {
        *pixel[i] = MAX_COLOR * (log((float)mandelbrot_val[i]) / log_max_iter);
        *pixel_symmetric[i] = *pixel[i];
      }
    }
  }

// #elif __ARM_NEON
//
// #define NUM_PIXELS 4
//
//   // Ensure alignment for SIMD operations using GCC/Clang specific attribute
//   float imag_values[NUM_PIXELS] __attribute__((aligned(16)));
//
// #pragma omp parallel for schedule(dynamic) // Improved workload
//
//   for (int y = 0; y <= image.height / 2; y += 4) {
//     for (int i = 0; i < 4; ++i) {
//       imag_values[i] = (2.0 * (y + i) / (image.height - 1.0)) - 1.0;
//     }
//     float32x4_t imag = vld1q_f32(imag_values);
//
//     for (int x = 0; x < image.width; x++) {
//       float reals = (3.0 * (double)x / ((double)image.width - 1.0)) - 2.0;
//       float32x4_t real = vld1q_dup_f32(&reals);
//
//       // Compute the symmetric part together
//       char *pixel[NUM_PIXELS] = {
//           pixel_at(&image, x, y), pixel_at(&image, x, y + 1),
//           pixel_at(&image, x, y + 2), pixel_at(&image, x, y + 3)};
//
//       char *pixel_symmetric[NUM_PIXELS] = {
//           pixel_at(&image, x, image.height - y - 1),
//           pixel_at(&image, x, image.height - y - 2),
//           pixel_at(&image, x, image.height - y - 3),
//           pixel_at(&image, x, image.height - y - 4)};
//
//       uint32x4_t mandelbrot_val = mandelbrot_point_calc(real, imag,
//       max_iter);
//
//       for (int i = 0; i < NUM_PIXELS; i++) {
//         *pixel[i] = MAX_COLOR * (log((float)mandelbrot_val[i]) /
//         log_max_iter); *pixel_symmetric[i] = *pixel[i];
//       }
//     }
//   }
//
#else
  // distribution
  float y_normalization = 2.0 / ((float)image.height - 1.0);
  float x_normalization = 3.0 / ((float)image.width - 1.0);

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
