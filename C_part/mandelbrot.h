// Jacopo Zacchinga SM3201293
#pragma once
#include <math.h>

// If we want to vectorize it we can do it with GCC_EXTENSIONS by having
// GCC_EXTENSIONS defined
#ifdef GCC_EXTENSIONS

// I had also made a version specific for NEON instruction but I decided to not
// include that since it wasn't that relevant and I believe using GCC GCC
// EXTENSIONS is a cleaner and more elegant way to do it

// Vector of four single-precision floats
typedef float v4sf __attribute__((vector_size(4 * sizeof(float))));
// Vector of four single-precision int
typedef int v4si __attribute__((vector_size(4 * sizeof(int))));

v4si mandelbrot_point_calc(v4sf x0, v4sf y0, const int max_iter);

#else // We run the simple non vectorize version
/**
 * Calculate Mandelbrot set value for a given point.
 *
 * @param x0 The real part of the complex number.
 * @param y0 The imaginary part of the complex number.
 * @param max_iter Maximum number of iterations.
 * @return Number of iterations to reach escape radius.
 */
int mandelbrot_point_calc(float x0, float y0, const int max_iter);
#endif
