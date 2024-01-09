// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
//
//
//
#pragma once
#include <stdio.h>
#include <stdlib.h>

#define WIDTH_SCALE 1.5
#define MAX_COLOR 255

struct _netpbm_image {
  int width;
  int height;
  int offset;
  int size;
  char *data;
  FILE *fd;
};

typedef enum {
  SUCCESS = 0,
  ERROR_NULL_POINTER = -1,
  ERROR_FILE_OPEN = -2,
  ERROR_FILE_WRITE = -3,
  ERROR_FILE_TRUNCATE = -4,
  ERROR_MMAP = -5,
  ERROR_FILE_CLOSE = -6
} ErrorCode;

typedef struct _netpbm_image netpbm;
typedef struct _netpbm_image *netpbm_ptr;

// Set info about the image and map the file
int set_image_info(netpbm_ptr img_ptr, const char *file_name, const int n_rows);

// Create image computing the color for every point
int create_image(const char *file_name, const int max_iter, const int n_rows);

// Get the pixel at a specific position
char *pixel_at(netpbm_ptr img_ptr, int x, int y);

int close_image(netpbm_ptr img_ptr);
