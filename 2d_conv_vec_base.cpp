#include <immintrin.h>
#include <stdlib.h>
#include <string.h>

#include "img_lib.cpp"
#include "print_vec.cpp"

float blur_kernel[3][3] = {{1.0f / 9, 1.0f / 9, 1.0f / 9},
                           {1.0f / 9, 1.0f / 9, 1.0f / 9},
                           {1.0f / 9, 1.0f / 9, 1.0f / 9}};

void conv_1d(float *in_pixels, float *out_pixels, int len, float kernel[3]) {

  int i;
  __m128 kernel_vec[3];

  // Repeat each kernel value in a 4-wide register
  for (i = 0; i < 3; ++i) {
    kernel_vec[i] = _mm_set1_ps(kernel[i]);
  }

  for (i = 1; i < len; i += 4) {
    // Zero accumulator
    __m128 acc = _mm_setzero_ps();

    for (int k = -1; k <= 1; ++k) {
      // Load 4-float data block (unaligned access)
      __m128 data_block = _mm_loadu_ps(in_pixels + i + k);
      // Make the products
      __m128 prod = _mm_mul_ps(kernel_vec[k + 1], data_block);

      // Accumulate the 4 parallel values
      acc = _mm_add_ps(acc, prod);
    }

    // Store. Note we go +4 every time.
    _mm_storeu_ps((out_pixels + i), acc);
  }
}

void conv_2d(float *in_pixels, float *out_pixels, int width, int height) {
  float *tmp_rows[3];

  for (int i = 0; i < 3; ++i) {
    tmp_rows[i] = (float *)malloc(width * sizeof(width));
  }

  int len_1d = width - 4 - 1;
  for (int y = 1; y < height - 1; y++) {
    // Compute 3 1D convolutions over consecutive rows.
    conv_1d(in_pixels + (y - 1) * width, tmp_rows[0], len_1d, blur_kernel[0]);
    conv_1d(in_pixels + y * width, tmp_rows[1], len_1d, blur_kernel[1]);
    conv_1d(in_pixels + (y + 1) * width, tmp_rows[2], len_1d, blur_kernel[2]);

    // Sum them up and store.
    for (int x = 0; x <= width - 4 - 1; x += 4) {
      __m128 sum1 = _mm_add_ps(_mm_load_ps(&tmp_rows[0][x]),
                               _mm_load_ps(&tmp_rows[1][x]));
      __m128 sum2 = _mm_add_ps(sum1, _mm_load_ps(&tmp_rows[2][x]));
      _mm_storeu_ps(&out_pixels[y * width + x], sum2);
    }
  }
}