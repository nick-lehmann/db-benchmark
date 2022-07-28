#pragma once

#include "Constants.h"
#include "Iterator.h"
#include "immintrin.h"

namespace RowStore {

/// This Helper is used to gather data from the memory to vector registers. The gathering procedure changes according to the base type, SIMD
/// variant and data alignement/stride size
/// \tparam T base type
/// \tparam V simd variant, decides the internal data layout of the table
/// \tparam A alignment of the data and stridesize (shold be a multiple of sizeof(T))
template <typename T, SIMD V, int A>
class VectorIteratorHelper;

template <int A>
class VectorIteratorHelper<uint32_t, SIMD::AVX512, A> {
   private:
    const __m512i vIndex;

   public:
    VectorIteratorHelper(uint32_t tupleWidth)
        : vIndex(_mm512_set_epi32(0 * tupleWidth, 1 * tupleWidth, 2 * tupleWidth, 3 * tupleWidth, 4 * tupleWidth, 5 * tupleWidth,
                                  6 * tupleWidth, 7 * tupleWidth, 8 * tupleWidth, 9 * tupleWidth, 10 * tupleWidth, 11 * tupleWidth,
                                  12 * tupleWidth, 13 * tupleWidth, 14 * tupleWidth, 15 * tupleWidth)) {}
    /// Gather method
    /// \param baseAddress source address for the gather method
    /// \param tupleWidth width of the tuples in the IntermediateTable
    const __m512i gather(uint32_t *baseAddress) { return _mm512_i32gather_epi32(vIndex, baseAddress, 4); }
};

template <int A>
class VectorIteratorHelper<uint64_t, SIMD::AVX512, A> {
   private:
    const __m512i vIndex;

   public:
    VectorIteratorHelper(uint32_t tupleWidth)
        : vIndex(_mm512_set_epi64(0 * tupleWidth, 1 * tupleWidth, 2 * tupleWidth, 3 * tupleWidth, 4 * tupleWidth, 5 * tupleWidth,
                                  6 * tupleWidth, 7 * tupleWidth)) {}

    const __m512i gather(uint64_t *baseAddress) { return _mm512_i64gather_epi64(vIndex, baseAddress, 8); }
};

template <int A>
class VectorIteratorHelper<uint32_t, SIMD::AVX512_Strided, A> {
   private:
    const __m512i vIndex = _mm512_set_epi32(
        0 * (A / sizeof(uint32_t)), 1 * (A / sizeof(uint32_t)), 2 * (A / sizeof(uint32_t)), 3 * (A / sizeof(uint32_t)),
        4 * (A / sizeof(uint32_t)), 5 * (A / sizeof(uint32_t)), 6 * (A / sizeof(uint32_t)), 7 * (A / sizeof(uint32_t)),
        8 * (A / sizeof(uint32_t)), 9 * (A / sizeof(uint32_t)), 10 * (A / sizeof(uint32_t)), 11 * (A / sizeof(uint32_t)),
        12 * (A / sizeof(uint32_t)), 13 * (A / sizeof(uint32_t)), 14 * (A / sizeof(uint32_t)), 15 * (A / sizeof(uint32_t)));

   public:
    /// Gather method
    /// \param baseAddress source address for the gather method
    const __m512i gather(uint32_t *baseAddress) { return _mm512_i32gather_epi32(vIndex, baseAddress, 4); }
};

template <int A>
class VectorIteratorHelper<uint64_t, SIMD::AVX512_Strided, A> {
   private:
    const __m512i vIndex =
        _mm512_set_epi64(0 * (A / sizeof(uint64_t)), 1 * (A / sizeof(uint64_t)), 2 * (A / sizeof(uint64_t)), 3 * (A / sizeof(uint64_t)),
                         4 * (A / sizeof(uint64_t)), 5 * (A / sizeof(uint64_t)), 6 * (A / sizeof(uint64_t)), 7 * (A / sizeof(uint64_t)));

   public:
    const __m512i gather(uint64_t *baseAddress) { return _mm512_i64gather_epi64(vIndex, baseAddress, 8); }
};

//##########################################################################################################################################

/// This Class is an Iterator which is used to access the IntermediateTables data. On demand a vector register is prepared.
/// \tparam T base type
/// \tparam V simd variant, decides the internal data layout of the table
/// \tparam A alignment of the data and stridesize (shold be a multiple of sizeof(T))
template <typename T, SIMD V, int A>
class VectorIterator : public IntermediateIterator<T, V, A> {};

template <typename T, int A>
class VectorIterator<T, SIMD::None, A> : public IntermediateIterator<T, SIMD::None, A> {
   public:
    static const uint32_t LaneMultiplier = 1;

   public:
    VectorIterator(T *baseAddress, uint32_t tupleWidth) : IntermediateIterator<T, SIMD::None, A>(baseAddress, tupleWidth) {
        this->currentAddress = baseAddress;
    }

    VectorIterator(T *baseAddress, uint32_t tupleWidth, uint64_t position) : VectorIterator(baseAddress, tupleWidth) {
        this->currentAddress += position * tupleWidth;
        this->pos = position;
    }

    ScalarIterator<T, SIMD::None, A> *getScalarIterator() {
        return new ScalarIterator<T, SIMD::None, A>(const_cast<T *>(this->baseAddress), this->tupleWidth, this->pos);
    }

    IntermediateIterator<T, SIMD::None, A> *operator++() override {
        ++(this->pos);
        this->currentAddress += this->tupleWidth;

        return this;
    }

    T gather(const uint32_t column) { return this->currentAddress[column]; }

   public:
    T *addressOf(uint64_t index) override { return const_cast<T *>(this->baseAddress + index * this->tupleWidth); }
};

template <typename T, int A>
class VectorIterator<T, SIMD::AVX512, A> : public IntermediateIterator<T, SIMD::AVX512, A> {
   public:
    static const uint32_t LaneMultiplier = VECTOR_BYTE_WIDTH / sizeof(T);

   private:
    VectorIteratorHelper<T, SIMD::AVX512, A> helper;

   public:
    VectorIterator(T *baseAddress, uint32_t tupleWidth)
        : IntermediateIterator<T, SIMD::AVX512, A>(baseAddress, tupleWidth), helper(tupleWidth) {
        this->currentAddress = baseAddress;
    }

    VectorIterator(T *baseAddress, uint32_t tupleWidth, uint64_t position) : VectorIterator(baseAddress, tupleWidth) {
        this->currentAddress += position * tupleWidth * VectorIterator<T, SIMD::AVX512, A>::LaneMultiplier;
        this->pos = position;
    }

    ScalarIterator<T, SIMD::AVX512, A> *getScalarIterator() {
        return new ScalarIterator<T, SIMD::AVX512, A>(const_cast<T *>(this->baseAddress), this->tupleWidth, this->pos * LaneMultiplier);
    }

    IntermediateIterator<T, SIMD::AVX512, A> *operator++() override {
        ++(this->pos);
        this->currentAddress += this->tupleWidth * VectorIterator<T, SIMD::AVX512, A>::LaneMultiplier;

        return this;
    }

    __m512i gather(const uint32_t column) { return helper.gather(this->currentAddress + column); }

   public:
    T *addressOf(uint64_t index) override { return const_cast<T *>(this->baseAddress + index * this->tupleWidth); }
};

template <typename T, int A>
class VectorIterator<T, SIMD::AVX512_Strided, A> : public IntermediateIterator<T, SIMD::AVX512_Strided, A> {
   public:
    static const uint32_t LaneMultiplier = VECTOR_BYTE_WIDTH / sizeof(T);

   private:
    VectorIteratorHelper<T, SIMD::AVX512_Strided, A> helper;

    const size_t strideW;
    const size_t strideSetW;
    const size_t strideCapacity;  // in tuples

    const uint32_t stridesPerSet;
    uint64_t strideSet = 0;  // stride set
    uint64_t stridePos = 0;  // position inside stride

   public:
    VectorIterator(T *baseAddress, uint32_t tupleWidth)
        : IntermediateIterator<T, SIMD::AVX512_Strided, A>(baseAddress, tupleWidth),
          helper(),
          strideW(A / sizeof(T)),
          strideSetW((A / sizeof(T)) * (VECTOR_BYTE_WIDTH / sizeof(T))),
          strideCapacity(A / (tupleWidth * sizeof(T))),
          stridesPerSet(VECTOR_BYTE_WIDTH / sizeof(T)) {
        this->currentAddress = baseAddress;
    }

    VectorIterator(T *baseAddress, uint32_t tupleWidth, uint64_t position) : VectorIterator(baseAddress, tupleWidth) {
        this->pos = position;
        strideSet = this->pos / strideCapacity;
        stridePos = this->pos % strideCapacity;
        this->currentAddress += strideSet * strideSetW + stridePos * this->tupleWidth;
    }

    ScalarIterator<T, SIMD::AVX512_Strided, A> *getScalarIterator() {
        return new ScalarIterator<T, SIMD::AVX512_Strided, A>(const_cast<T *>(this->baseAddress), this->tupleWidth,
                                                              this->pos * stridesPerSet);
    }

    IntermediateIterator<T, SIMD::AVX512_Strided, A> *operator++() override {
        ++(this->pos);

        stridePos = (++stridePos) % strideCapacity;
        this->currentAddress += this->tupleWidth;

        int if_ = (stridePos == 0);
        strideSet = strideSet + if_;
        this->currentAddress -= if_ * strideW;
        this->currentAddress += if_ * strideSetW;

        return this;
    }

    __m512i gather(const uint32_t column) { return helper.gather(this->currentAddress + column); }

   protected:
    T *addressOf(uint64_t index) override {
        uint32_t tmpStrideSet = index / strideCapacity;
        uint64_t tmpStridePos = index % strideCapacity;
        T *address = const_cast<T *>(this->baseAddress + tmpStrideSet * strideSetW + tmpStridePos * this->tupleWidth);

        return address;
    }
};

}  // namespace RowStore