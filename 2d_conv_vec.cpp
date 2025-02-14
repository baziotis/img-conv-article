#include "2d_conv_vec_base.cpp"

int main() {
  const char *filepath = "./images_in/window.pgm";
  Image img = read_pgm(filepath);

  uint8_t *pixels_in_u8 = img.pixels;

  float *pixels_in = (float *)malloc(img.width * img.height * sizeof(float));
  for (int i = 0; i < img.width * img.height; ++i) {
    pixels_in[i] = (float)pixels_in_u8[i];
  }
  float *pixels_out = (float *)malloc(img.width * img.height * sizeof(float));

  conv_2d(pixels_in, pixels_out, img.width, img.height);

  uint8_t *pixels_out_u8 =
      (uint8_t *)malloc(img.width * img.height * sizeof(uint8_t));
  for (int i = 0; i < img.width * img.height; ++i) {
    pixels_out_u8[i] = (uint8_t)pixels_out[i];
  }

  Image blurred = {
      .width = img.width, .height = img.height, .pixels = pixels_out_u8};

  write_pgm(blurred, "./images_out/window_2d.pgm");

  return 0;
}