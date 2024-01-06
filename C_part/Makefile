CC = gcc
DEBUG ?= 0

# Compiler flags
ifeq ($(DEBUG),1)
	CFLAGS += -g -fopenmp -Wall -Wextra -pedantic -O0 -fopt-info-missed -std=c17
else
	CFLAGS += -O3 -fopenmp -march=native -std=c17
endif

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
	rm -f *.o mandelbrot

.PHONY: clean_all
clean_all:
	rm -f *.o *.pgm mandelbrot

# CC = gcc
# VECTORIZATION_FLAGS = -DGCC_EXTENSIONS
#
# HEADERS = ${wildcard *.h}
# OBJECTS = main.o pgm.o mandelbrot.o 
#
# all: mandelbrot
#
# mandelbrot: ${OBJECTS} ${HEADERS}
# 	${CC} ${CFLAGS} -o $@ ${OBJECTS}
#
# vectorized: CFLAGS += ${VECTORIZATION_FLAGS}
# vectorized: mandelbrot
#
# %.o: %.c ${HEADERS}
# 	${CC} ${CFLAGS} -c $<
#
# .PHONY: clean
#
# clean:
# 	rm -f *.o mandelbrot *.pgm
