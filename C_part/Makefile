# CC = gcc
# # CC = clang
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
# 	CFLAGS += -O2
# endif
#
# # Source files
# SRCS = $(wildcard *.c)
# OBJS = $(SRCS:.c=.o)
#
# # Targets lib and main
# all: $(LIB) main
#
# $(LIB): $(OBJS)
# ifeq ($(SHARED),1)
# 	$(CC) -shared -o $@ $^
# else
# 	ar rcs $@ $^
# endif
#
# main: main.c $(LIB)
# 	$(CC) $(CFLAGS) -o $@ $< -L. -libmandelbrot
#
# %.o: %.c
# 	@$(CC) $(CFLAGS) -c $< -o $@
#
# clean:
# 	rm -f *.o main
#
# # Delete everything, might be useful
# clean_all:
# 	rm -f *.o $(LIB) $(LIB_NAME).a $(LIB_NAME).so main
# 	rm -rf main.dSYM
#

CC = gcc
# CFLAGS = -O3 -fopenmp -march=native -std=c17 -Wall -pedantic
CFLAGS = -O3 -g -fopenmp -march=native -std=c17 -Wall -pedantic -fopt-info-missed 
SIMD_FLAGS = -DSIMD
HEADERS = ${wildcard *.h}
OBJECTS = main.o pgm.o mandelbrot.o 

all: mandelbrot

mandelbrot: ${OBJECTS} ${HEADERS}
	${CC} ${CFLAGS} -o $@ ${OBJECTS}

simd: CFLAGS += ${SIMD_FLAGS}
simd: mandelbrot

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $<

.PHONY: clean

clean:
	rm -f *.o mandelbrot *.pgm
