// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
#include <complex.h>
#include <stdio.h>
#include <stdlib.h> // it directly includes stddef.h. (for size_t)

#include "mandelbrot.h"

// #define RADIUS 2
#define RADIUS_SQUARED 4
#define PERIOD 100

// define macros if they help you look it up online

// double mandelbrot_calculator(uint16 num_iterations)

// Iterative implementation of mandelbrot_set -> less stack-frames

#ifdef SIMD
v4df mandelbrot_point_calc(v4df x0, v4df y0, const int max_iter) {
  v4df x = {0.0, 0.0, 0.0, 0.0};
  v4df y = {0.0, 0.0, 0.0, 0.0};
  v4df x2 = {0.0, 0.0, 0.0, 0.0};
  v4df y2 = {0.0, 0.0, 0.0, 0.0};
  v4df result = {0.0, 0.0, 0.0, 0.0};
  v4df adding = {1.0, 1.0, 1.0, 1.0};
  v4df old_position_real = {0.0, 0.0, 0.0, 0.0};
  v4df old_position_imag = {0.0, 0.0, 0.0, 0.0};
  v4df old_position_check = {0.0, 0.0, 0.0, 0.0};
  int period = 0;

  for (unsigned int i = 0; i < max_iter; i++) {
    if ((x2 + y2)[0] >= RADIUS_SQUARED && (x2 + y2)[1] >= RADIUS_SQUARED &&
        (x2 + y2)[2] >= RADIUS_SQUARED && (x2 + y2)[3] >= RADIUS_SQUARED) {
      return result;
    }

    // testing then I want to create a mask to add to the result
    adding[0] = (x2 + y2)[0] < RADIUS_SQUARED;
    adding[1] = (x2 + y2)[1] < RADIUS_SQUARED;
    adding[2] = (x2 + y2)[2] < RADIUS_SQUARED;
    adding[3] = (x2 + y2)[3] < RADIUS_SQUARED;

    y = 2 * x * y + y0;
    x = x2 - y2 + x0;
    x2 = x * x;
    y2 = y * y;

    old_position_check[0] =
        (old_position_real[0] == x[0]) && (old_position_imag[0] == y[0]);
    old_position_check[1] =
        (old_position_real[1] == x[1]) && (old_position_imag[1] == y[1]);
    old_position_check[2] =
        (old_position_real[2] == x[2]) && (old_position_imag[2] == y[2]);
    old_position_check[3] =
        (old_position_real[3] == x[3]) && (old_position_imag[3] == y[3]);

    if ((old_position_check[0] == 1.0) && (old_position_check[1] == 1.0) &&
        (old_position_check[2] == 1.0) && (old_position_check[3] == 1.0)) {
      return (v4df){max_iter, max_iter, max_iter, max_iter};
    }

    period += 1;
    if (period == PERIOD) {
      old_position_real = x;
      old_position_imag = y;
      period = 0;
    }

    result += adding;
  }

  return result;
}
#else
int mandelbrot_point_calc(double x0, double y0, const int max_iter) {
  // This will be computed thanks to starting point perhaps two variables
  // double complex c = x + y * i;
  // double complex z = c;

  // use the bailout method to keep in mind the derivative
  double x = 0.0;
  double y = 0.0;
  double x_squared = 0.0;
  double y_squared = 0.0;

  // periodicity checking
  int period = 0;
  double old_position_real = 0.0;
  double old_position_imag = 0.0;
  //
  // implement the function iteratively
  unsigned int i = 0;
  while (i < max_iter) {

    // check if radius distance is greater or equal to the radius
    if ((x_squared + y_squared) >= RADIUS_SQUARED) {
      // if ((cabs(z) >= radius)) {
      return i;
    }

    // slow implementation
    // double xtemp = x * x - y * y + x0;
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
      return max_iter;
    }

    if (period % PERIOD == 0) {
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
#endif
