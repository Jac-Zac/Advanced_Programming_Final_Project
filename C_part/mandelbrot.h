// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.

#pragma once

typedef double v4df __attribute__((vector_size(4 * sizeof(double))));

// Compute if a point is inside or not of the Mandelbrot set
#ifdef SIMD
v4df mandelbrot_point_calc(v4df x0, v4df y0, const int max_iter);
#else
int mandelbrot_point_calc(double x0, double y0, const int max_iter);
#endif
