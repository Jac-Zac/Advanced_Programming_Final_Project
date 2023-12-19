// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
#pragma once
typedef double v4df __attribute__((vector_size(32)));

// Compute if a point is inside or not of the Mandelbrot set
#ifdef SIMD
void mandelbrot_point_calc(v4df x0, v4df y0, char **pixel,
                           char **pixel_symmetric, const int max_iter,
                           const int MAX_COLOR, const double log_max_iter);
#else
void mandelbrot_point_calc(double x0, double y0, char *pixel,
                           char *pixel_symmetric, const int max_iter,
                           const int MAX_COLOR, const double log_max_iter);
#endif
