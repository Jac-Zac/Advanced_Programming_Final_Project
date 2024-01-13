# Jacopo Zacchinga SM3201293
CC = gcc
OPTIMIZATION = -O3 # Allow user to choose the optimization they want default is O3

# Compiler flags
CFLAGS = -fopenmp -std=c17 $(OPTIMIZATION)
ifdef DEBUG
	CFLAGS += -g -Wall -Wextra -pedantic -fopt-info-missed -DDEBUG
else
	CFLAGS += -march=native
endif

# GCC extension vectorization
VECTORIZATION_FLAGS = -DGCC_EXTENSIONS

# Source files
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
HEADERS = $(wildcard *.h)

# Target
all: mandelbrot

mandelbrot: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Added a phony target for 'vectorized' to ensure it's not confused with a file name.
.PHONY: vectorized
vectorized: CFLAGS += $(VECTORIZATION_FLAGS)
vectorized: mandelbrot

# The rule for object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# clean_all and clean
.PHONY: clean
clean:
	rm -f *.o

.PHONY: clean_all
clean_all:
	rm -f *.o *.pgm mandelbrot
