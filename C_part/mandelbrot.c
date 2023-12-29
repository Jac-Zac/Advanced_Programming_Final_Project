// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
#include "mandelbrot.h"
#include <math.h>

// #define RADIUS 2
#define RADIUS_SQUARED 4.0f

// The period is chosen in relation to the number of max_iter
// If we wanted a really high number of max iteration we should decide on a
// bigger period
#define PERIOD 100

// define macros if they help you look it up online

// float Mandelbrot_calculator(unite nm_iterations)

// Iterative implementation of mandelbrot_set -> less stack-frames

#ifdef SIMD
// The mandelbrot_point_calc function rewritten to use GCC vector types
v4sf mandelbrot_point_calc(v4sf x0, v4sf y0, const int max_iter) {
  // Early Bailout Test for Main Cardioid and Period-2 Bulb
  v4sf q = ((x0 - 0.25f) * (x0 - 0.25f)) + (y0 * y0);
  v4si cardioid_mask = (v4si)((q * (q + (x0 - 0.25f))) <= (0.25f * y0 * y0));
  if (cardioid_mask[0] && cardioid_mask[1] && cardioid_mask[2] &&
      cardioid_mask[3]) {
    return (v4sf){max_iter, max_iter, max_iter, max_iter};
  }

  // Initialization
  v4sf x = {0, 0, 0, 0}, y = {0, 0, 0, 0};
  v4sf x2 = x * x, y2 = y * y;
  v4sf result = {0, 0, 0, 0};

  v4sf old_position_real = {0, 0, 0, 0}, old_position_imag = {0, 0, 0, 0};
  int period = 0;

  for (int i = 0; i < max_iter; i++) {
    y = (2.0f * x * y) + y0;
    x = x2 - y2 + x0;

    x2 = x * x;
    y2 = y * y;

    v4sf mask = (v4sf)(x2 + y2 < (v4sf){RADIUS_SQUARED, RADIUS_SQUARED,
                                        RADIUS_SQUARED, RADIUS_SQUARED});

    // Debugging: Print the current values
    // printf("Iteration %d: x = [%f, %f, %f, %f], y = [%f, %f, %f, %f]\n", i,
    //        x[0], x[1], x[2], x[3], y[0], y[1], y[2], y[3]);
    // printf("x2 + y2 = [%f, %f, %f, %f], Mask = [%f, %f, %f, %f]\n",
    //        x2[0] + y2[0], x2[1] + y2[1], x2[2] + y2[2], x2[3] + y2[3],
    //        mask[0], mask[1], mask[2], mask[3]);

    // Limit the growth of x and y to prevent overflow
    if (mask[0] == 0.0f && mask[1] == 0.0f && mask[2] == 0.0f &&
        mask[3] == 0.0f) {
      break;
    }

    // Increment result only for points within the radius
    result += (v4sf){mask[0], mask[1], mask[2], mask[3]};
  }

  return result;
}

// Periodicity Check (commented out for now)
/*
v4si period_mask_real = (v4si)(old_position_real == x);
v4si period_mask_imag = (v4si)(old_position_imag == y);

if (period_mask_real[0] && period_mask_real[1] && period_mask_real[2] &&
period_mask_real[3] && period_mask_imag[0] && period_mask_imag[1] &&
period_mask_imag[2] && period_mask_imag[3]) { return (v4sf){max_iter,
max_iter, max_iter, max_iter};
}
if (period % PERIOD == 0) {
  old_position_real = x;
  old_position_imag = y;
}
*/

#elif __ARM_NEON
float32x4_t mandelbrot_point_calc(float32x4_t x0, float32x4_t y0,
                                  const int max_iter) {
  // Check if inside the bulb
  // Wikipedia formula
  float32x4_t q = vaddq_f32(vmulq_f32(vsubq_f32(x0, vdupq_n_f32(0.25)),
                                      vsubq_f32(x0, vdupq_n_f32(0.25))),
                            vmulq_f32(y0, y0));

  uint32x4_t mask =
      vcleq_f32(vmulq_f32(q, vaddq_f32(q, vsubq_f32(x0, vdupq_n_f32(0.25)))),
                vmulq_f32(vmulq_n_f32(y0, 0.25), y0));

  if (vmaxvq_u32(mask) == 0xFFFFFFFF) {
    return vdupq_n_f32(max_iter);
  } else {
    // Initialization
    float32x4_t x = vdupq_n_f32(0), y = vdupq_n_f32(0);
    float32x4_t x2 = vdupq_n_f32(0), y2 = vdupq_n_f32(0);

    uint32x4_t result = vdupq_n_u32(0);

    float32x4_t old_position_real = vdupq_n_f32(0),
                old_position_imag = vdupq_n_f32(0);

    uint32x4_t mask, period_mask_real, period_mask_imag;

    // // Create a mask where all bits are set
    int period = 0;

    for (int i = 0; i < max_iter; i++) {
      mask = vcltq_f32(vaddq_f32(x2, y2), vdupq_n_f32(RADIUS_SQUARED));

      // // Break out of the loop if all points are outside the radius
      if (vmaxvq_u32(mask) == 0) {
        break;
      }

      // y = 2 * x * y + y0;
      // x = x_squared - y_squared + x0;
      y = vmlaq_f32(y0, vmulq_n_f32(y, 2.0), x);
      x = vaddq_f32(vsubq_f32(x2, y2), x0);

      // Square the numbers
      x2 = vmulq_f32(x, x);
      y2 = vmulq_f32(y, y);

      // Check if all elements in period_mask are set to 1
      period_mask_real = vceqq_f32(old_position_real, x);
      period_mask_imag = vceqq_f32(old_position_imag, y);

      // Check if any elements in period_mask are set to 0
      if ((vminvq_u32(period_mask_real) != 0) &&
          (vminvq_u32(period_mask_imag) != 0)) {
        return vdupq_n_f32(max_iter);
      }

      // Update every period
      if (period % PERIOD == 0) {
        old_position_real = x;
        old_position_imag = y;
      }

      // Increment the result for points that are still within the radius
      result = vaddq_u32(result, vandq_u32(vdupq_n_u32(1), mask));
      period++;
    }
    return vcvtq_f32_u32(result);
  }
}
#else

// Clean up the number of arguments passed
int mandelbrot_point_calc(float x0, float y0, const int max_iter) {
  // variable to check if inside bulb
  // is inside the bulb
  // Link to the Wikimedia article
  float q = (x0 - 0.25) * (x0 - 0.25) + y0 * y0;
  if (q * (q + (x0 - 0.25)) <= 0.25 * y0 * y0) {
    return max_iter;
  } else {
    // This will be computed thanks to starting point perhaps two variables
    // float complex c = x + y * i;
    // float complex z = c;

    // use the bailout method to keep in mind the derivative
    float x = 0.0, y = 0.0, x_squared = 0.0, y_squared = 0.0;

    // periodicity checking
    int period = 0;
    float old_position_real = 0.0, old_position_imag = 0.0;

    // implement the function iteratively
    unsigned int i = 0;
    while (i < max_iter) {
      // check if radius distance is greater or equal to the radius
      if ((x_squared + y_squared) >= RADIUS_SQUARED) {
        return i;
      }

      // slow implementation
      // float xtemp = x * x - y * y + x0;
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
        // You could simply I believe
        break;
      }

      // 0 for read, 3 for temporal locality
      // __builtin_prefetch(&old_position_real, 0, 3);
      // __builtin_prefetch(&old_position_imag, 0, 3);
      // __builtin_prefetch(&x, 0, 3);
      // __builtin_prefetch(&y, 0, 3);
      if (period % PERIOD == 0) {
        // old_position_real = x; is slowwww
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
}
#endif
