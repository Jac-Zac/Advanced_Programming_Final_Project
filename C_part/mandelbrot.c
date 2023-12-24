// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
#include "mandelbrot.h"
#include <math.h>

// #define RADIUS 2
#define RADIUS_SQUARED 4

// The period is chosen in relation to the number of max_iter
// If we wanted a really high number of max iteration we should decide on a
// bigger period
#define PERIOD 100

// define macros if they help you look it up online

// float Mandelbrot_calculator(unite nm_iterations)

// Iterative implementation of mandelbrot_set -> less stack-frames

#ifdef SIMD
v8df mandelbrot_point_calc(v8df x0, v8df y0, const int max_iter) {
  // int flag = 0;
  // // Check if the point is in the set
  // v8df q = (x0 - 0.25) * (x0 - 0.25) + y0 * y0;
  // v8df mask = (q * (q + (x0 - 0.25)) <= 0.25 * y0 * y0);
  //
  // for (int i = 0; i < 8; i++) {
  //   // is inside the bulb
  //   if (mask[i]) {
  //     flag += 1;
  //   }
  // }
  //
  // if (flag == 8) {
  //   return (v8df){0};
  // }
  // else {
  // It should be v8d
  // vector of 8 zeros
  v8df x = {0.0};
  v8df y = {0.0};
  v8df x2 = {0.0};
  v8df y2 = {0.0};
  v8df result = {0.0};
  v8di mask;
  // int period = 0.0;

  for (int i = 0; i < max_iter; i++) {
    // testing then I want to create a mask to add to the result
    mask = (x2 + y2) < RADIUS_SQUARED;

    y = 2 * x * y + y0;
    x = x2 - y2 + x0;
    x2 = x * x;
    y2 = y * y;

    // period++;
    result += mask;
  }
  return result;
  // }
}
#elif __ARM_NEON
float32x4_t mandelbrot_point_calc(float32x4_t x0, float32x4_t y0,
                                  const int max_iter) {
  // int flag = 0;
  // float32x4_t q = vmlaq_f32(vmulq_f32(x0, x0), y0, y0);
  // uint32x4_t mask = vcleq_f32(q, vmulq_f32(vaddq_f32(q, x0), y0));
  //
  // for (int i = 0; i < 4; i++) {
  //   if (vgetq_lane_u32(mask, i)) {
  //     flag += 1;
  //   }
  // }
  //
  // if (flag == 4) {
  //   return vdupq_n_f32(0);
  // } else {
  float32x4_t x = vdupq_n_f32(0);
  float32x4_t y = vdupq_n_f32(0);
  float32x4_t x2 = vdupq_n_f32(0);
  float32x4_t y2 = vdupq_n_f32(0);
  uint32x4_t result = vdupq_n_u32(0);
  float32x4_t old_position_real = vdupq_n_f32(0);
  float32x4_t old_position_imag = vdupq_n_f32(0);
  uint32x4_t mask;
  uint32x4_t period_mask;
  int period = 0;

  for (int i = 0; i < max_iter; i++) {
    mask = vcltq_f32(vaddq_f32(x2, y2), vdupq_n_f32(RADIUS_SQUARED));

    // Break out of the loop if all points are outside the radius
    if (vmaxvq_u32(mask) == 0) {
      break;
    }

    float32x4_t temp_x = vsubq_f32(x2, y2);
    float32x4_t temp_y = vmulq_f32(x, y);
    temp_y = vaddq_f32(temp_y, temp_y); // equivalent to 2*x*y
    x = vaddq_f32(temp_x, x0);
    y = vaddq_f32(temp_y, y0);
    x2 = vmulq_f32(x, x);
    y2 = vmulq_f32(y, y);

    period_mask = vorrq_u32(vceqq_f32(old_position_real, x),
                            vceqq_f32(old_position_imag, y));

    // Create a mask where all bits are set
    uint32x4_t all_set_mask = vdupq_n_u32(~0);

    // Check if all elements in period_mask are set to 1
    if (vminvq_u32(veorq_u32(period_mask, all_set_mask)) == 0) {
      return vdupq_n_f32(max_iter);
    }

    if (period % PERIOD == 0) {
      old_position_real = x;
      old_position_imag = y;
    }

    // Increment the result for points that are still within the radius
    result = vaddq_u32(result, vandq_u32(vdupq_n_u32(1), mask));
    period++;
  }
  return vcvtq_f32_u32(result);
  // }
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
    float x = 0.0;
    float y = 0.0;
    float x_squared = 0.0;
    float y_squared = 0.0;

    // periodicity checking
    int period = 0;
    float old_position_real = 0.0;
    float old_position_imag = 0.0;
    //
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
