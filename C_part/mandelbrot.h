// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
#pragma once

// Compute if a point is inside or not of the Mandelbrot set
#ifdef SIMD
typedef float v8df __attribute__((vector_size(8 * sizeof(float))));
typedef int v8si __attribute__((vector_size(8 * sizeof(int))));
v8df mandelbrot_point_calc(v8df _x0, v8df y0, const int max_iter);

#elif __ARM_NEON
#include <arm_neon.h>
float32x4_t mandelbrot_point_calc(float32x4_t x0, float32x4_t y0,
                                  const int max_iter);
#else
int mandelbrot_point_calc(float x0, float y0, const int max_iter);
#endif
