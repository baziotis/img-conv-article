#include <stdlib.h>
#include <string.h>
#include <immintrin.h>

#include "img_lib.cpp"
#include "print_vec.cpp"

// 1x3 blur kernel
static float kernel[3] = {0.33f, 0.33f, 0.33f};

void conv_ser(Image img_in, Image img_out) {
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

void conv_vec(Image img_in, Image img_out) {
  // Set rounding to round down because that's what the casts do in C.
  // Get current MXCSR value
  unsigned int mxcsr_value = _mm_getcsr();
  // Clear rounding control bits (13-14)
  mxcsr_value &= ~(3 << 13);
  // Set rounding control to "round down" (01)
  mxcsr_value |= (1 << 13);
  // Set the modified value back to MXCSR               
  _mm_setcsr(mxcsr_value);

  assert(img_in.height == img_out.height);
  assert(img_in.width == img_in.width);

  int width = img_in.width;
  int height = img_in.height;

  uint8_t *in_pixels = img_in.pixels;
  uint8_t *out_pixels = img_out.pixels;


	int i, k;
  __m128 kernel_vec[3];

	// Repeat each kernel value in a 4-wide register
	for(i = 0; i < 3; ++i) {
		kernel_vec[i] = _mm_set1_ps(kernel[i]);
	}

  int len = (width*height) - 4 - 1;

	for(i = 1; i < len; i+=4) {
		// Zero accumulator
		__m128 acc = _mm_setzero_ps();

		for(k = -1; k <= 1; ++k) {
      // Load 4 uint8_t values into a 128-bit vector
      __m128i uint8_vec = _mm_loadu_si32(in_pixels + i + k);

      // Zero-extend the uint8_t values to 32-bit integers
      __m128i uint32_vec = _mm_cvtepu8_epi32(uint8_vec);

      // Convert the integers to floats
      __m128 float_vec = _mm_cvtepi32_ps(uint32_vec);

      // Make the products
			__m128 prod = _mm_mul_ps(kernel_vec[k + 1], float_vec);

			// Accumulate the 4 parallel values
			acc = _mm_add_ps(acc, prod);

		}

    // Convert the floats to 32-bit integers
    __m128i int32_vec = _mm_cvtps_epi32(acc);

    __m128i zero = _mm_setzero_si128();

    // Pack 32-bit integers into 16-bit integers (saturating to avoid overflow)
    __m128i int16_vec = _mm_packs_epi32(int32_vec, zero);

    // Pack 16-bit integers into 8-bit integers (saturating to avoid overflow)
    __m128i uint8_vec = _mm_packus_epi16(int16_vec, zero);

		// Store. Note we go +4 every time.
		_mm_storeu_si32((out_pixels + i), uint8_vec);
  }
}

int main() {
  const char *filepath = "./images_in/window.pgm";
  Image img = read_pgm(filepath);

  uint8_t *pixels_vec = (uint8_t *)malloc(img.width * img.height);
  Image blurred_vec = {
      .width = img.width, .height = img.height, .pixels = pixels_vec};

  uint8_t *pixels_ser = (uint8_t *)malloc(img.width * img.height);
  Image blurred_ser = {
      .width = img.width, .height = img.height, .pixels = pixels_ser};

  conv_vec(img, blurred_vec);
  conv_ser(img, blurred_ser);

  // We expect not to handle the last couple of pixels because we don't handle
  // the edges.
  for (int i = 0; i < (img.height * img.width); ++i) {
    if (pixels_ser[i] != pixels_vec[i]) {
      printf("i: %d\n", i);
      printf("distance from end: %d\n", (img.height * img.width)-i);
      printf("ser: %u\n", pixels_ser[i]);
      printf("vec: %u\n", pixels_vec[i]);
      assert(false);
    }
  }

  return 0;
}