// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
#pragma once

// Compute if a point is inside or not of the Mandelbrot set
#ifdef SIMD
// #elif __ARM_NEON
#include <arm_neon.h>
float32x4_t mandelbrot_point_calc(float32x4_t x0, float32x4_t y0,
                                  const int max_iter);
#else
int mandelbrot_point_calc(float x0, float y0, const int max_iter);
#endif
