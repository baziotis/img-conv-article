#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "img_lib.h"

Image read_pgm(const char *filepath) {
  FILE *file = fopen(filepath, "rb");
  assert(file);

  // Read magic number
  char magic[3];
  assert(fgets(magic, sizeof(magic), file));
  assert(magic[0] == 'P' && magic[1] == '5');

  // Read image dimensions and maximum pixel value
  int width, height, max_val;
  assert(fscanf(file, "%d %d %d", &width, &height, &max_val) == 3);
  assert(max_val == 255);

  // Skip the single whitespace character after maxVal
  fgetc(file);

  // Read pixels
  uint8_t *pixels = (uint8_t *)malloc(width * height);
  size_t bytes_read = fread(pixels, 1, width * height, file);
  assert(bytes_read == (size_t)(width * height));

  fclose(file);

  Image img = {.width = width, .height = height, .pixels = pixels};
  return img;
}

Image read_ppm(const char *filepath) {
  FILE *file = fopen(filepath, "rb");
  assert(file);

  // Read magic number
  char magic[3];
  assert(fgets(magic, sizeof(magic), file));
  assert(magic[0] == 'P' && magic[1] == '6');

  // Read image dimensions and maximum pixel value
  int width, height, max_val;
  assert(fscanf(file, "%d %d %d", &width, &height, &max_val) == 3);
  assert(max_val == 255);

  // Skip the single whitespace character after maxVal
  fgetc(file);

  // Read pixels
  int total_bytes = width * height * 3;
  uint8_t *pixels = (uint8_t *)malloc(total_bytes);
  size_t bytes_read = fread(pixels, 1, total_bytes, file);
  assert(bytes_read == (size_t)(total_bytes));

  fclose(file);

  Image img = {.width = width, .height = height, .pixels = pixels};
  return img;
}

void write_img(Image img, const char *out_path, bool is_rgb) {
  FILE *file = fopen(out_path, "wb");
  assert(file);

  int len = img.width * img.height;
  if (is_rgb) {
    fprintf(file, "P6\n%d %d\n255\n", img.width, img.height);
    len *= 3;
  } else {
    fprintf(file, "P5\n%d %d\n255\n", img.width, img.height);
  }

  // Write the pixel data
  size_t bytes_written = fwrite(img.pixels, 1, len, file);
  assert(bytes_written == (size_t)(len));

  fclose(file);  
}

void write_pgm(Image img, const char *out_path) {
  write_img(img, out_path, false);
}

void write_ppm(Image img, const char *out_path) {
  write_img(img, out_path, true);
}

RGBSet split_colors(Image img) {
  int width = img.width;
  int height = img.height;

  int len = width * height;

  RGBSet res;
  res.r.height = res.g.height = res.b.height = height;
  res.r.width = res.g.width = res.b.width = width;

  res.r.pixels = (uint8_t *)malloc(width * height * sizeof(uint8_t));
  res.g.pixels = (uint8_t *)malloc(width * height * sizeof(uint8_t));
  res.b.pixels = (uint8_t *)malloc(width * height * sizeof(uint8_t));

  for (int i = 0; i < len; ++i) {
    int num_channels = 3;
    for (int j = 0; j < num_channels; ++j) {
      res.r.pixels[i] = img.pixels[i*num_channels];
      res.g.pixels[i] = img.pixels[i*num_channels+1];
      res.b.pixels[i] = img.pixels[i*num_channels+2];
    }
  }

  return res;
}

Image combine_colors(RGBSet rgb) {
  int width = rgb.r.width;
  int height = rgb.r.height;

  int len = width * height;

  Image res;
  res.height = height;
  res.width = width;
  int num_channels = 3;
  res.pixels = (uint8_t *)malloc(width * height * num_channels * sizeof(uint8_t));

  for (int i = 0; i < len; ++i) {
    res.pixels[i*num_channels] = rgb.r.pixels[i];
    res.pixels[i*num_channels+1] = rgb.g.pixels[i];
    res.pixels[i*num_channels+2] = rgb.b.pixels[i];
  }

  return res;
}