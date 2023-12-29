# Advanced Programming Exam

## Mandelbrot Fractal Generator in C

#### Overview

This project involves creating a C program to generate the Mandelbrot fractal as a grayscale image. It is the first part of a two-part series in advanced and parallel programming.

- ##### Building the Project

  Ensure you have a C compiler installed (e.g., GCC).
  Use the provided Makefile for compilation.

  It can be compiled with

  - `Make` : for the non SIMD version
  - `Make simd` : for the non SIMD version

- ##### Running the Program

  ###### Command to run:

  ```bash
  ./mandelbrot [output filename] [max iterations] [vertical resolution]`
  ```

  > Example: `./mandelbrot fractal.pgm 1000 1000`

  You can also directly pipe it into the kitty terminal if you have that by doing something like this: `./mandelbrot fractal.pgm 1000 1000 && icat test.pgm`
  `

- ##### Implementation Details

  The program calculates the Mandelbrot set and outputs an image in the netpbm grayscale format.
  The horizontal resolution of the image is set to 1.5 times the vertical resolution.
  It has a SIMD implementation and uses multithreading.

- ##### Dependencies

  No external libraries are required.

- ##### Notes

  Detailed mathematical background and algorithmic approach are outlined in the accompanying documentation (project_c.pdf).

# Python part:

Install dependencies

You can run the tests by:

```bash
pytest
```
