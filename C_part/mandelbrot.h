// Jacopo Zacchinga SM3201293
#pragma once
#include <math.h>

// #define RADIUS 2
#define RADIUS_SQUARED 4

// The period is chosen in relation to the number of max_iter
// If we wanted a really high number of max iteration we should decide on a
// bigger period
// I actually thought it might work and then found it on this Wikipedia article
// https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set#Periodicity_checking
#define PERIOD 100

// If GCC extensions are not used
// We run the simple non vectorize version
#ifndef GCC_EXTENSIONS
/**
 * Calculate Mandelbrot set value for a given point.
 *
 * @param x0 The real part of the complex number.
 * @param y0 The imaginary part of the complex number.
 * @param max_iter Maximum number of iterations.
 * @return Number of iterations to reach escape radius.
 */
int calculate_mandelbrot_pixel(float x0, float y0, const int max_iter);

#else

// I had also made a version specific for NEON instruction but I decided to
// not include that since it wasn't that relevant and I believe using GCC
// GCC EXTENSIONS is a cleaner and more elegant way to do it

// Vector of four single-precision floats
typedef float v4sf __attribute__((vector_size(4 * sizeof(float))));
// Vector of four int
typedef int v4si __attribute__((vector_size(4 * sizeof(int))));

v4si calculate_mandelbrot_pixel(v4sf x0, v4sf y0, const int max_iter);
#endif
