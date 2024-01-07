// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
//
//
//
#pragma once
#include <stdio.h>
#include <stdlib.h>

struct _netpbm_image {
  int width;
  int height;
  int offset;
  int size;
  char *data __attribute__((
      aligned(16))); // Assuming `data` is the image pixel storage
  FILE *fd;
};

typedef struct _netpbm_image netpbm;
typedef struct _netpbm_image *netpbm_ptr;

// Set info about the image and map the file
int set_image_info(netpbm_ptr img_ptr, const char *file_name, const int n_rows);

// Create image computing the color for every point
int create_image(const char *file_name, const int max_iter, const int n_rows);

// Get the pixel at a specific position
char *pixel_at(netpbm_ptr img_ptr, int x, int y);

int close_image(netpbm_ptr img_ptr);
