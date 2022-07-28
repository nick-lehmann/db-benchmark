#pragma once

#include "Constants.h"
#include "Iterator.h"

namespace RowStore {

/// This Class is an Iterator which is used to access the IntermediateTables data.
/// \tparam T base type
/// \tparam V simd variant, decides the internal data layout of the table
/// \tparam A alignment of the data and stridesize (shold be a multiple of sizeof(T))
template <typename T, SIMD V, int A>
class ScalarIterator : public IntermediateIterator<T, V, A> {};

template <typename T, int A>
class ScalarIterator<T, SIMD::None, A> : public IntermediateIterator<T, SIMD::None, A> {
   public:
    ScalarIterator(T *baseAddress, uint32_t tupleWidth) : IntermediateIterator<T, SIMD::None, A>(baseAddress, tupleWidth) {
        this->currentAddress = baseAddress;
    }

    ScalarIterator(T *baseAddress, uint32_t tupleWidth, uint64_t position) : ScalarIterator(baseAddress, tupleWidth) {
        this->currentAddress += position * tupleWidth;
        this->pos = position;
    }

    IntermediateIterator<T, SIMD::None, A> *operator++() override {
        ++(this->pos);
        this->currentAddress += this->tupleWidth;

        return this;
    }

   public:
    T *addressOf(uint64_t index) override { return const_cast<T *>(this->baseAddress + index * this->tupleWidth); }
};

template <typename T, int A>
class ScalarIterator<T, SIMD::AVX512, A> : public IntermediateIterator<T, SIMD::AVX512, A> {
   public:
    ScalarIterator(T *baseAddress, uint32_t tupleWidth) : IntermediateIterator<T, SIMD::AVX512, A>(baseAddress, tupleWidth) {
        this->currentAddress = baseAddress;
    }

    ScalarIterator(T *baseAddress, uint32_t tupleWidth, uint64_t position) : ScalarIterator(baseAddress, tupleWidth) {
        this->currentAddress += position * tupleWidth;
        this->pos = position;
    }

    IntermediateIterator<T, SIMD::AVX512, A> *operator++() override {
        ++(this->pos);
        this->currentAddress += this->tupleWidth;

        return this;
    }

   public:
    T *addressOf(uint64_t index) override { return const_cast<T *>(this->baseAddress + index * this->tupleWidth); }
};

template <typename T, int A>
class ScalarIterator<T, SIMD::AVX512_Strided, A> : public IntermediateIterator<T, SIMD::AVX512_Strided, A> {
   private:
    const size_t strideW;
    const size_t strideSetW;
    const size_t strideCapacity;  // in tuples

    const uint32_t stridesPerSet;
    uint32_t strideNum = 0;  // stride of set
    uint64_t strideSet = 0;  // stride set
    uint64_t stridePos = 0;  // position inside stride

   public:
    ScalarIterator(T *baseAddress, uint32_t tupleWidth)
        : IntermediateIterator<T, SIMD::AVX512_Strided, A>(baseAddress, tupleWidth),
          strideW(A / sizeof(T)),
          strideSetW((A / sizeof(T)) * (VECTOR_BYTE_WIDTH / sizeof(T))),
          strideCapacity(A / (tupleWidth * sizeof(T))),
          stridesPerSet(VECTOR_BYTE_WIDTH / sizeof(T)) {
        this->currentAddress = baseAddress;
    }

    ScalarIterator(T *baseAddress, uint32_t tupleWidth, uint64_t position) : ScalarIterator(baseAddress, tupleWidth) {
        this->pos = position;
        strideSet = this->pos / (strideCapacity * stridesPerSet);
        strideNum = this->pos % stridesPerSet;
        stridePos = (this->pos / stridesPerSet) % strideCapacity;
        this->currentAddress += strideSet * strideSetW + strideNum * strideW + stridePos * this->tupleWidth;
    }

    IntermediateIterator<T, SIMD::AVX512_Strided, A> *operator++() override {
        ++(this->pos);
        strideNum = (++strideNum) % stridesPerSet;
        this->currentAddress += strideW;

        int if_ = (strideNum == 0);
        stridePos += if_;
        stridePos = stridePos % strideCapacity;
        this->currentAddress -= if_ * strideSetW;
        this->currentAddress += if_ * this->tupleWidth;

        int if__ = (stridePos == 0) && (strideNum == 0);
        strideSet = strideSet + if__;
        this->currentAddress -= if__ * strideW;
        this->currentAddress += if__ * strideSetW;

        return this;
    }

   public:
    T *addressOf(uint64_t index) override {
        uint32_t tmpStrideSet = index / (strideCapacity * stridesPerSet);
        uint64_t tmpStrideNum = index % stridesPerSet;
        uint64_t tmpStridePos = (index / stridesPerSet) % strideCapacity;
        T *address =
            const_cast<T *>(this->baseAddress + tmpStrideSet * strideSetW + tmpStrideNum * strideW + tmpStridePos * this->tupleWidth);

        return address;
    }
};

}  // namespace RowStore