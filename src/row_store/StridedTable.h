#pragma once
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "Constants.h"
#include "StridedTableVectorIterator.h"

// TODO test Iterator and create VectorRegister Iterator

namespace RowStore {

// TODO this is meant to implement an more complex (lane)-pages, round-robin, tuple allocation algortithm that enables better
// gathers-performance
template <class T>
class StridedTableIterator {
   private:
    using ValueType = typename T::Type;
    using PointerType = ValueType *;
    using ReferenceType = ValueType &;
    static const int StrideWidth = T::StrideWidth;

    ValueType const *baseAddress;
    PointerType currentAddress;

    const size_t tupleW;
    const size_t strideW;
    const size_t strideSetW;
    const size_t strideCapacity;  // in tuples

    const uint32_t stridesPerSet;
    uint64_t pos = 0;
    uint32_t strideNum = 0;  // stride of set
    uint64_t strideSet = 0;  // stride set
    uint64_t stridePos = 0;  // position inside stride

   public:
    StridedTableIterator(PointerType baseAddress, size_t tupleWidth)
        : baseAddress(baseAddress),
          currentAddress(baseAddress),
          tupleW(tupleWidth),
          strideW(StrideWidth / sizeof(ValueType)),
          strideSetW((StrideWidth / sizeof(ValueType)) * (VECTOR_BYTE_WIDTH / sizeof(ValueType))),
          strideCapacity(StrideWidth / (tupleWidth * sizeof(ValueType))),
          stridesPerSet(VECTOR_BYTE_WIDTH / sizeof(ValueType)) {}

    StridedTableIterator(PointerType baseAddress, size_t tupleWidth, uint64_t position) : StridedTableIterator(baseAddress, tupleWidth) {
        pos = position;
        strideSet = pos / (strideCapacity * stridesPerSet);
        strideNum = pos % stridesPerSet;
        stridePos = (pos / stridesPerSet) % strideCapacity;
        currentAddress += strideSet * strideSetW + strideNum * strideW + stridePos * tupleW;
    }

    uint64_t getPos() { return pos; }

    StridedTableIterator &operator++() {
        ++pos;
        strideNum = (++strideNum) % stridesPerSet;
        currentAddress += strideW;

        int if_ = (strideNum == 0);
        stridePos += if_;
        stridePos = stridePos % strideCapacity;
        currentAddress -= if_ * strideSetW;
        currentAddress += if_ * tupleW;

        int if__ = (stridePos == 0) && (strideNum == 0);
        strideSet = strideSet + if__;
        currentAddress -= if__ * strideW;
        currentAddress += if__ * strideSetW;

        return *this;
    }

    StridedTableIterator operator++(int) {
        StridedTableIterator iter = *this;
        ++(*this);
        return iter;
    }

    ReferenceType operator[](uint64_t index) {
        uint32_t tmpStrideSet = index / (strideCapacity * stridesPerSet);
        uint64_t tmpStrideNum = index % stridesPerSet;
        uint64_t tmpStridePos = (index / stridesPerSet) % strideCapacity;
        PointerType address = baseAddress + tmpStrideSet * strideSetW + tmpStrideNum * strideW + tmpStridePos * tupleW;

        return *address;
    }

    PointerType operator->() { return currentAddress; }

    PointerType getAddress() { return currentAddress; }

    ReferenceType operator*() { return *currentAddress; }

    bool operator==(const StridedTableIterator &other) const { return other.pos == pos; }

    bool operator!=(const StridedTableIterator &other) const { return other.pos != pos; }
};

///
/// \tparam T base type
/// \tparam S stride with in byte
template <typename T, int S>
class StridedTable {
   public:
    using Type = T;
    static const int StrideWidth = S;

   private:
    size_t tupleWidth;

    size_t capacity;
    size_t strideCapacity;
    size_t strideCount;
    StridedTableIterator<StridedTable<T, S>> *writeIterator;
    T *data;

   public:
    /// Initializes the StridedTable with a given tuple width.
    /// \param tupleWidth number of attributes per tuple
    StridedTable(uint32_t tupleWidth, size_t tupleCapacity) : tupleWidth(tupleWidth), capacity(tupleCapacity) {
        size_t requiredMemory = tupleWidth * tupleCapacity * sizeof(T);

        strideCapacity = StrideWidth / (tupleWidth * sizeof(T));
        size_t requiredStrides = tupleCapacity / strideCapacity + ((tupleCapacity % strideCapacity) != 0);
        size_t laneCount = VECTOR_BYTE_WIDTH / sizeof(T);
        strideCount = requiredStrides + (laneCount - (requiredStrides % laneCount));
        size_t tmp = strideCount * StrideWidth;

        // allloc memory that is aligned at the page boundaries
        data = (T *)aligned_alloc(StrideWidth, strideCount * StrideWidth);

        writeIterator = new StridedTableIterator<StridedTable<T, S>>(data, tupleWidth);
    }

    /// Initializes the StridedTable with a given tuple width and initial data. The content of the inital data vector
    /// is copied to the table's data storage
    /// \param tupleWidth number of attributes per tuple
    /// \param initData data that is copied into the table
    StridedTable(uint32_t tupleWidth, std::vector<T *> &initData) : StridedTable(tupleWidth, initData.size()) {
        auto &wIter = *writeIterator;
        for (uint32_t i = 0; i < initData.size(); ++i) {
            // increment iterator and get address of old iterator position (in memcpy)
            std::memcpy(wIter++.getAddress(), initData[i], tupleWidth);
        }
    }

    /// Destructor
    ~StridedTable() {
        delete writeIterator;
        free(data);
    }

    /// Returns iterator pointing to the first address of the allocated memory
    StridedTableIterator<StridedTable<T, S>> begin() { return StridedTableIterator<StridedTable<T, S>>(data, tupleWidth); }

    /// Returns iterator pointing to the address one past the last tuple of the table
    StridedTableIterator<StridedTable<T, S>> end() { return *writeIterator; }

    /// Appends a row to the table and increments the write iterator
    void addRow(T *src) { std::memcpy((*writeIterator)++.getAddress(), src, tupleWidth); }

    /// Returns the number of tuples that are contaied in this table
    uint64_t count() {
        // TODO do something fancy counting stuff
        return writeIterator->getPos();
    }

    /// Returns the number of values in a tuple, aka number of attributes of the table.
    uint32_t getTupleWidth() { return tupleWidth; }
};
}  // namespace RowStore
