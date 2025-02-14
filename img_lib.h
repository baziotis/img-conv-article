#ifndef STEF_img_lib_H
#define STEF_img_lib_H

#include <stdint.h>

struct Image {
  int width, height;
  uint8_t *pixels;
};

struct RGBSet {
  Image r, g, b;
};

#endif