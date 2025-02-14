#ifndef PRINT_VEC
#define PRINT_VEC

#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>

void print_m128(__m128 vec) {
  float values[4];
  _mm_storeu_ps(values, vec);
  printf("Vector: [%f, %f, %f, %f]\n", values[0], values[1], values[2], values[3]);
}

void print_m128i_u8(__m128i vec) {
  uint8_t values[16];
  _mm_storeu_si128((__m128i_u *)values, vec);
  printf("Vector: [%u, %u, %u, %u]\n", values[0], values[1], values[2], values[3]);
}

void print_m128i_i32(__m128i vec) {
  int values[4];
  _mm_storeu_si128((__m128i_u *)values, vec);
  printf("Vector: [%d, %d, %d, %d]\n", values[0], values[1], values[2], values[3]);
}

#endif