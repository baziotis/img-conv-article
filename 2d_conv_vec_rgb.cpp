#include "2d_conv_vec_base.cpp"

int main() {
  const char *filepath = "./images_in/window.ppm";
  Image img = read_ppm(filepath);

  RGBSet rgb = split_colors(img);

  float *pixels_in_r = (float *)malloc(img.width * img.height * sizeof(float));
  float *pixels_in_g = (float *)malloc(img.width * img.height * sizeof(float));
  float *pixels_in_b = (float *)malloc(img.width * img.height * sizeof(float));
  for (int i = 0; i < img.width * img.height; ++i) {
    pixels_in_r[i] = (float)rgb.r.pixels[i];
    pixels_in_g[i] = (float)rgb.g.pixels[i];
    pixels_in_b[i] = (float)rgb.b.pixels[i];
  }
  float *pixels_out_r = (float *)malloc(img.width * img.height * sizeof(float));
  float *pixels_out_g = (float *)malloc(img.width * img.height * sizeof(float));
  float *pixels_out_b = (float *)malloc(img.width * img.height * sizeof(float));

  conv_2d(pixels_in_r, pixels_out_r, img.width, img.height);
  conv_2d(pixels_in_g, pixels_out_g, img.width, img.height);
  conv_2d(pixels_in_b, pixels_out_b, img.width, img.height);

  for (int i = 0; i < img.width * img.height; ++i) {
    rgb.r.pixels[i] = (uint8_t)pixels_out_r[i];
    rgb.g.pixels[i] = (uint8_t)pixels_out_g[i];
    rgb.b.pixels[i] = (uint8_t)pixels_out_b[i];
  }

  Image blurred = combine_colors(rgb);

  write_ppm(blurred, "./images_out/window_blurred.ppm");

  return 0;
}