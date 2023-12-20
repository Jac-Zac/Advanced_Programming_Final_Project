// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
#pragma once

// Compute if a point is inside or not of the Mandelbrot set
#ifdef SIMD
typedef float v4df __attribute__((vector_size(8 * sizeof(float))));

void mandelbrot_point_calc(v4df x0, v4df y0, char **pixel,
                           char **pixel_symmetri, const int max_iter,
                           const int MAX_COLOR, const float log_max_iter);
#else
int mandelbrot_point_calc(float x0, float y0, const int max_iter);
#endif
