#pragma once

#include <immintrin.h>
#include <cstdint>

#include "Constants.h"

namespace Filters::AVX {
    template<typename T>
    class Filter {
    };

    template<>
    class Filter<uint32_t> {
    public:
        Filter(unsigned index, uint32_t value) : index(index), value(value),
                                                     value_register(_mm512_set1_epi32(value)) {
        }

        virtual __mmask16 match(__m512i reg, __mmask16 mask = ONE_MASK) const = 0;

    public:
        const __m512i value_register;
        const unsigned index;
        const uint32_t value;
    };

    template<>
    class Filter<uint64_t> {
    public:
        Filter(unsigned index, uint64_t value) : index(index), value(value), value_register(_mm512_set1_epi64(value)) {
        }

        virtual __mmask8 match(__m512i reg, __mmask8 mask = ONE_MASK) const = 0;

    public:
        const __m512i value_register;
        const unsigned index;
        const uint64_t value;
    };

    /// EQUAL FILTER

    template<typename T>
    class Equal : public Filter<T> {

    };

    template<>
    class Equal<uint32_t> : public Filter<uint32_t> {
        using Filter<uint32_t>::Filter;

        __mmask16 match(__m512i reg, __mmask16 mask) const override {
            return _mm512_mask_cmpeq_epi32_mask(mask, reg, value_register);
        }
    };

    template<>
    class Equal<uint64_t> : public Filter<uint64_t> {
        using Filter<uint64_t>::Filter;

        __mmask8 match(__m512i reg, __mmask8 mask) const override {
            return _mm512_mask_cmpeq_epi64_mask(mask, reg, value_register);
        }
    };

    /// NOT EQUAL

    template<typename T>
    class NotEqual : public Filter<T> {

    };

    template<>
    class NotEqual<uint32_t> : public Filter<uint32_t> {
        using Filter<uint32_t>::Filter;

        __mmask16 match(__m512i reg, __mmask16 mask) const override {
            return _mm512_mask_cmpneq_epi32_mask(mask, reg, value_register);
        }
    };

    template<>
    class NotEqual<uint64_t> : public Filter<uint64_t> {
        using Filter<uint64_t>::Filter;

        __mmask8 match(__m512i reg, __mmask8 mask) const override {
            return _mm512_mask_cmpneq_epi64_mask(mask, reg, value_register);
        }
    };

    /// GREATER THAN

    template<typename T>
    class GreaterThan : public Filter<T> {

    };

    template<>
    class GreaterThan<uint32_t> : public Filter<uint32_t> {
        using Filter<uint32_t>::Filter;

        __mmask16 match(__m512i reg, __mmask16 mask) const override {
            return _mm512_mask_cmpgt_epi32_mask(mask, reg, value_register);
        }
    };

    template<>
    class GreaterThan<uint64_t> : public Filter<uint64_t> {
        using Filter<uint64_t>::Filter;

        __mmask8 match(__m512i reg, __mmask8 mask) const override {
            return _mm512_mask_cmpgt_epi64_mask(mask, reg, value_register);
        }
    };

    /// GREATER EQUAL

    template<typename T>
    class GreaterEqual : public Filter<T> {

    };

    template<>
    class GreaterEqual<uint32_t> : public Filter<uint32_t> {
        using Filter<uint32_t>::Filter;

        __mmask16 match(__m512i reg, __mmask16 mask) const override {
            return _mm512_mask_cmpge_epi32_mask(mask, reg, value_register);
        }
    };

    template<>
    class GreaterEqual<uint64_t> : public Filter<uint64_t> {
        using Filter<uint64_t>::Filter;

        __mmask8 match(__m512i reg, __mmask8 mask) const override {
            return _mm512_mask_cmpge_epi64_mask(mask, reg, value_register);
        }
    };

    /// LESS THAN

    template<typename T>
    class LessThan : public Filter<T> {

    };

    template<>
    class LessThan<uint32_t> : public Filter<uint32_t> {
        using Filter<uint32_t>::Filter;

        __mmask16 match(__m512i reg, __mmask16 mask) const override {
            return _mm512_mask_cmplt_epi32_mask(mask, reg, value_register);
        }
    };

    template<>
    class LessThan<uint64_t> : public Filter<uint64_t> {
        using Filter<uint64_t>::Filter;

        __mmask8 match(__m512i reg, __mmask8 mask) const override {
            return _mm512_mask_cmplt_epi64_mask(mask, reg, value_register);
        }
    };

    /// LESS EQUAL

    template<typename T>
    class LessEqual : public Filter<T> {

    };

    template<>
    class LessEqual<uint32_t> : public Filter<uint32_t> {
        using Filter<uint32_t>::Filter;

        __mmask16 match(__m512i reg, __mmask16 mask) const override {
            return _mm512_mask_cmple_epi32_mask(mask, reg, value_register);
        }
    };

    template<>
    class LessEqual<uint64_t> : public Filter<uint64_t> {
        using Filter<uint64_t>::Filter;

        __mmask8 match(__m512i reg, __mmask8 mask) const override {
            return _mm512_mask_cmple_epi64_mask(mask, reg, value_register);
        }
    };
}
