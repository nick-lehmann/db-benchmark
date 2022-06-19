#pragma once
#include "./Base.h"
#include "SIMD.h"

namespace Filter {
template <typename T, SIMD Variant>
class LessThan : public Filter<T, Variant> {};

template <>
class LessThan<uint32_t, SIMD::AVX512> : public Filter<uint32_t, SIMD::AVX512> {
   public:
    using Filter<uint32_t, SIMD::AVX512>::Filter;

    __mmask16 match(__m512i reg, __mmask16 mask = ONE_MASK) { return _mm512_mask_cmpeq_epi32_mask(mask, reg, value_register); }
};
}  // namespace Filter