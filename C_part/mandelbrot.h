// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
#pragma once
typedef float v4df __attribute__((vector_size(4 * sizeof(float))));

// Compute if a point is inside or not of the Mandelbrot set
#ifdef SIMD
v4df mandelbrot_point_calc(v4df x0, v4df y0, const int max_iter);
#else
void mandelbrot_point_calc(float x0, float y0, char *pixel,
                           char *pixel_symmetric, const int max_iter,
                           const int MAX_COLOR, const float log_max_iter);
#endif
