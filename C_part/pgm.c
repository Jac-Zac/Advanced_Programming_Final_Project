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
  // Maube do also this part with mmmap
  fprintf(img_ptr->fd, "P5\n%d %d\n255\n", img_ptr->width, img_ptr->height);

  // IMPORTANT
  // Flush the file to ensure all data is written
  // fflush(img_ptr->fd);
  //
  int fdnum = fileno(img_ptr->fd);

  // from my current position save the offset
  img_ptr->offset = ftell(img_ptr->fd);

  // Compute the size
  img_ptr->size = img_ptr->width * img_ptr->height + img_ptr->offset;
  // img_ptr->size = img_ptr->width * img_ptr->height;

  // Extend the file size to the desired size
  if (ftruncate(fdnum, img_ptr->size) == -1) {
    perror("Failed to extend the file size");
    fclose(img_ptr->fd);
    return -1;
  }

  // mapping the file to the ...
  // size + 1000 just an hacky way to do it
  img_ptr->data = mmap((void *)0, img_ptr->size, PROT_READ | PROT_WRITE,
                       MAP_SHARED, fileno(img_ptr->fd), 0);

  if (img_ptr->data == MAP_FAILED) {
    // USE THIS TO CLOSE
    // close_image(img_ctr);
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
  const double log_max_iter = log((double)max_iter);

#ifdef SIMD
#pragma omp parallel for schedule(dynamic) // Improved workload

  // distribution
  for (int y = 0; y <= image.height / 2; y += 4) {
    // Compute the normalized coordinates
    v4df imag = {(2.0 * (double)y / ((double)image.height - 1.0)) - 1.0,
                 (2.0 * (double)(y + 1) / ((double)image.height - 1.0)) - 1.0,
                 (2.0 * (double)(y + 2) / ((double)image.height - 1.0)) - 1.0,
                 (2.0 * (double)(y + 3) / ((double)image.height - 1.0)) -
                     1.0}; // Moved outside inner loop
    //
    for (int x = 0; x < image.width; x++) {
      v4df real = {
          (3.0 * (double)x / ((double)image.width - 1.0)) - 2.0,
          (3.0 * (double)x / ((double)image.width - 1.0)) - 2.0,
          (3.0 * (double)x / ((double)image.width - 1.0)) - 2.0,
          (3.0 * (double)x / ((double)image.width - 1.0)) - 2.0,
      };

      // Compute the symmetric part together
      char *pixel[4] = {pixel_at(&image, x, y), pixel_at(&image, x, y + 1),
                        pixel_at(&image, x, y + 2), pixel_at(&image, x, y + 3)};

      char *pixel_symmetric[4] = {pixel_at(&image, x, image.height - y - 1),
                                  pixel_at(&image, x, image.height - y - 2),
                                  pixel_at(&image, x, image.height - y - 3),
                                  pixel_at(&image, x, image.height - y - 4)};

      mandelbrot_point_calc(real, imag, pixel, pixel_symmetric, max_iter,
                            MAX_COLOR, log_max_iter);
    }
  }
#else

  // distribution
  double y_normalization = 2.0 / ((double)image.height - 1.0);
  double x_normalization = 3.0 / ((double)image.width - 1.0);

#pragma omp parallel for schedule(dynamic)
  for (int y = 0; y <= image.height / 2; y++) {
    // Compute the normalized coordinates
    double imag = (y * y_normalization) - 1.0;
    for (int x = 0; x < image.width; x++) {
      double real = (x * x_normalization) - 2.0;

      // Compute the symmetric part together
      char *pixel = pixel_at(&image, x, y);
      // Wasting a bit of time hear
      char *pixel_symmetric = pixel_at(&image, x, image.height - y - 1);

      mandelbrot_point_calc(real, imag, pixel, pixel_symmetric, max_iter,
                            MAX_COLOR, log_max_iter);
    }
  }
#endif
  return close_image(&image);
}

char *pixel_at(netpbm_ptr img_ptr, int x, int y) {
  if ((x < 0 || x > img_ptr->width) || (y < 0 || y > img_ptr->height)) {
    return NULL;
  }
  return &img_ptr->data[y * img_ptr->width + x + img_ptr->offset];
}

int close_image(netpbm_ptr img_ptr) {
  munmap(img_ptr->data, img_ptr->size);
  return fclose(img_ptr->fd);
}
