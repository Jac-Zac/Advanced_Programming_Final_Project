// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
#pragma once

// Compute if a point is inside or not of the Mandelbrot set
#ifdef GCC_EXTENSIONS

// Vector of four single-precision floats
typedef float v4sf __attribute__((vector_size(4 * sizeof(float))));
// Vector of four single-precision int
typedef int v4si __attribute__((vector_size(4 * sizeof(int))));

v4si mandelbrot_point_calc(v4sf x0, v4sf y0, const int max_iter);

// #elif __ARM_NEON
// #include <arm_neon.h>
// uint32x4_t mandelbrot_point_calc(float32x4_t x0, float32x4_t y0,
//                                  const int max_iter);

#else
/**
 * Calculate Mandelbrot set value for a given point.
 *
 * @param real The real part of the complex number.
 * @param imag The imaginary part of the complex number.
 * @param max_iter Maximum number of iterations.
 * @return Number of iterations to reach escape radius.
 */
int mandelbrot_point_calc(float x0, float y0, const int max_iter);
#endif
