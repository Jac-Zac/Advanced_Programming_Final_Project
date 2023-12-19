// Copyright (c) 2023 Jacopo Zacchigna. All Rights Reserved.
//
//
//
#pragma once

struct _netpbm_image {
  int width;
  int height;
  int offset;
  int size;
  char *data;
  FILE *fd;
};

typedef struct _netpbm_image netpbm;
typedef struct _netpbm_image *netpbm_ptr;

int set_image_info(netpbm_ptr img_ptr, const char *file_name, const int n_rows);

int create_image(const char *file_name, const int max_iter, const int n_rows);

char *pixel_at(netpbm_ptr img_ptr, int x, int y);

int close_image(netpbm_ptr img_ptr);
