#pragma once

const __mmask16 ONE_MASK = _mm512_int2mask(INT32_MAX);

const __m512i ONE_REGISTER_32 = _mm512_set1_epi32(1);
const __m512i ONE_REGISTER_64 = _mm512_set1_epi64(1);