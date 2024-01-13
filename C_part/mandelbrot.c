// Jacopo Zacchinga SM3201293
#include "mandelbrot.h"

// Running not vectorized version if GCC_EXTENSIONS is not defined
#ifndef GCC_EXTENSIONS
int calculate_mandelbrot_pixel(float x0, float y0, const int max_iter) {
  // Check if the point is inside the bulb to avoid unnecessary computation
  // reasonably a 3x speedup from this Wikipedia article
  // https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set#Cardioid_/_bulb_checking
  float q = (x0 - 0.25) * (x0 - 0.25) + y0 * y0;
  if (q * (q + (x0 - 0.25)) <= 0.25 * y0 * y0) {
    return max_iter;
  } else {
    // Old version involved complex numbers but I decided to avoid those to be
    // more compatible with the vectorized version

    // float complex c = x + y * i;
    // float complex z = c;

    // use the bailout method to keep in mind the derivative
    float x = 0.0, y = 0.0, x_squared = 0.0, y_squared = 0.0;

    // periodicity checking
    int period = 0;
    float old_position_real = 0.0, old_position_imag = 0.0;

    // implement the function iteratively
    // Using while instead of for since we mess with the flow of the loop I
    // believe this lead to less confusion.
    int i = 0;
    while (i < max_iter) {
      // check if radius distance is greater or equal to the radius
      if ((x_squared + y_squared) >= RADIUS_SQUARED) {
        return i;
      }

      // slow implementation of the multiplication
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

      // I tried prefetching bu it didn't really help I believe the compiler
      // already optimizes the hack out of this

      // __builtin_prefetch(&old_position_real);
      // __builtin_prefetch(&old_position_imag);
      // __builtin_prefetch(&x);
      // __builtin_prefetch(&y);
      if (period % PERIOD == 0) {
        old_position_real = x;
        old_position_imag = y;
      }

      period++;
      i++;
    }
    return max_iter;
  }
}

#else

// The calculate_mandelbrot_pixel function rewritten to use GCC vector types
v4si calculate_mandelbrot_pixel(v4sf x0, v4sf y0, const int max_iter) {
  // Early Bailout Test for Main Cardioid and Period-2 Bulb
  v4sf q = ((x0 - 0.25f) * (x0 - 0.25f)) + (y0 * y0);
  v4si cardioid_mask = (v4si)((q * (q + (x0 - 0.25f))) <= (0.25f * y0 * y0));

  // Checking if all of the points are inside then we can escape
  if (cardioid_mask[0] && cardioid_mask[1] && cardioid_mask[2] &&
      cardioid_mask[3]) {
    return (v4si){max_iter, max_iter, max_iter, max_iter};
  }

  // Initialization which implicitly does {0, 0, 0, 0} since we defined v4sf
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
    // Since the return is a bit vector of 1s we get -1 for true values we
    // simply flip make it positive
    mask = -(v4si)((x2 + y2) < radius_squared);

    // Break only if all points have escaped
    if (mask[0] == 0 && mask[1] == 0 && mask[2] == 0 && mask[3] == 0) {
      break;
    }

    // Mandelbrot iteration which implicitly uses vectorized instructions
    y = (2.0f * x * y) + y0;
    x = x2 - y2 + x0;

    x2 = x * x;
    y2 = y * y;

    // Periodicity Check by explicitly telling the compiler those are v4si
    // Similarly to before we need to get the opposite of the result
    period_mask_real = -(v4si)(old_position_real == x);
    period_mask_imag = -(v4si)(old_position_imag == y);

    // Check if all elements in period_mask are set to 1 and thus all equal
    if ((period_mask_real[0] == 1 && period_mask_real[1] == 1 &&
         period_mask_real[2] == 1 && period_mask_real[3] == 1) &&
        (period_mask_imag[0] == 1 && period_mask_imag[1] == 1 &&
         period_mask_imag[2] == 1 && period_mask_imag[3] == 1)) {
      return (v4si){max_iter, max_iter, max_iter, max_iter};
    }

    // Possible branchless version a bit overkill no real performance benefits
    // but I should test it more
    // int condition = (period % PERIOD == 0);
    // old_position_real = (v4sf){condition, condition, condition, condition} *
    // x +
    //                     ((v4sf){1.0f, 1.0f, 1.0f, 1.0f} -
    //                      (v4sf){condition, condition, condition, condition})
    //                      *
    //                         old_position_real;
    // old_position_imag = (v4sf){condition, condition, condition, condition} *
    // y +
    //                     ((v4sf){1.0f, 1.0f, 1.0f, 1.0f} -
    //                      (v4sf){condition, condition, condition, condition})
    //                      *
    //                         old_position_imag;
    // Update old positions every period

    // Better way is to use ternary operator
    int condition = (period % PERIOD == 0); // 0 or 1 based on the condition
    old_position_real = condition ? x : old_position_real;
    old_position_imag = condition ? y : old_position_imag;

    period++;
    // Increment result for points that have not escaped
    result += mask;
    i++;
  }

  return result;
}
#endif
