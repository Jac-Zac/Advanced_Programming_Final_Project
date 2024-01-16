# Advanced Programming Exam

## Mandelbrot Fractal Generator in C

#### Overview

This project involves creating a C program to generate the Mandelbrot fractal as a grayscale image. It is the first part of a two-part series in advanced and parallel programming.

- ##### Building the Project

  Ensure you have a C compiler installed (e.g., GCC).
  Use the provided Makefile for compilation.

###### It can be compiled with

- `Make` : for to get a release version of the code
- `Make vectorized` : for the version that uses GCC extensions
- `Make DEBUG=1`: to run in debug version with also info on the time it took
- `Make OPTIMIZATION=-Ox`: to set what optimization we want to use the default is 3

You can remove anything which is not relevant with `make clean` and `make clean_all` if you also want to remove the generated image

- ##### Running the Program

  ###### Command to run:

  ```bash
  ./mandelbrot [output filename] [max iterations] [vertical resolution]`
  ```

  > Example: `./mandelbrot fractal.pgm 1000 1000`

    <details>
      <summary>More tips</summary>
        You can also directly pipe it into the kitty terminal if you have that by doing something like this:

  ```bash
  ./mandelbrot fractal.pgm 1000 1000 && icat test.pgm
  ```

    </details>

  ##### Those are the possible error codes:

  > That are defined inside [`pgm.h`](C_part/pgm.h)

  ```c
  SUCCESS = 0,
  INVALID_ARGUMENTS = -1,
  INVALID_FILE_EXTENSION = -2,
  INVALID_MAX_ITER = -3,
  INVALID_N_ROWS = -4,
  ERROR_NULL_POINTER = -5,
  ERROR_FILE_OPEN = -6,
  ERROR_FILE_WRITE = -7,
  ERROR_FILE_TRUNCATE = -8,
  ERROR_MMAP = -9,
  ERROR_FILE_CLOSE = -10
  ```

- ##### Implementation Details

  The program calculates the Mandelbrot set and outputs an image in the netpbm grayscale format.
  The horizontal resolution of the image is set to 1.5 times the vertical resolution.
  It has a SIMD implementation and uses multithreading.

- ##### Dependencies

  No external libraries are required.

- ##### Notes

  Detailed mathematical background and algorithmic approach are outlined in the accompanying documentation (project_c.pdf).

## Python part:

#### Overview

This section of the project focuses on _brief description of what the Python part does, e.g., testing, implementing certain functionalities, etc._

##### Running Tests

You can simply navigate to the Python_part directory and run all of the tests with:

```bash
pytest
```

##### Python Files Description

```
.
├── src
│   ├── utils
│   │   ├── __init__.py
│   │   ├── exception.py
│   │   ├── mixins.py
│   │   └── stack.py
│   ├── __init__.py
│   ├── condition.py
│   ├── control_flow.py
│   ├── definition.py
│   ├── expression.py
│   ├── instruction.py
│   ├── sequence.py
│   └── subroutine.py
├── test
│   ├── __init__.py
│   ├── test_comparisons.py
│   ├── test_conditionals.py
│   ├── test_control_flow.py
│   ├── test_definitions.py
│   ├── test_exercise_requirements.py
│   ├── test_print_nop.py
│   ├── test_sequences.py
│   ├── test_subroutine.py
│   └── test_teacher.py
├── instructions.pdf
└── main.py
```

#### Testing New Code

To test new code in the project, follow these steps:

1. **Modify the Code**:

   - Open the [`main.py`](Python_part/main.py) file located in the `Python_part` directory.
   - Make the necessary modifications or additions to the code that you wish to test.

2. **Run the Script**:

   - Ensure you have the required environment set up to run Python scripts. This typically includes having Python installed and any necessary dependencies.
   - Navigate to the directory containing `main.py` using a terminal or command prompt.
   - Execute the script by running:
     ```bash
     ./main.py
     ```
