#pragma once
#include <immintrin.h>

#include <cstdlib>

const size_t PAGE_SIZE = 4096;  // in bytes

const size_t VECTOR_BIT_WIDTH = 512;
const size_t VECTOR_BYTE_WIDTH = VECTOR_BIT_WIDTH / 8;

const __mmask16 ONE_MASK = _mm512_int2mask(INT32_MAX);

const __m512i ONE_REGISTER_32 = _mm512_set1_epi32(1);
const __m512i ONE_REGISTER_64 = _mm512_set1_epi64(1);
