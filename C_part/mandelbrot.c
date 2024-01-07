// Jacopo Zacchinga SM3201293
#include "mandelbrot.h"

// #define RADIUS 2
#define RADIUS_SQUARED 4

// The period is chosen in relation to the number of max_iter
// If we wanted a really high number of max iteration we should decide on a
// bigger period
#define PERIOD 100

#ifdef GCC_EXTENSIONS

// The mandelbrot_point_calc function rewritten to use GCC vector types
v4si mandelbrot_point_calc(v4sf x0, v4sf y0, const int max_iter) {
  // Early Bailout Test for Main Cardioid and Period-2 Bulb
  v4sf q = ((x0 - 0.25f) * (x0 - 0.25f)) + (y0 * y0);
  v4si cardioid_mask = (v4si)((q * (q + (x0 - 0.25f))) <= (0.25f * y0 * y0));

  // If equal to 1 escape
  if (cardioid_mask[0] && cardioid_mask[1] && cardioid_mask[2] &&
      cardioid_mask[3]) {
    return (v4si){max_iter, max_iter, max_iter, max_iter};
  }

  // Initialization
  v4sf x = {0}, y = {0}, x2 = {0}, y2 = {0};
  v4sf old_position_real = {0}, old_position_imag = {0};

  v4sf radius_squared = {RADIUS_SQUARED, RADIUS_SQUARED, RADIUS_SQUARED,
                         RADIUS_SQUARED};

  // Result and Masks
  v4si result = {0};
  v4si mask = {-1}, period_mask_real = {0}, period_mask_imag = {0};

  int period = 0;

  int i = 0;
  while (i < max_iter) {
    // Update mask: true (-1) for points within the escape radius
    mask = -(v4si)((x2 + y2) < radius_squared);

    // Break if all points have escaped
    if (mask[0] == 0 && mask[1] == 0 && mask[2] == 0 && mask[3] == 0) {
      break;
    }

    // Mandelbrot iteration
    y = (2.0f * x * y) + y0;
    x = x2 - y2 + x0;

    x2 = x * x;
    y2 = y * y;

    // Periodicity Check (uncommented and enabled)
    period_mask_real = (v4si)(old_position_real == x);
    period_mask_imag = (v4si)(old_position_imag == y);

    // Check if all elements in period_mask are set to 1
    if ((period_mask_real[0] != 0 && period_mask_real[1] != 0 &&
         period_mask_real[2] != 0 && period_mask_real[3] != 0) &&
        (period_mask_imag[0] != 0 && period_mask_imag[1] != 0 &&
         period_mask_imag[2] != 0 && period_mask_imag[3] != 0)) {
      return (v4si){max_iter, max_iter, max_iter, max_iter};
    }

    // Update old positions every period
    if (period % PERIOD == 0) {
      old_position_real = x;
      old_position_imag = y;
    }
    period++;

    // Increment result for points that have not escaped
    result += mask;
    i++;
  }

  return result;
}

#else

// Iterative implementation of mandelbrot_set -> less stack-frames
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
    int i = 0;
    while (i < max_iter) {
      // check if radius distance is greater or equal to the radius
      if ((x_squared + y_squared) >= RADIUS_SQUARED) {
        return i;
      }

      // slow implementation
      // float xtemp = x * x - y * y + x0;
      // y = 2 * x * y + y0;
      // x = xtemp;

      // Find the position
      y = 2 * x * y + y0;
      x = x_squared - y_squared + x0;

      // update x_squared and y_squared for the next iteration
      x_squared = x * x;
      y_squared = y * y;

      // trade of memory for compute
      if ((old_position_real == x && old_position_imag == y)) {
        // set to max for the color plotting
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

      period++;
      i++;
    }
    return max_iter;
  }
}
#endif
