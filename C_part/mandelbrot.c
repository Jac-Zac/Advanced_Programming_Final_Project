// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
#include "mandelbrot.h"
#include <math.h>

// #define RADIUS 2
#define RADIUS_SQUARED 4

// The period is chosen in relation to the number of max_iter
// If we wanted a really high number of max iteration we should decide on a
// bigger period
#define PERIOD 100

// define macros if they help you look it up online

// float Mandelbrot_calculator(unite nm_iterations)

// Iterative implementation of mandelbrot_set -> less stack-frames

#ifdef SIMD
void mandelbrot_point_calc(v4df x0, v4df y0, char **pixel,
                           char **pixel_symmetric, const int max_iter,
                           const int MAX_COLOR, const float log_max_iter) {
  // int flag = 0;
  // // Check if the point is in the set
  // v4df q = (x0 - 0.25) * (x0 - 0.25) + y0 * y0;
  // v4df mask = (q * (q + (x0 - 0.25)) <= 0.25 * y0 * y0);
  //
  // for (int i = 0; i < 8; i++) {
  //   // is inside the bulb
  //   if (mask[i]) {
  //     flag += 1;
  //   }
  // }
  //
  // if (flag == 8) {
  //   *pixel[0] = (char)MAX_COLOR;
  //   *pixel[1] = (char)MAX_COLOR;
  //   *pixel[2] = (char)MAX_COLOR;
  //   *pixel[3] = (char)MAX_COLOR;
  //   *pixel[4] = (char)MAX_COLOR;
  //   *pixel[5] = (char)MAX_COLOR;
  //   *pixel[6] = (char)MAX_COLOR;
  //   *pixel[7] = (char)MAX_COLOR;
  //   // *pixel_symmetric[0] = (char)(MAX_COLOR);
  //   // *pixel_symmetric[1] = (char)(MAX_COLOR);
  //   // *pixel_symmetric[2] = (char)(MAX_COLOR);
  //   // *pixel_symmetric[3] = (char)(MAX_COLOR);
  // } else {
  // It should be v8d
  // vector of 8 zeros
  v4df x = {0.0};
  v4df y = {0.0};
  v4df x2 = {0.0};
  v4df y2 = {0.0};
  v4df result = {0.0};
  int period = 0;

  int i = 0;
  while (i < max_iter) {
    // if (((x2 + y2)[0] >= RADIUS_SQUARED && (x2 + y2)[1] >= RADIUS_SQUARED) &&
    //     ((x2 + y2)[2] >= RADIUS_SQUARED && (x2 + y2)[3] >= RADIUS_SQUARED) &&
    //     ((x2 + y2)[4] >= RADIUS_SQUARED && (x2 + y2)[5] >= RADIUS_SQUARED) &&
    //     ((x2 + y2)[6] >= RADIUS_SQUARED && (x2 + y2)[7] >= RADIUS_SQUARED)) {
    //   break;
    // }

    // testing then I want to create a mask to add to the result
    for (int j = 0; j < 8; j++) {
      if ((x2 + y2)[j] < RADIUS_SQUARED) {
        // Change this part to use gcc intrinsics
        result[j] += 1.0;
      } else {
        result[j] += 0.0;
      }
    }

    y = 2 * x * y + y0;
    x = x2 - y2 + x0;
    x2 = x * x;
    y2 = y * y;

    period++;
    i++;
  }

  for (int j = 0; j < 8; j++) {
    *pixel[j] = (char)(MAX_COLOR * log((float)result[j]) / log_max_iter);
    // *pixel[j] = (char)(MAX_COLOR * ((float)result[j]) / log_max_iter);
    *pixel_symmetric[j] = *pixel[j];
  }
}
// }
#else

// Clean up the number of arguments passed
int mandelbrot_point_calc(float x0, float y0, const int max_iter) {
  // variable to check if inside bulb
  // is inside the bulb
  // Link to the Wikimedia article
  float q = (x0 - 0.25) * (x0 - 0.25) + y0 * y0;
  if (q * (q + (x0 - 0.25)) <= 0.25 * y0 * y0) {
    return max_iter;
  } else {
    // This will be computed thanks to starting point perhaps two variables
    // float complex c = x + y * i;
    // float complex z = c;

    // use the bailout method to keep in mind the derivative
    float x = 0.0;
    float y = 0.0;
    float x_squared = 0.0;
    float y_squared = 0.0;

    // periodicity checking
    int period = 0;
    float old_position_real = 0.0;
    float old_position_imag = 0.0;
    //
    // implement the function iteratively
    unsigned int i = 0;
    while (i < max_iter) {
      // check if radius distance is greater or equal to the radius
      if ((x_squared + y_squared) >= RADIUS_SQUARED) {
        return i;
      }

      // slow implementation
      // float xtemp = x * x - y * y + x0;
      // y = 2 * x * y + y0;
      // x = xtemp;
      //
      // Find the position
      y = 2 * x * y + y0;
      x = x_squared - y_squared + x0;

      // update x_squared and y_squared for the next iteration
      x_squared = x * x;
      y_squared = y * y;

      // at each 100 periods if repeated exit
      // trade of memory for compute
      if ((old_position_real == x && old_position_imag == y)) {
        // set to max for the color plotting
        // You could simply I believe
        break;
      }

      // 0 for read, 3 for temporal locality
      // __builtin_prefetch(&old_position_real, 0, 3);
      // __builtin_prefetch(&old_position_imag, 0, 3);
      // __builtin_prefetch(&x, 0, 3);
      // __builtin_prefetch(&y, 0, 3);
      if (period % PERIOD == 0) {
        // old_position_real = x; is slowwww
        old_position_real = x;
        old_position_imag = y;
      }
      // this obviously must be above the other
      // check every the old position is the same break
      period++;
      i++;
    }
    return max_iter;
  }
}
#endif
