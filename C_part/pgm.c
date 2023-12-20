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
  const float log_max_iter = log((float)max_iter);

#ifdef SIMD
#pragma omp parallel for schedule(dynamic) // Improved workload

  // // distribution
  // // for (int y = 0; y <= image.height / 2; y++) {
  // for (int y = 0; y <= image.height / 2; y++) {
  //   // Compute the normalized coordinates
  //   v4df imag;
  //   for (int j = 0; j < 8; j++) {
  //     imag[j] = (2.0 * (float)(y + j) / ((float)image.height - 1.0)) - 1.0;
  //   }
  //
  //   // Inner loop
  //   for (int x = 0; x < image.width; x += 8) {
  //     // storing real part
  //     v4df real;
  //     for (int j = 0; j < 8; j++) {
  //       real[j] = (3.0 * (float)(x + j) / ((float)image.width - 1.0)) - 2.0;
  //     }
  //
  //     if (x + 8 > image.width) {
  //       // Handle the case where x + 8 is greater than image.width
  //       // This is where you should compute the pixels without using the SIMD
  //       // version
  //       // Compute the symmetric part together
  //     } else {
  //       char *pixel[8]; // Declare pixel as an array of 8 char elements
  //       char *pixel_symmetric[8];
  //       for (int j = 0; j < 8; j++) {
  //         pixel[j] = pixel_at(&image, x, y + j);
  //         pixel_symmetric[j] = pixel_at(&image, x, image.height - y - (1 +
  //         j)); if (pixel[j] == NULL) {
  //           printf("Error at x = %d y = %d\n", x, y);
  //         }
  //       }
  //
  //       mandelbrot_point_calc(real, imag, pixel, pixel_symmetric, max_iter,
  //                             MAX_COLOR, log_max_iter);
  //     }
  //   }
  // }

  for (int y = 0; y <= image.height / 2; y++) {
    v4df imag;
    for (int j = 0; j < 8; j++) {
      imag[j] = (2.0 * (float)(y + j) / ((float)image.height - 1.0)) - 1.0;
    }

    for (int x = 0; x < image.width; x += 8) {
      v4df real;
      for (int j = 0; j < 8; j++) {
        real[j] = (3.0 * (float)(x + j) / ((float)image.width - 1.0)) - 2.0;
      }

      char *pixel[8];
      char *pixel_symmetric[8];
      for (int j = 0; j < 8; j++) {
        pixel[j] = pixel_at(&image, x, y + j);
        pixel_symmetric[j] = pixel_at(&image, x, image.height - y - (1 + j));
        if (pixel[j] == NULL) {
          printf("Error at x = %d y = %d\n", x, y);
        }
      }

      mandelbrot_point_calc(real, imag, pixel, pixel_symmetric, max_iter,
                            MAX_COLOR, log_max_iter);
    }
  }

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

// int open_image(char * path, netpbm_ptr img)
// {
//   img->fd = fopen(path, "r+");
//   if (img->fd == NULL) {
//     return -1;
//   }
//   struct stat sbuf;
//   stat(path, &sbuf);
//   img->size = sbuf.st_size;
//   if (fscanf(img->fd, "P5\n%d %d\n255\n", &img->width, &img->height) != 2)
//   {
//     fclose(img->fd);
//     return -2;
//   }
//   img->offset = ftell(img->fd);
//   img->data = mmap((void *)0, img->size, PROT_READ | PROT_WRITE,
//   MAP_SHARED, fileno(img->fd), 0); if (img->data == MAP_FAILED) {
//     fclose(img->fd);
//     return -3;
//   }
//   return 0;
// }

// int empty_image(char * path, netpbm_ptr img, int width, int height)
// {
//   FILE * fd = fopen(path, "w+");
//   if (fd == NULL) {
//     return -1;
//   }
//   int written = fprintf(fd, "P5\n%d %d\n255\n", width, height);
//   ftruncate(fileno(fd), written + width * height);
//   fclose(fd);
//   return open_image(path, img);
// }

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
