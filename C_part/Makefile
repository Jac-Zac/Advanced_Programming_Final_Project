# CC = gcc
# LIB_NAME = libmandelbrot
# SHARED ?= 0
# DEBUG ?= 0
#
# # Decide if you want the library to be shared or not
# ifeq ($(SHARED),1)
# 	LIB_EXT = .so
# 	CFLAGS += -fPIC
# else
# 	LIB_EXT = .a
# endif
# LIB = $(LIB_NAME)$(LIB_EXT)
#
# # Compiler flags
# ifeq ($(DEBUG),1)
# 	CFLAGS += -g -Wall -Wextra -O0
# else
# 	CFLAGS += -O3 -g -fopenmp -march=native -std=c17 -Wall -pedantic -fopt-info-missed
# endif
#
# VECTORIZATION_FLAGS = -DGCC_EXTENSIONS
#
# # Source files
# SRCS = $(wildcard *.c)
# OBJS = $(SRCS:.c=.o)
# HEADERS = $(wildcard *.h)
#
# # Targets lib and main
# all: $(LIB) mandelbrot
#
# $(LIB): $(OBJS)
# ifeq ($(SHARED),1)
# 	$(CC) -shared -o $@ $^
# else
# 	ar rcs $@ $^
# endif
#
# mandelbrot: $(OBJS) $(HEADERS)
# 	$(CC) $(CFLAGS) -o $@ $(OBJS)
#
# vectorized: CFLAGS += $(VECTORIZATION_FLAGS)
# vectorized: mandelbrot
#
# %.o: %.c $(HEADERS)
# 	@$(CC) $(CFLAGS) -c $< -o $@
#
# clean:
# 	rm -f *.o *.pgm mandelbrot
#
# # Delete everything, might be useful
# clean_all:
# 	rm -f *.o $(LIB) $(LIB_NAME).a $(LIB_NAME).so mandelbrot
# 	rm -rf mandelbrot.dSYM
#
CC = gcc
# CFLAGS = -O3 -fopenmp -march=native -std=c17 -Wall -pedantic
CFLAGS = -O3 -g -fopenmp -march=native -std=c17 -Wall -Wextra -pedantic -fopt-info-missed
VECTORIZATION_FLAGS = -DGCC_EXTENSIONS

HEADERS = ${wildcard *.h}
OBJECTS = main.o pgm.o mandelbrot.o 

all: mandelbrot

mandelbrot: ${OBJECTS} ${HEADERS}
	${CC} ${CFLAGS} -o $@ ${OBJECTS}

vectorized: CFLAGS += ${VECTORIZATION_FLAGS}
vectorized: mandelbrot

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $<

.PHONY: clean

clean:
	rm -f *.o mandelbrot *.pgm
