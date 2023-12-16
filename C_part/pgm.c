// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
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
    // close_image(img_ptr);
    fclose(img_ptr->fd);
    return -3;
  }

  return 0;
}

int create_image(const char *file_name, const int max_iter, const int n_rows) {
  netpbm image;

  int err = set_image_info(&image, file_name, n_rows);

  // Maybe switch case hear
  if (err != 0) {
    return err;
  }

  calculate_mandelbrot(&image, max_iter);

  return close_image(&image);
}

void calculate_mandelbrot(netpbm_ptr img_ptr, const int max_iter) {
  const double log_max_iter = log((double)max_iter);

#ifdef SIMD
#pragma omp parallel for schedule(dynamic) // Improved workload
  // distribution
  for (int y = 0; y <= img_ptr->height / 2; y += 4) {
    // Compute the normalized coordinates
    v4df imag = {
        (2.0 * (double)y / ((double)img_ptr->height - 1.0)) - 1.0,
        (2.0 * (double)(y + 1) / ((double)img_ptr->height - 1.0)) - 1.0,
        (2.0 * (double)(y + 2) / ((double)img_ptr->height - 1.0)) - 1.0,
        (2.0 * (double)(y + 3) / ((double)img_ptr->height - 1.0)) -
            1.0}; // Moved outside inner loop
    //
    for (int x = 0; x < img_ptr->width; x++) {
      v4df real = {
          (3.0 * (double)x / ((double)img_ptr->width - 1.0)) - 2.0,
          (3.0 * (double)x / ((double)img_ptr->width - 1.0)) - 2.0,
          (3.0 * (double)x / ((double)img_ptr->width - 1.0)) - 2.0,
          (3.0 * (double)x / ((double)img_ptr->width - 1.0)) - 2.0,
      };

      // Compute the symmetric part together
      char *pixel[4] = {pixel_at(img_ptr, x, y), pixel_at(img_ptr, x, y + 1),
                        pixel_at(img_ptr, x, y + 2),
                        pixel_at(img_ptr, x, y + 3)};

      char *pixel_symmetric[4] = {
          pixel_at(img_ptr, x, img_ptr->height - y - 1),
          pixel_at(img_ptr, x, img_ptr->height - y - 2),
          pixel_at(img_ptr, x, img_ptr->height - y - 3),
          pixel_at(img_ptr, x, img_ptr->height - y - 4)};

      int flag = 0;
      // Check if the point is in the set
      v4df q = (real - 0.25) * (real - 0.25) + imag * imag;
      v4df mask = (q * (q + (real - 0.25)) <= 0.25 * imag * imag);

      for (int i = 0; i < 4; i++) {
        // is inside the bulb
        if (mask[i]) {
          flag += 1;
        }
      }

      if (flag == 4) {
        *pixel[0] = (char)MAX_COLOR;
        *pixel[1] = (char)MAX_COLOR;
        *pixel[2] = (char)MAX_COLOR;
        *pixel[3] = (char)MAX_COLOR;
        *pixel_symmetric[0] = (char)(MAX_COLOR);
        *pixel_symmetric[1] = (char)(MAX_COLOR);
        *pixel_symmetric[2] = (char)(MAX_COLOR);
        *pixel_symmetric[3] = (char)(MAX_COLOR);
      } else {
        // Compute the normalized coordinates
        v4df mandelbrot_val = mandelbrot_point_calc(real, imag, max_iter);

        for (int i = 0; i < 4; i++) {
          *pixel[i] = mandelbrot_val[i] == max_iter
                          ? (char)MAX_COLOR
                          : (char)(MAX_COLOR * log((double)mandelbrot_val[i]) /
                                   log_max_iter);
          *pixel_symmetric[i] = *pixel[i];
        }
      }
    }
  }
#else
#pragma omp parallel for schedule(dynamic) // Improved workload
  // distribution
  for (int y = 0; y <= img_ptr->height / 2; y++) {
    // Compute the normalized coordinates
    double imag = (2.0 * (double)y / ((double)img_ptr->height - 1.0)) -
                  1.0; // Moved outside inner loop
    for (int x = 0; x < img_ptr->width; x++) {
      double real = (3.0 * (double)x / ((double)img_ptr->width - 1.0)) - 2.0;

      // Compute the symmetric part together
      char *pixel = pixel_at(img_ptr, x, y);
      char *pixel_symmetric = pixel_at(img_ptr, x, img_ptr->height - y - 1);

      // variable to check if inside bulb
      // is inside the bulb
      // Link to the Wikimedia article
      double q = (real - 0.25) * (real - 0.25) + imag * imag;
      if (q * (q + (real - 0.25)) <= 0.25 * imag * imag) {
        *pixel = (char)MAX_COLOR;
        *pixel_symmetric = (char)(MAX_COLOR);
      } else {
        int mandelbrot_val = mandelbrot_point_calc(real, imag, max_iter);
        *pixel = mandelbrot_val == max_iter
                     ? (char)MAX_COLOR
                     : (char)(MAX_COLOR * log((double)mandelbrot_val) /
                              log_max_iter);

        *pixel_symmetric = *pixel;
      }
    }
  }
#endif
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
