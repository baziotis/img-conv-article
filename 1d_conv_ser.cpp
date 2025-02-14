#include <stdlib.h>
#include <string.h>

#include "img_lib.cpp"

// 1x3 blur kernel
static float kernel[3] = {0.33f, 0.33f, 0.33f};

static int counter = 0;

void conv(Image img_in, Image img_out) {
  assert(img_in.height == img_out.height);
  assert(img_in.width == img_in.width);

  int width = img_in.width;
  int height = img_in.height;

  uint8_t *in_pixels = img_in.pixels;
  uint8_t *out_pixels = img_out.pixels;

  for (int i = 1; i < (width * height)-1; ++i) {
    uint8_t left   = in_pixels[i-1];
    uint8_t center = in_pixels[i];
    uint8_t right  = in_pixels[i+1];

    float sum = kernel[0]*left + kernel[1]*center + kernel[2]*right;
    uint8_t sum_uint8 = (uint8_t)sum;
    out_pixels[i] = sum_uint8;
  }
}

int main() {
  const char *filepath = "./images_in/window.pgm";
  Image img = read_pgm(filepath);

  uint8_t *pixels_out = (uint8_t *)malloc(img.width * img.height);
  Image blurred = {
      .width = img.width, .height = img.height, .pixels = pixels_out};

  for (int it = 0; it < 15; ++it) {
    conv(img, blurred);
    uint8_t *tmp = img.pixels;
    img.pixels = blurred.pixels;
    blurred.pixels = tmp;
  }
  write_pgm(blurred, "./images_out/window_1d.pgm");

  return 0;
}