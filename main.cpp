#include <stdlib.h>
#include <string.h>

#include "img_lib.cpp"

// 3x3 blur kernel
float blur_kernel[3][3] = {{1.0f / 9, 1.0f / 9, 1.0f / 9},
                           {1.0f / 9, 1.0f / 9, 1.0f / 9},
                           {1.0f / 9, 1.0f / 9, 1.0f / 9}};

void box_blur(Image img_in, Image img_out) {
  assert(img_in.height == img_out.height);
  assert(img_in.width == img_in.width);

  int width = img_in.width;
  int height = img_in.height;

  for (int y = 1; y < height - 1; y++) {
    for (int x = 1; x < width - 1; x++) {
      float sum = 0.0f;

      for (int ky = -1; ky <= 1; ky++) {
        for (int kx = -1; kx <= 1; kx++) {
          uint8_t pixel = img_in.pixels[(y + ky) * width + (x + kx)];
          float kernel_val = blur_kernel[ky + 1][kx + 1];
          sum += pixel * kernel_val;
        }
      }

      img_out.pixels[y * width + x] = (uint8_t)sum;
    }
  }
}

int main() {
  const char *filepath = "./images/window.pgm";
  Image img = read_pgm(filepath);

  uint8_t *pixels_out = (uint8_t *)malloc(img.width * img.height);
  Image blurred = {
      .width = img.width, .height = img.height, .pixels = pixels_out};

  for (int it = 0; it < 15; ++it) {
    box_blur(img, blurred);
    uint8_t *tmp = img.pixels;
    img.pixels = blurred.pixels;
    blurred.pixels = tmp;
  }
  write_pgm(blurred, "./images/window_blurred.pgm");

  return 0;
}