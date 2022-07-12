#pragma once
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "Constants.h"
#include "immintrin.h"

namespace RowStore {

template <typename T, int S>
class VectorIteratorHelper;

template <int S>
class VectorIteratorHelper<uint32_t, S> {
    static constexpr __m512i vIndex = _mm512_set1_epi32(S / sizeof(uint32_t));

    static constexpr __m512i gather(uint32_t *baseAddress) { return _mm512_i32gather_epi32(vIndex, baseAddress, 4); }
};

template <int S>
class VectorIteratorHelper<uint64_t, S> {
    static constexpr __m512i vIndex = _mm512_set1_epi64(S / sizeof(uint64_t));

    static constexpr __m512i gather(uint64_t *baseAddress) { return _mm512_i64gather_epi64(vIndex, baseAddress, 8); }
};

template <typename T, int S>
class StridedTableVectorIterator {
   private:
    using Helper = VectorIteratorHelper<T, S>;

    using VectorType = __m512i;
    using ValueType = T;
    using PointerType = ValueType *;
    using ReferenceType = ValueType &;
    static const int StrideWidth = S;

    ValueType const *baseAddress;
    PointerType currentAddress;

    const size_t tupleW;
    const size_t strideW;
    const size_t strideSetW;
    const size_t strideCapacity;  // in tuples

    const uint32_t stridesPerSet;
    uint64_t pos = 0;
    uint64_t strideSet = 0;  // stride set
    uint64_t stridePos = 0;  // position inside stride

   public:
    StridedTableVectorIterator(PointerType baseAddress, size_t tupleWidth)
        : baseAddress(baseAddress),
          currentAddress(baseAddress),
          tupleW(tupleWidth),
          strideW(StrideWidth / sizeof(ValueType)),
          strideSetW((StrideWidth / sizeof(ValueType)) * (VECTOR_BYTE_WIDTH / sizeof(ValueType))),
          strideCapacity(StrideWidth / (tupleWidth * sizeof(ValueType))),
          stridesPerSet(VECTOR_BYTE_WIDTH / sizeof(ValueType)) {}

    StridedTableVectorIterator(PointerType baseAddress, size_t tupleWidth, uint64_t position)
        : StridedTableVectorIterator(baseAddress, tupleWidth) {
        pos = position;
        strideSet = pos / strideCapacity;
        stridePos = pos % strideCapacity;
        currentAddress += strideSet * strideSetW + stridePos * tupleW;
    }

    uint64_t getPos() { return pos; }

    StridedTableVectorIterator &operator++() {
        ++pos;

        stridePos = (++stridePos) % strideCapacity;
        currentAddress += tupleW;

        int if_ = (stridePos == 0);
        strideSet = strideSet + if_;
        currentAddress -= if_ * strideW;
        currentAddress += if_ * strideSetW;

        return *this;
    }

    StridedTableVectorIterator operator++(int) {
        StridedTableVectorIterator iter = *this;
        ++(*this);
        return iter;
    }

    ReferenceType operator[](uint64_t index) {
        uint32_t tmpStrideSet = index / strideCapacity;
        uint64_t tmpStridePos = index % strideCapacity;
        PointerType address = baseAddress + tmpStrideSet * strideSetW + tmpStridePos * tupleW;

        return *address;
    }

    PointerType operator->() { return currentAddress; }

    PointerType getAddress() { return currentAddress; }

    ReferenceType operator*() { return *currentAddress; }

    VectorType gather(uint32_t column) { return Helper::gather(currentAddress + column); }

    bool operator==(const StridedTableVectorIterator &other) const { return other.pos == pos; }

    bool operator!=(const StridedTableVectorIterator &other) const { return other.pos != pos; }
};
}  // namespace RowStore
