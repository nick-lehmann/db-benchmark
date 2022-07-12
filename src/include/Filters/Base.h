#pragma once
#include <immintrin.h>

#include "Constants.h"
#include "SIMD.h"

namespace Filters {

template <typename T, SIMD Variant>
class Filter {};

template <>
class Filter<uint32_t, SIMD::AVX512> {
   public:
    const unsigned index;
    const uint32_t value;
    __m512i value_register;

    Filter(unsigned index, uint32_t value) : index(index), value(value), value_register(_mm512_set1_epi32(value)) {}
    virtual __mmask16 match(__m512i reg, __mmask16 mask = ONE_MASK) const = 0;
};

template <>
class Filter<uint64_t, SIMD::AVX512> {
   public:
    const unsigned index;
    const uint64_t value;
    __m512i value_register;

    Filter(unsigned index, uint64_t value) : index(index), value(value), value_register(_mm512_set1_epi64(value)) {}
    virtual __mmask8 match(__m512i reg, __mmask8 mask = ONE_MASK) const = 0;
};

template <typename T>
class Filter<T, SIMD::None> {
   public:
    unsigned index;
    const T value;

    Filter(unsigned index, T value) : index(index), value(value) {}
    virtual bool match(T value) = 0;
};
}  // namespace Filters