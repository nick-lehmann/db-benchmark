#pragma once
#include <immintrin.h>

#include "./Base.h"
#include "Constants.h"
#include "SIMD.h"

namespace Filters {

template <typename T, SIMD Variant>
class Equal : public Filter<T, Variant> {};

template <>
class Equal<uint32_t, SIMD::AVX512> : public Filter<uint32_t, SIMD::AVX512> {
   public:
    using Filter<uint32_t, SIMD::AVX512>::Filter;

    __mmask16 match(__m512i reg, __mmask16 mask = ONE_MASK) const override {
        return _mm512_mask_cmpeq_epi32_mask(mask, reg, value_register);
    }
};

template <>
class Equal<uint64_t, SIMD::AVX512> : public Filter<uint64_t, SIMD::AVX512> {
    using Filter<uint64_t, SIMD::AVX512>::Filter;

    __mmask8 match(__m512i reg, __mmask8 mask = ONE_MASK) const override { return _mm512_mask_cmpeq_epi64_mask(mask, reg, value_register); }
};

template <>
class Equal<uint32_t, SIMD::AVX512_Strided> : public Filter<uint32_t, SIMD::AVX512_Strided> {
   public:
    using Filter<uint32_t, SIMD::AVX512_Strided>::Filter;

    __mmask16 match(__m512i reg, __mmask16 mask = ONE_MASK) const override {
        return _mm512_mask_cmpeq_epi32_mask(mask, reg, value_register);
    }
};

template <>
class Equal<uint64_t, SIMD::AVX512_Strided> : public Filter<uint64_t, SIMD::AVX512_Strided> {
    using Filter<uint64_t, SIMD::AVX512_Strided>::Filter;

    __mmask8 match(__m512i reg, __mmask8 mask = ONE_MASK) const override { return _mm512_mask_cmpeq_epi64_mask(mask, reg, value_register); }
};

template <typename T>
class Equal<T, SIMD::None> : public Filter<T, SIMD::None> {
   public:
    using Filter<T, SIMD::None>::Filter;

    bool match(T value) override { return this->value == value; }
};
};  // namespace Filters